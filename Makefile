# Build the entire project
COMPILER=compiler/
TESTBED=testbed/

all: $(COMPILER)Makefile $(TESTBED)Makefile
	echo "Building compiler..."
	(cd $(COMPILER); make)
	echo "Building testbed..."
	(cd $(TESTBED); make)

run: all 
	./bin/testbed examples/example1.hero

rerun: all
	./bin/testbed examples/example1.hero

# Test suite
lex-test: all
	./bin/lexer examples/example1.hero

pre-test: all
	./bin/preprocessor examples/example1.hero

parser-test: all
	./bin/parser examples/example1.hero

code-test: all
	./bin/codegen examples/example1.hero

test-all: all
	./bin/preprocessor examples/example1.hero
	./bin/lexer examples/example1.hero
	./bin/parser examples/example1.hero
	./bin/codegen examples/example1.hero

clean: 
	rm -f bin/* 
