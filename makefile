# ================================
# Multi-Threaded Proxy Server
# ================================

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -lcurl

# Source files
SRC = $(wildcard src/*.cpp) main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = proxy_server

.PHONY: all clean run

# Default build
all: $(TARGET)

# Link object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the server on port 8080
run: $(TARGET)
	./$(TARGET) 8080

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)