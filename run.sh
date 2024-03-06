#!/bin/bash

echo "Running assembly"

make -f "Makefile.linux.mak" all
errorlevel=$?
if [ $errorlevel -ne 0 ]
then
    echo "Error: $errorlevel" && exit
fi

./bin/viper
