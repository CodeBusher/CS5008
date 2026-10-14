#!/bin/bash
pushd ../build

gcc -Wall -ggdb ../code/main.c -o test_lexer
ret_val=$?
if [ $ret_val -ne 0 ]; then
	echo ERROR: Failed to build
	exit $ret_val
fi

echo "Build success!"

echo === TEST ON EXAMPLE.JIVE ===

./test_lexer ../jive_programs/simple.jive
ret_val=$?
if [ $ret_val -ne 0 ]; then
	echo ERROR: Lexer returned an error compiling simple.jive
	exit $ret_val
fi

popd
