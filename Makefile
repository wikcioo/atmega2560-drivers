ifndef config
	config=debug
endif

FIRMWARE_NAME := firmware

TOOLCHAIN := avr-
CC        := $(TOOLCHAIN)gcc
OBJCPY    := $(TOOLCHAIN)objcopy
MCU       := atmega2560
CPU_SPEED := 16000000L

BUILD_DIR := build
BIN_DIR   := $(BUILD_DIR)/$(config)/bin
OBJ_DIR   := $(BUILD_DIR)/$(config)/obj

FIRMWARE_INC_DIR := firmware
FIRMWARE_SRC_DIR := firmware

WARNINGS := -Wall -Wshadow -Wpedantic
INC_DIRS := -I$(FIRMWARE_INC_DIR)

CFLAGS  := -mmcu=$(MCU) -c -std=gnu11 -DF_CPU=$(CPU_SPEED) $(WARNINGS) $(INC_DIRS)
LDFLAGS := -mmcu=$(MCU)

SOURCES := $(wildcard $(FIRMWARE_SRC_DIR)/*.c)
OBJECTS := $(addprefix $(OBJ_DIR)/, $(addsuffix .c.o, $(basename $(notdir $(SOURCES)))))

ifeq ($(config), debug)
	CFLAGS += -Og -ggdb -DDEBUG
endif
ifeq ($(config), release)
	CFLAGS += -O1 -DNDEBUG
endif

all:
	@echo "Building firmware in $(config) configuration"
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)
	@make --no-print-directory $(BIN_DIR)/$(FIRMWARE_NAME).elf
	$(OBJCPY) -O ihex -j .text -j .data $(BIN_DIR)/$(FIRMWARE_NAME).elf $(BIN_DIR)/$(FIRMWARE_NAME).hex

$(BIN_DIR)/$(FIRMWARE_NAME).elf: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.c.o: $(FIRMWARE_SRC_DIR)/%.c
	$(CC) $^ $(CFLAGS) -o $@

flash: $(BIN_DIR)/$(FIRMWARE_NAME).hex
	avrdude -C /etc/avrdude.conf -v -p m2560 -c atmelice -P usb -U flash:w:$^:i

clean:
	rm -rf $(BUILD_DIR)/$(config)
