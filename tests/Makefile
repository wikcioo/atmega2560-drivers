ifndef dist
	dist=arch
endif

OUTPUT_FILENAME := gtests

CC  := gcc
CXX := g++

# Directory variables
BUILD_DIR := build
BIN_DIR   := $(BUILD_DIR)/bin
OBJ_DIR   := $(BUILD_DIR)/obj

DRIVERS_SRC_DIR := ../drivers/src
TESTS_SRC_DIR   := src
VENDOR_DIR      := vendor

ifeq ($(dist), arch)
	AVR_DIR := /usr/avr/include
endif
ifeq ($(dist), debian)
	AVR_DIR := /usr/lib/avr/include
endif

# ======== C variables ========
C_INCLUDES := -include vendor/forced_include.h -idirafter $(AVR_DIR)
C_FLAGS    := -c -std=gnu11 $(C_INCLUDES)
C_FILES    := led_array.c
C_SOURCES  := $(addprefix $(DRIVERS_SRC_DIR)/, $(C_FILES))
C_SOURCES  += $(wildcard $(VENDOR_DIR)/*.c)
OBJECTS    := $(addprefix $(OBJ_DIR)/, $(addsuffix .c.o, $(basename $(notdir $(C_SOURCES)))))

# ======== C++ variables ========
CXX_INCLUDES := -include vendor/forced_include.h -I$(DRIVERS_SRC_DIR) -idirafter $(AVR_DIR)
CXX_FLAGS    := -c -std=c++17 $(CXX_INCLUDES)
CXX_LD_FLAGS := -lgtest
CPP_SOURCES  += $(wildcard $(TESTS_SRC_DIR)/*.cpp)
OBJECTS      += $(addprefix $(OBJ_DIR)/, $(addsuffix .cpp.o, $(basename $(notdir $(CPP_SOURCES)))))

.PHONY = all clean

all:
	@echo "Building tests"
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)
	@make --no-print-directory $(BIN_DIR)/$(OUTPUT_FILENAME)

$(BIN_DIR)/$(OUTPUT_FILENAME): $(OBJECTS)
	$(CXX) $^ $(CXX_LD_FLAGS) -o $@

$(OBJ_DIR)/%.c.o: $(DRIVERS_SRC_DIR)/%.c
	$(CC) $^ $(C_FLAGS) -o $@

$(OBJ_DIR)/%.c.o: $(VENDOR_DIR)/%.c
	$(CC) $^ $(C_FLAGS) -o $@

$(OBJ_DIR)/%.cpp.o: $(TESTS_SRC_DIR)/%.cpp
	$(CXX) $^ $(CXX_FLAGS) -o $@

clean:
	rm -rf $(BUILD_DIR)
