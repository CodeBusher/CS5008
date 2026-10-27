#!/bin/bash
pushd ../build

gcc -Wall -ggdb ../code/main.c -o jive
ret_val=$?
if [ $ret_val -ne 0 ]; then
	echo ERROR: Failed to build
	exit $ret_val
fi

echo "Build success!"

echo === TEST ON SIMPLE.JIVE ===

./jive ../jive_programs/simple.jive -o simple.asm
ret_val=$?
if [ $ret_val -ne 0 ]; then
	echo ERROR: Compiler returned an error compiling simple.jive
	exit $ret_val
fi

# nasm -felf64 simple.asm
# ret_val=$?
# if [ $ret_val -ne 0 ]; then
# 	echo ERROR: Assembler failed to assemble simple.asm
# 	exit $ret_val
# fi

# ld simple.o -o simple
# ret_val=$?
# if [ $ret_val -ne 0 ]; then
# 	echo ERROR: Linker failed to link simple.o
# 	exit $ret_val
# fi

popd