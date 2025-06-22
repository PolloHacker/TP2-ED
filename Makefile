# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -g -std=c++11 -I./include

# Folders
BIN_FOLDER := ./bin
OBJ_FOLDER := ./obj
SRC_FOLDER := ./src
INPUT_FOLDER := ./inputs
RESULTS_FOLDER := ./results

# Source and object files
EXP_FILE := $(SRC_FOLDER)/genwkl.c
SRCS := $(filter-out $(EXP_FILE), $(wildcard $(SRC_FOLDER)/*.cpp))
OBJS := $(patsubst $(SRC_FOLDER)/%.cpp, $(OBJ_FOLDER)/%.o, $(SRCS))

# Output binary
TARGET := $(BIN_FOLDER)/tp2.out

# Default target
all: $(TARGET)

exp: $(BIN_FOLDER)
	$(CXX) $(CXXFLAGS) $(EXP_FILE) -o $(BIN_FOLDER)/genwkl.out -lm
		

print: 
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "BIN_FOLDER: $(BIN_FOLDER)"
	@echo "OBJ_FOLDER: $(OBJ_FOLDER)"
	@echo "SRC_FOLDER: $(SRC_FOLDER)"
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"
	@echo "EXP_OBJS: $(EXP_OBJS)"
	@echo "TARGET: $(TARGET)"
	@echo "---------------------"

$(TARGET): $(OBJS) | $(BIN_FOLDER)
	$(CXX) $(OBJS) -o $@

$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.cpp | $(OBJ_FOLDER)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_FOLDER):
	mkdir -p $(OBJ_FOLDER)

$(BIN_FOLDER):
	mkdir -p $(BIN_FOLDER)

cleanExp:
	rm -rf $(INPUT_FOLDER) $(RESULTS_FOLDER)

clean:
	rm -rf $(BIN_FOLDER) $(OBJ_FOLDER)

.PHONY: all clean