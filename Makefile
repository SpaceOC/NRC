CC = clang++

SRC_DIR = source
BUILD_DIR = build

CORE_DIR = $(SRC_DIR)/Core
OS_DIR = $(SRC_DIR)/OS
FLAGS = -std=c++17 -c -static -Wall

OS_NAME = YourOSName

.PHONY: all Core OS clean

all: Core OS
	$(CC) -std=c++17 -static -Wall $(SRC_DIR)/main.cpp $(BUILD_DIR)/Core.o $(BUILD_DIR)/OS.o -o $(BUILD_DIR)/Result/$(OS_NAME).exe

Core:
	$(CC) $(FLAGS) $(SRC_DIR)/Core/core.cpp -o $(BUILD_DIR)/Core.o

OS:
	$(CC) $(FLAGS) $(OS_DIR)/os_main.cpp -o $(BUILD_DIR)/OS.o

	




