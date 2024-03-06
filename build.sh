#!/bin/bash

echo "Building everything..."

make -f "Makefile.linux.mak" all
errorlevel=$?
if [ $errorlevel -ne 0 ]
then
    echo "Error: $errorlevel" && exit
fi

echo "All assemblies built"
