# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 $(shell python3.11-config --cflags)
LDFLAGS = $(shell python3.11-config --ldflags) -lpython3.11

# Directories
SRC_DIR = src
BUILD_DIR = build

# Find all source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Convert source files to object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Output executable
TARGET = prog

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Rule to create object files from source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean
