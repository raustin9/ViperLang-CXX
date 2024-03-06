#!/bin/bash

echo "Cleaning..."

make -f "Makefile.linux.mak" clean
errorlevel=$?
echo "Done"
