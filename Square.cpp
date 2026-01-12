#include "Square.h"
#include "Piece.h"
#include <stdexcept>
#include <sstream>

// Constructor with position only
Square::Square(int row, int col) : row(row), col(col), piece(nullptr) {
    if (row < 0 || row > 7 || col < 0 || col > 7) {
        throw std::invalid_argument("Invalid square coordinates: row and col must be between 0 and 7");
    }
}

// Constructor with position and piece
Square::Square(int row, int col, std::shared_ptr<Piece> piece) : row(row), col(col), piece(piece) {
    if (row < 0 || row > 7 || col < 0 || col > 7) {
        throw std::invalid_argument("Invalid square coordinates: row and col must be between 0 and 7");
    }
}

// Getters
int Square::getRow() const {
    return row;
}

int Square::getCol() const {
    return col;
}

std::shared_ptr<Piece> Square::getPiece() const {
    return piece;
}

// Piece management
void Square::setPiece(std::shared_ptr<Piece> piece) {
    this->piece = piece;
}

void Square::removePiece() {
    piece = nullptr;
}

bool Square::hasPiece() const {
    return piece != nullptr;
}

// Get algebraic notation (e.g., "e4")
std::string Square::getAlgebraicNotation() const {
    char file = 'a' + col;
    int rank = 8 - row;
    return std::string(1, file) + std::to_string(rank);
}

// Determine if square is light or dark
bool Square::isLightSquare() const {
    return (row + col) % 2 != 0;
}

// Display methods
std::string Square::toString() const {
    if (piece) {
        std::string symbol = piece->getSymbol();
        std::string color = piece->getColor();
        
        // ANSI color codes
        std::string colorCode = (color == "white") ? "\033[97m" : "\033[30m";
        std::string bgColor = isLightSquare() ? "\033[48;5;222m" : "\033[48;5;94m";
        std::string reset = "\033[0m";
        
        return bgColor + colorCode + " " + symbol + " " + reset;
    } else {
        std::string bgColor = isLightSquare() ? "\033[48;5;222m" : "\033[48;5;94m";
        std::string reset = "\033[0m";
        return bgColor + "   " + reset;
    }
}

std::string Square::toStringDetailed() const {
    std::ostringstream oss;
    oss << "Square " << getAlgebraicNotation();
    if (piece) {
        oss << " contains " << piece->getColor() << " " << piece->getType();
    } else {
        oss << " is empty";
    }
    return oss.str();
}
