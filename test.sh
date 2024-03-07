#!/bin/bash

echo "Running tests"

make -f "Makefile.linux.mak" tests
errorlevel=$?
if [ $errorlevel -ne 0 ]
then
    echo "Error: $errorlevel" && exit
fi

./bin/tests
