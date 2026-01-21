# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS =

# Directories
PIECES_DIR = pieces
EXCEPTIONS_DIR = exceptions

# Source files
SOURCES = main.cpp \
          PGNReader.cpp \
          PGNWriter.cpp \
          AlgebraicNotationParser.cpp \
          ChessEngine.cpp \
          Board.cpp \
          Move.cpp \
          Square.cpp \
          Piece.cpp \
          GameResult.cpp \
          $(PIECES_DIR)/Pawn.cpp \
          $(PIECES_DIR)/Rook.cpp \
          $(PIECES_DIR)/Knight.cpp \
          $(PIECES_DIR)/Bishop.cpp \
          $(PIECES_DIR)/Queen.cpp \
          $(PIECES_DIR)/King.cpp

# Test sources
TEST_CHESS_SOURCES = test_chess.cpp \
                     ChessEngine.cpp \
                     Board.cpp \
                     Move.cpp \
                     Square.cpp \
                     Piece.cpp \
                     GameResult.cpp \
                     $(PIECES_DIR)/Pawn.cpp \
                     $(PIECES_DIR)/Rook.cpp \
                     $(PIECES_DIR)/Knight.cpp \
                     $(PIECES_DIR)/Bishop.cpp \
                     $(PIECES_DIR)/Queen.cpp \
                     $(PIECES_DIR)/King.cpp

TEST_PGN_SOURCES = test_pgn.cpp \
                   PGNReader.cpp \
                   PGNWriter.cpp \
                   AlgebraicNotationParser.cpp \
                   ChessEngine.cpp \
                   Board.cpp \
                   Move.cpp \
                   Square.cpp \
                   Piece.cpp \
                   GameResult.cpp \
                   $(PIECES_DIR)/Pawn.cpp \
                   $(PIECES_DIR)/Rook.cpp \
                   $(PIECES_DIR)/Knight.cpp \
                   $(PIECES_DIR)/Bishop.cpp \
                   $(PIECES_DIR)/Queen.cpp \
                   $(PIECES_DIR)/King.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
TEST_CHESS_OBJECTS = $(TEST_CHESS_SOURCES:.cpp=.o)
TEST_PGN_OBJECTS = $(TEST_PGN_SOURCES:.cpp=.o)

# Executable names
TARGET = chess
TEST_CHESS_TARGET = test_chess
TEST_PGN_TARGET = test_pgn

# Default target
all: $(TARGET)

# Build main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Built $(TARGET) successfully!"

# Build test executables
test: $(TEST_CHESS_TARGET)

$(TEST_CHESS_TARGET): $(TEST_CHESS_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Built $(TEST_CHESS_TARGET) successfully!"

$(TEST_PGN_TARGET): $(TEST_PGN_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Built $(TEST_PGN_TARGET) successfully!"

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the chess game
run: $(TARGET)
	./$(TARGET)

# Run tests
run-test: $(TEST_CHESS_TARGET)
	./$(TEST_CHESS_TARGET)

run-test-pgn: $(TEST_PGN_TARGET)
	./$(TEST_PGN_TARGET)

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TEST_CHESS_OBJECTS) $(TEST_PGN_OBJECTS) $(TARGET) $(TEST_CHESS_TARGET) $(TEST_PGN_TARGET)
	rm -f $(PIECES_DIR)/*.o
	@echo "Cleaned build artifacts"

# Clean and rebuild
rebuild: clean all

# Install (optional - copies to /usr/local/bin)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete! Run 'chess' from anywhere."

# Uninstall
uninstall:
	@echo "Removing $(TARGET) from /usr/local/bin..."
	@sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstallation complete."

# Help target
help:
	@echo "Mo-Lights Chess C++ - Makefile Help"
	@echo "===================================="
	@echo "Available targets:"
	@echo "  make          - Build the chess executable"
	@echo "  make test     - Build the test executable"
	@echo "  make run      - Build and run the chess game"
	@echo "  make run-test - Build and run tests"
	@echo "  make clean    - Remove all build artifacts"
	@echo "  make rebuild  - Clean and rebuild everything"
	@echo "  make install  - Install chess to /usr/local/bin"
	@echo "  make uninstall- Remove chess from /usr/local/bin"
	@echo "  make help     - Show this help message"

# Phony targets
.PHONY: all test run run-test clean rebuild install uninstall help
