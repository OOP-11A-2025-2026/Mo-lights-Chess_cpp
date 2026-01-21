#ifndef CHESSEXCEPTION_H
#define CHESSEXCEPTION_H

#include <exception>
#include <string>

// Base exception class for all chess-related exceptions
class ChessException : public std::exception {
protected:
    std::string message;

public:
    explicit ChessException(const std::string& msg) : message(msg) {}
    
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

// Exception for invalid moves
class InvalidMoveException : public ChessException {
public:
    explicit InvalidMoveException(const std::string& msg) 
        : ChessException("Invalid Move: " + msg) {}
};

// Exception for invalid square references
class InvalidSquareException : public ChessException {
public:
    explicit InvalidSquareException(const std::string& msg) 
        : ChessException("Invalid Square: " + msg) {}
    
    InvalidSquareException(int row, int col) 
        : ChessException("Invalid Square: row=" + std::to_string(row) + 
                        ", col=" + std::to_string(col)) {}
};

// Exception for invalid game state operations
class GameStateException : public ChessException {
public:
    explicit GameStateException(const std::string& msg) 
        : ChessException("Game State Error: " + msg) {}
};

// Exception for invalid color specifications
class InvalidColorException : public ChessException {
public:
    explicit InvalidColorException(const std::string& color) 
        : ChessException("Invalid Color: '" + color + "'. Must be 'white' or 'black'") {}
};

// Exception for file I/O errors
class ChessFileException : public ChessException {
public:
    explicit ChessFileException(const std::string& msg) 
        : ChessException("Chess File Error: " + msg) {}
    
    ChessFileException(const std::string& filename, const std::string& operation, const std::string& reason)
        : ChessException("Chess File Error: Failed to " + operation + " file '" + 
                        filename + "': " + reason) {}
};

// Exception for PGN parsing errors
class PGNParseException : public ChessException {
public:
    explicit PGNParseException(const std::string& msg) 
        : ChessException("PGN Parse Error: " + msg) {}
    
    PGNParseException(const std::string& move, const std::string& reason)
        : ChessException("PGN Parse Error: Failed to parse move '" + move + 
                        "': " + reason) {}
};

// Exception for wrong turn violations
class WrongTurnException : public ChessException {
public:
    explicit WrongTurnException(const std::string& attemptedColor, const std::string& currentTurn)
        : ChessException("Wrong Turn: It is " + currentTurn + "'s turn, but " + 
                        attemptedColor + " tried to move") {}
};

#endif // CHESSEXCEPTION_H
