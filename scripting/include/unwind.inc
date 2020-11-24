#if defined __unwind_ext
	#endinput
#endif

#define __unwind_ext

native void DumpStackTrace();

public Extension __ext_unwinder = {
	name = "Unwind",
	file = "unwinder.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};

#if !defined REQUIRE_EXTENSIONS
public void __ext_unwinder_SetNTVOptional() {
}
#endif
