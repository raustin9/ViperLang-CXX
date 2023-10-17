# Build the testbed
CXX=clang++
CCFLAGS=-std=c++17 -g -fPIC -fdeclspec
INCLUDES=-Isrc -I../compiler/src
LDFLAGS=-L../bin/ -lcompiler -Wl,-rpath,./bin/
CCFILES=$(shell find . -type f -name "*.cc")
DEFINES=-DHIMPORT -D_DEBUG

all: pre lexer parser codegen

#testbed: ../bin/testbed
#	echo "Testbed built successfully"
#	cd ..

pre: ../bin/preprocessor

lexer: pre ../bin/lexer

parser: lexer ../bin/parser

codegen:
	echo "coming soon :)"

# Compile the binaries

../bin/preprocessor: src/preprocessor.cc
	$(CXX) $< -o $@ $(DEFINES) $(INCLUDES) $(LDFLAGS)

../bin/lexer: src/lexer.cc
	$(CXX) $< -o $@ $(DEFINES) $(INCLUDES) $(LDFLAGS)

../bin/parser: src/parser.cc
	$(CXX) $< -o $@ $(DEFINES) $(INCLUDES) $(LDFLAGS)

../bin/codegen: src/codegen.cc
	$(CXX) $< -o $@ $(DEFINES) $(INCLUDES) $(LDFLAGS)


#../bin/testbed: 
#	$(CXX) $(CCFILES) $(CCFLAGS) -o $@ $(DEFINES) $(INCLUDES) $(LDFLAGS)
