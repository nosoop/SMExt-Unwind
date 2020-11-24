#include "extension.h"
#include "unwind.h"
#include <cxxabi.h>
#include <cstdlib>

#define UNW_LOCAL_ONLY
#include <libunwind.h>

HandleType_t g_CodeFrameIteratorType = 0;

void CodeFrameIteratorHandler::OnHandleDestroy(HandleType_t type, void* object) {
	if (type == g_CodeFrameIteratorType) {
		delete (CodeFrameIterator*) object;
	}
}

cell_t sm_CreateCodeFrameIterator(IPluginContext *pContext, const cell_t *params) {
	unw_cursor_t cursor;
	unw_context_t context;

	// Initialize cursor to current frame for local unwinding.
	unw_getcontext(&context);
	unw_init_local(&cursor, &context);

	// Unwind frames one by one, going up the frame stack.
	CodeFrameIterator *cfi = new CodeFrameIterator;
	cfi->m_Index = 0;
	while (unw_step(&cursor) > 0) {
		unw_word_t offset, pc;
		unw_get_reg(&cursor, UNW_REG_IP, &pc);
		if (pc == 0) {
			break;
		}
		
		CodeFrame cf;
		cf.m_Address = pc;
		char sym[256];
		if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
			char* nameptr = sym;
			int status;
			char* demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
			if (!status) {
				nameptr = demangled;
			}
			cf.m_Name = nameptr;
			std::free(demangled);
		}
		cf.m_Offset = offset;
		cfi->m_Frames.push_back(cf);
	}
	return g_pHandleSys->CreateHandle(g_CodeFrameIteratorType, cfi,
			pContext->GetIdentity(), myself->GetIdentity(), NULL);
}

cell_t sm_CodeFrameIteratorNext(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	CodeFrameIterator *cfi = nullptr;
	HandleError err;
	HandleSecurity sec{NULL, myself->GetIdentity()};
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CodeFrameIteratorType, &sec, (void **) &cfi))
			!= HandleError_None) {
		return pContext->ThrowNativeError("Invalid CodeFrameIterator handle %x (error %d)",
				hndl, err);
	}
	
	if (cfi->m_Index < cfi->m_Frames.size() - 1) {
		cfi->m_Index++;
		return true;
	}
	return false;
}

cell_t sm_CodeFrameIteratorGetCodeAddress(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	CodeFrameIterator *cfi = nullptr;
	HandleError err;
	HandleSecurity sec{NULL, myself->GetIdentity()};
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CodeFrameIteratorType, &sec, (void **) &cfi))
			!= HandleError_None) {
		return pContext->ThrowNativeError("Invalid CodeFrameIterator handle %x (error %d)",
				hndl, err);
	}
	
	return cfi->m_Frames[cfi->m_Index].m_Address;
}

cell_t sm_CodeFrameIteratorGetCodeOffset(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	CodeFrameIterator *cfi = nullptr;
	HandleError err;
	HandleSecurity sec{NULL, myself->GetIdentity()};
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CodeFrameIteratorType, &sec, (void **) &cfi))
			!= HandleError_None) {
		return pContext->ThrowNativeError("Invalid CodeFrameIterator handle %x (error %d)",
				hndl, err);
	}
	return cfi->m_Frames[cfi->m_Index].m_Offset;
}

cell_t sm_CodeFrameIteratorGetFunctionName(IPluginContext *pContext, const cell_t *params) {
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	
	CodeFrameIterator *cfi = nullptr;
	HandleError err;
	HandleSecurity sec{NULL, myself->GetIdentity()};
	if ((err = g_pHandleSys->ReadHandle(hndl, g_CodeFrameIteratorType, &sec, (void **) &cfi))
			!= HandleError_None) {
		return pContext->ThrowNativeError("Invalid CodeFrameIterator handle %x (error %d)",
				hndl, err);
	}
	
	size_t numBytes;
	pContext->StringToLocalUTF8(params[2], params[3],
			cfi->m_Frames[cfi->m_Index].m_Name.c_str(), &numBytes);
	
	return numBytes;
}
