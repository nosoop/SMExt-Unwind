#ifndef _INCLUDE_UNWIND_EXTENSION_NATIVES_H_
#define _INCLUDE_UNWIND_EXTENSION_NATIVES_H_

#include <string>
#include <vector>

extern HandleType_t g_CodeFrameIteratorType;

class CodeFrameIteratorHandler : public IHandleTypeDispatch {
	public:
	void OnHandleDestroy(HandleType_t type, void* object);
};

struct CodeFrame {
	std::string m_Name;
	size_t m_Address;
	size_t m_Offset;
};

struct CodeFrameIterator {
	size_t m_Index;
	std::vector<CodeFrame> m_Frames;
};

cell_t sm_CreateCodeFrameIterator(IPluginContext *pContext, const cell_t *params);
cell_t sm_CodeFrameIteratorNext(IPluginContext *pContext, const cell_t *params);
cell_t sm_CodeFrameIteratorGetCodeAddress(IPluginContext *pContext, const cell_t *params);
cell_t sm_CodeFrameIteratorGetCodeOffset(IPluginContext *pContext, const cell_t *params);
cell_t sm_CodeFrameIteratorGetFunctionName(IPluginContext *pContext, const cell_t *params);

const sp_nativeinfo_t g_UnwindNatives[] = {
	{ "CodeFrameIterator.CodeFrameIterator", sm_CreateCodeFrameIterator },
	{ "CodeFrameIterator.Next", sm_CodeFrameIteratorNext },
	{ "CodeFrameIterator.CodeAddress.get", sm_CodeFrameIteratorGetCodeAddress },
	{ "CodeFrameIterator.CodeOffset.get", sm_CodeFrameIteratorGetCodeOffset },
	{ "CodeFrameIterator.GetFunctionName", sm_CodeFrameIteratorGetFunctionName },
	
	{ NULL, NULL },
};

#endif
