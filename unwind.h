#ifndef _INCLUDE_UNWIND_EXTENSION_NATIVES_H_
#define _INCLUDE_UNWIND_EXTENSION_NATIVES_H_

cell_t sm_DumpStackTrace(IPluginContext *pContext, const cell_t *params);

const sp_nativeinfo_t g_UnwindNatives[] = {
	{ "DumpStackTrace", sm_DumpStackTrace },
	
	{ NULL, NULL },
};

#endif
