# Build the testbed
CXX=clang++
CCFLAGS=-std=c++17 -g -fPIC -fdeclspec
INCLUDES=-Isrc -I../compiler/src
LDFLAGS=-L../bin/ -lcompiler -Wl,-rpath,./bin/
CCFILES=$(shell find . -type f -name "*.cc")
DEFINES=-DHIMPORT -D_DEBUG

testbed: ../bin/testbed
	echo "Testbed built successfully"
	cd ..

../bin/testbed: 
	$(CXX) $(CCFILES) $(CCFLAGS) -o $@ $(DEFINES) $(INCLUDES) $(LDFLAGS)
