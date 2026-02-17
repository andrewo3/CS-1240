# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -Wall -Wextra -std=c++17 -O2

# Target executable name
TARGET := randmst

# Automatically find all .cpp files
SRC := $(wildcard *.cpp)

# Object files
OBJ := $(SRC:.cpp=.o)

# Default rule
all: $(TARGET)

# Link step
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile step
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(TARGET) $(OBJ)

# Phony targets
.PHONY: all clean
