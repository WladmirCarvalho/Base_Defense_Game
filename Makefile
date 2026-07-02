CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Detecta o sistema operacional para ajustar paths do SFML e comandos de shell
ifeq ($(OS),Windows_NT)
    EXEC = base_defense.exe
    MKDIR_CMD = mkdir $(OBJ_DIR)
    CLEAN_CMD = rmdir /S /Q $(OBJ_DIR) & del $(EXEC)
    RUN_CMD = $(EXEC)
else
    EXEC = base_defense
    MKDIR_CMD = mkdir -p $(OBJ_DIR)
    CLEAN_CMD = rm -rf $(OBJ_DIR) $(EXEC)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        SFML_PREFIX := $(shell brew --prefix sfml)
        CXXFLAGS += -I$(SFML_PREFIX)/include
        LDFLAGS += -L$(SFML_PREFIX)/lib
        RUN_CMD = DYLD_LIBRARY_PATH=$(SFML_PREFIX)/lib ./$(EXEC)
    else
        RUN_CMD = ./$(EXEC)
    endif
endif

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	$(MKDIR_CMD)

run: $(EXEC)
	$(RUN_CMD)

clean:
	$(CLEAN_CMD)

.PHONY: all clean run