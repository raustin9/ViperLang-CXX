CC := clang++
CXXSPEC := c++20

BUILD_DIR := bin
OBJ_DIR := obj

ASSEMBLY := viper# change this to the name of the assembly you want to build
COMPILER_FLAGS := -g -Werror -Wall -std=$(CXXSPEC)
INCLUDE_FLAGS := -I$(ASSEMBLY)/src -I$(ASSEMBLY)
LINKER_FLAGS := 
DEFINES := -DQDEBUG -DQEXPORT

SRC_FILES := $(shell find $(ASSEMBLY) -name *.cc)		# .cc files
DIRECTORIES := $(shell find $(ASSEMBLY) -type d)		# directories with .h files
OBJ_FILES := $(SRC_FILES:%=$(OBJ_DIR)/%.o)				# compiled .o objects

all: scaffold compile bin/$(ASSEMBLY)

# .PHONY: scaffold
scaffold: # create build directory
	@echo Scaffolding folder structure...
	@mkdir -p $(addprefix $(OBJ_DIR)/,$(DIRECTORIES))
	@echo Done.

# .PHONY: compile
compile: #compile .cc files
	@echo Compiling...

# .PHONY: bin/$(ASSEMBLY)
bin/$(ASSEMBLY): $(OBJ_FILES)
	$(CC) $(COMPILER_FLAGS) $(OBJ_FILES) -o $@ $(LDFLAGS)


# .PHONY: clean
clean: # clean build directory
	rm -rf $(BUILD_DIR)/$(ASSEMBLY)
	rm -rf $(OBJ_DIR)/$(ASSEMBLY)

$(OBJ_DIR)/%.cc.o: %.cc # compile .c to .o object
	@echo   $<...
	clang++ $< $(COMPILER_FLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)
