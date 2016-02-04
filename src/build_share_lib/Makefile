# default compiler
CC := gcc

# folders
SOURCE_DIR := src
BUILD_DIR := build
BIN_DIR := bin
LIB_DIR := lib
INCLUDE_DIR := include

# srcs, objs
SOURCES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS := $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# target
TARGET := $(LIB_DIR)/libdemo.so

# compile flags
CFLAGS := -Wall -pthread -Werror -fPIC
INC := -Iinclude

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(LIB_DIR)
	$(CC) -g -shared -o $@ $^

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR) $(LIB_DIR)

.PHONY: clean all
