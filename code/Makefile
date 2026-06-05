# ==========================================
# Compiler and Flags
# ==========================================
CXX      := g++
CXXFLAGS := -std=c++26 -Wall -Wextra -Wpedantic -Wshadow -O3

# FIX 1: Use $(shell ...) and separate compilation flags (--cflags)
CPPFLAGS := -Iinclude -MMD -MP $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system)

LDFLAGS  := 

# FIX 2: Place the library linkers (--libs) where the linker actually reads them
LDLIBS   := $(shell pkg-config --libs sfml-graphics sfml-window sfml-system)

# ==========================================
# Paths and Files
# ==========================================
TARGET_NAME := mp
BUILD_DIR   := build
SRC_DIR     := src
TARGET      := $(BUILD_DIR)/$(TARGET_NAME)

# Automatically find all .cpp files in SRC_DIR
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
# Map source files to object files in BUILD_DIR
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
# Map object files to dependency files
DEPS := $(OBJS:.o=.d)

# ==========================================
# Build Rules
# ==========================================
.PHONY: all clean run

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo " Build successful: $(TARGET)"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Include compiler-generated dependency files (.d)
-include $(DEPS)

# ==========================================
# Utilities
# ==========================================
clean:
	@echo " Cleaning up..."
	rm -rf $(BUILD_DIR)

# FIX 3: Run the game from inside the build directory so relative paths (../../pngs/) match perfectly
run: all
	@echo " Running $(TARGET)..."
	@cd $(BUILD_DIR) && ./$(TARGET_NAME)
