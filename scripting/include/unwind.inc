#if defined __unwind_ext
	#endinput
#endif

#define __unwind_ext

methodmap CodeFrameIterator < Handle {
	/**
	 * Creates a new stack frame iterator.
	 * The contents of the stack trace are already saved, we just use this to match SourceMod's
	 * own FrameIterator handle.
	 * 
	 * Once initialized, the iterator points to a valid entry.  You probably want to process
	 * that before calling CodeFrameIterator.Next().
	 */
	public native CodeFrameIterator();
	
	/**
	 * Returns if the iterator is not at the end, advancing to the next entry if it is valid.
	 */
	public native bool Next();
	
	/**
	 * Copies the demangled function name into the given buffer.
	 */
	public native int GetFunctionName(char[] buffer, int maxlen);
	
	/**
	 * The address of the function.
	 */
	property Address CodeAddress {
		public native get();
	}
	
	/**
	 * The offset into the function that the instruction pointer was located.
	 */
	property int CodeOffset {
		public native get();
	}
}

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
