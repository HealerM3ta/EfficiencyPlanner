# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -std=c++11 -Wall -Wextra

# Source files
SRCS := PP3Test.cpp HuffmanBase.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable name
TARGET := Project3

# Build target
all: $(TARGET)

$(TARGET): $(OBJS)
        $(CXX) $(CXXFLAGS) -o $@ $^

# Rule for compiling source files to object files
%.o: %.cpp
        $(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean rule
clean:
        rm -f $(OBJS) $(TARGET)