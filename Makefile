CXX      := g++
CXXFLAGS := -Wall -Wextra -Werror -pthread
LDFLAGS  := -lstdc++ -lm -pthread
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/
TARGET   := app
INCLUDE  := -Iinclude/
SRC      := $(wildcard src/*.cpp)

# Objeto relativo ao caminho src
OBJECTS := $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $@ $(OBJECTS)

.PHONY: all build clean debug release run valgrind

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O3
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

run:
	./$(APP_DIR)/$(TARGET)

valgrind: debug
	valgrind --leak-check=full ./$(APP_DIR)/$(TARGET)
