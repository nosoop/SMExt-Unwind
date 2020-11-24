#include "extension.h"
#include "unwind.h"
#include <cxxabi.h>
#include <cstdlib>

#define UNW_LOCAL_ONLY
#include <libunwind.h>

cell_t sm_DumpStackTrace(IPluginContext *pContext, const cell_t *params) {
	unw_cursor_t cursor;
	unw_context_t context;

	// Initialize cursor to current frame for local unwinding.
	unw_getcontext(&context);
	unw_init_local(&cursor, &context);

	// Unwind frames one by one, going up the frame stack.
	while (unw_step(&cursor) > 0) {
		unw_word_t offset, pc;
		unw_get_reg(&cursor, UNW_REG_IP, &pc);
		if (pc == 0) {
			break;
		}
		
		char sym[256];
		if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
			char* nameptr = sym;
			int status;
			char* demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
			if (!status) {
				nameptr = demangled;
			}
			rootconsole->ConsolePrint("0x%lx: (%s + 0x%lx)", pc, nameptr, offset);
			std::free(demangled);
		} else {
			rootconsole->ConsolePrint("0x%lx: unable to obtain symbol name for this frame", pc);
		}
	}
	return 0;
}
