CC := g++-13
LD := ld
CXXSPEC := c++23

BUILD_DIR := bin
OBJ_DIR := obj

ASSEMBLY := viper
TEST_DIR := tests
COMPILER_FLAGS := -g -Werror -Wall -std=$(CXXSPEC) -fPIC
INCLUDE_FLAGS := -I$(ASSEMBLY)/src -I$(ASSEMBLY)
TEST_INCLUDE_FLAGS := -I$(TEST_DIR)/src -I$(TEST_DIR) 
LINKER_FLAGS := -shared
TEST_LINKER_FLAGS := -L./bin -lviper -Wl,-rpath,./bin/
DEFINES := -DQDEBUG -DQEXPORT

SRC_FILES := $(shell find $(ASSEMBLY) -name *.cc)		# .cc files
TEST_FILES := $(shell find $(TEST_DIR) -name *.cc)		# .cc files
DIRECTORIES := $(shell find $(ASSEMBLY) -type d)		# directories with .h files
OBJ_FILES := $(SRC_FILES:%=$(OBJ_DIR)/%.o)				# compiled .o objects
TEST_OBJ_FILES := $(TEST_FILES:%=$(OBJ_DIR)/%.o)				# compiled .o objects
TEST_DIRECTORIES := $(shell find $(TEST_DIR) -type d)		# directories with .h files
EXTENSION := .so

all: scaffold compile bin/$(ASSEMBLY)

tests: test_scaffold compile test_link bin/$(TEST_DIR) 

# .PHONY: scaffold
scaffold: # create build directory
	@echo Scaffolding folder structure...
	@mkdir -p $(addprefix $(OBJ_DIR)/,$(DIRECTORIES))
	@echo Done.

# .PHONY: scaffold
test_scaffold: # create build directory
	@echo Scaffolding folder structure...
	@mkdir -p $(addprefix $(OBJ_DIR)/,$(TEST_DIRECTORIES))
	@echo Done.

# .PHONY: compile
compile: #compile .cc files
	@echo Compiling...

.PHONY: bin/$(ASSEMBLY)
bin/$(ASSEMBLY): $(OBJ_FILES)
	@$(CC) $(COMPILER_FLAGS) $(OBJ_FILES) -o $@ $(LDFLAGS)

.PHONY: bin/$(TEST_DIR)
bin/$(TEST_DIR): $(TEST_OBJ_FILES) $(OBJ_FILES)
	@$(CC) $(COMPILER_FLAGS) $(TEST_OBJ_FILES) -o $@ $(TEST_INCLUDE_FLAGS) $(TEST_LINKER_FLAGS)

test_link: scaffold $(OBJ_FILES)
	@$(CC) $(OBJ_FILES) -o  $(BUILD_DIR)/lib$(ASSEMBLY)$(EXTENSION) $(LINKER_FLAGS)

# .PHONY: clean
clean: # clean build directory
	rm -rf $(BUILD_DIR)/$(ASSEMBLY)
	rm -rf $(OBJ_DIR)/$(ASSEMBLY)
	rm -rf $(BUILD_DIR)/$(TEST_DIR)
	rm -rf $(OBJ_DIR)/$(TEST_DIR)

$(OBJ_DIR)/%.cc.o: %.cc # compile .c to .o object
	@echo   $<...
	@$(CC) $< $(COMPILER_FLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS) $(TEST_INCLUDE_FLAGS)
