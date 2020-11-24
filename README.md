# Unwind Extension for SourceMod

A wrapper for `libunwind` to get the full stack trace from a plugin.

Sometimes you want to empirically track something down which happens under certain conditions
without dealing with `gdb`.

Like, you know the resulting symptom, but you can't pin down exactly what the cause is.

## Dependencies

`libunwind-dev:i386` is required to build on Ubuntu.

## Building

1. Create a `build/` subdirectory within the project root, then set it as your working
directory.
2. Run `python ../configure.py --sm-path {SM_DIR}`, where `{SM_DIR}` is the directory where the
SourceMod project repository is located.
3. Run `ambuild`.  Output files will be generated.
