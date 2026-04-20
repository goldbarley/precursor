CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -MMD -MP
LFLAGS := -lncurses
INC_FLAGS := -Iinclude
SRC_FILES := main.c $(wildcard src/*.c)
BUILD_DIR := build
TARGET := $(BUILD_DIR)/main

OPTIM_LVL ?= 0
ECHO ?= 0
CFLAGS += -O$(OPTIM_LVL) -DPRC_ECHO=$(ECHO)

OBJ_FILES := $(SRC_FILES:%.c=$(BUILD_DIR)/%.o)
DEPEND := $(OBJ_FILES:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@$(CC) $(OBJ_FILES) -o $@ $(LFLAGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

-include $(DEPEND)

run: all
	@./$(TARGET)

clean:
	@rm -rf $(BUILD_DIR) $(TARGET)
