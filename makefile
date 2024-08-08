# Define the compiler and flags
CXX := g++
CXXFLAGS := -g -w
LDFLAGS := -lGL -lGLU -lglut -lGLEW -lm -lSOIL

# Output filenames
OUTPUT := space

# Source files
SRC := space_explores.cpp

# Rule to build the program
all: $(OUTPUT)

$(OUTPUT): $(SRC)
	@echo "Compiling..."
	@$(CXX) $(CXXFLAGS) $(SRC) -o $(OUTPUT) $(LDFLAGS)
	@echo "Running..."
	@./$(OUTPUT)

# Clean up
clean:
	@echo "Cleaning up..."
	@rm -f $(OUTPUT)