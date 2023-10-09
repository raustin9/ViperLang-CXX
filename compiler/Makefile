# Build the compiler
CXX=clang++
CCFLAGS=-std=c++17 -g -shared -fdeclspec -fPIC
INCLUDES=-Isrc
LDFLAGS=
CCFILES=$(shell find . -type f -name "*.cc")
DEFINES=-DHEXPORT -D_DEBUG

compiler: ../bin/compiler.so
	echo "Compiler built successfully"
	cd ..

../bin/compiler.so: $(CCFILES)
	$(CXX) $(CCFILES) $(CCFLAGS) -o ../bin/libcompiler.so $(DEFINES) $(INCLUDES) $(LDFLAGS)
