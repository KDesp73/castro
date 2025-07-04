CC = gcc

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DIST_DIR = dist
BIN_DIR = $(BUILD_DIR)/bin

# Binaries
ENGINE = $(BIN_DIR)/engine
TEST_MOVEGEN = $(BIN_DIR)/test_movegen
TEST_ENGINE = $(BIN_DIR)/test_engine

# Sanitizers
SANITIZERS = -fsanitize=address,undefined

type ?= RELEASE

LIBRARY_NAME = castro
SO_NAME      = lib$(LIBRARY_NAME).so
A_NAME       = lib$(LIBRARY_NAME).a

TEST_DIR = test

# Flags
INCLUDE = -Isrc -Iextern
CFLAGS  = -Wall $(INCLUDE) -fPIC
LDFLAGS =

ifeq ($(type), RELEASE)
	CFLAGS  += -O3
else
	CFLAGS  += -ggdb -Og $(SANITIZERS)
	LDFLAGS += $(SANITIZERS)
endif

CFLAGS += -D$(type)

# Sources & Objects
SRC_FILES  := $(shell find $(SRC_DIR) -name '*.c' ! -name 'main.c')
TEST_FILES := $(shell find $(TEST_DIR) -name '*.c')
OBJ_FILES  = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Version Info
version_file   = include/version.h
VERSION_MAJOR  = $(shell sed -n -e 's/#define VERSION_MAJOR \([0-9]*\)/\1/p' $(version_file))
VERSION_MINOR  = $(shell sed -n -e 's/#define VERSION_MINOR \([0-9]*\)/\1/p' $(version_file))
VERSION_PATCH  = $(shell sed -n -e 's/#define VERSION_PATCH \([0-9]*\)/\1/p' $(version_file))
VERSION        = $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

.DEFAULT_GOAL := help

# Targets

.PHONY: all
all: $(BUILD_DIR) static shared check ## Build everything
	@echo "[INFO] Build complete."

$(BUILD_DIR): ## Create build directories
	@echo "[INFO] Creating build directories"
	mkdir -p $(BUILD_DIR)/bin $(BUILD_DIR)/movegen $(BUILD_DIR)/notation $(BUILD_DIR)/bitboard

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c ## Compile .c to .o
	@echo "[ CC ] $< -> $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: check
check: $(BUILD_DIR) static ## Build test binary
	@echo "[INFO] Building test executable: $(TEST_MOVEGEN)"
	@$(CC) $(TEST_FILES) -o $(TEST_MOVEGEN) $(INCLUDE) -L. -l:$(A_NAME) -ggdb $(SANITIZERS)

.PHONY: shared
shared: $(OBJ_FILES) ## Build shared library
	@echo "[INFO] Building shared library: $(SO_NAME)"
	@$(CC) -shared $(CFLAGS) -o $(SO_NAME) $(OBJ_FILES)

.PHONY: static
static: $(OBJ_FILES) ## Build static library
	@echo "[INFO] Building static library: $(A_NAME)"
	@$(AR) rcs $(A_NAME) $(OBJ_FILES)

.PHONY: clean
clean: ## Remove all build files
	@echo "[INFO] Cleaning build artifacts"
	rm -rf $(BUILD_DIR) $(SO_NAME) $(A_NAME)

.PHONY: compile_commands.json
compile_commands.json: $(SRC_FILES) ## Generate compile_commands.json with Bear
	@echo "[INFO] Generating compile_commands.json"
	bear -- make all

.PHONY: help
help: ## Show help message
	@echo "Available targets:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'
