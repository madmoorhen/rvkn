SRC_DIR=src
INC_DIR=include
OBJ_DIR=obj
BIN_DIR=bin

PREFIX=riscv64-unknown-elf-
CC=$(PREFIX)gcc
LD=$(PREFIX)ld

CFLAGS = -Wall -Wextra -Wpedantic -Werror -Wno-unused -std=c11 -I$(INC_DIR)
CFLAGS += -Wno-format # My printf isn't cannon (extra octal O specifier)
CFLAGS += -mcmodel=medany -ffreestanding
LDFLAGS = -T $(SRC_DIR)/linker.ld -nostdlib # -lgcc

C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.S)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.S, $(OBJ_DIR)/%.o, $(ASM_SOURCES))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BIN_DIR)/kernel.elf: $(OBJECTS) | $(BIN_DIR)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR):
	mkdir -p $@
$(BIN_DIR):
	mkdir -p $@

.PHONY: clean build test

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

build: $(BIN_DIR)/kernel.elf

test: build
	# add -smp n for n harts
	qemu-system-riscv64 \
      -machine virt \
      -bios none \
      -serial mon:stdio \
      -nographic \
      -m 128M \
      -kernel $(BIN_DIR)/kernel.elf
