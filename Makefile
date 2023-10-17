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

test-lex: all
	./bin/testbed examples/example1.hero

clean: 
	rm -f bin/* 
