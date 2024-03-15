#!/bin/bash

echo "Running tests"
// export LD_LIBRARY_PATH=./bin

make -f "Makefile.linux.mak" tests
errorlevel=$?
if [ $errorlevel -ne 0 ]
then
    echo "Error: $errorlevel" && exit
fi

./bin/tests
