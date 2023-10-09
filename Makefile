# Build the entire project
COMPILER=compiler/
TESTBED=testbed/

all: $(COMPILER)Makefile $(TESTBED)Makefile
	echo "Building compiler..."
	(cd $(COMPILER); make)
	echo "Building testbed..."
	(cd $(TESTBED); make)

run: 
	./bin/testbed

rerun: all
	./bin/testbed

clean: 
	rm -f bin/* 
