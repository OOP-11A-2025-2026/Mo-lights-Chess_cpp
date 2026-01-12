#include "Piece.h"
#include <stdexcept>
#include <algorithm>

// Constructor
Piece::Piece(const std::string& color) : color(color), moved(false) {
    if (color != "white" && color != "black") {
        throw std::invalid_argument("Invalid color: " + color + ". Must be 'white' or 'black'.");
    }
}

// Getters
std::string Piece::getColor() const {
    return color;
}

bool Piece::hasMoved() const {
    return moved;
}

void Piece::setMoved(bool moved) {
    this->moved = moved;
}

// Get the type of piece (class name)
std::string Piece::getType() const {
    // This will be overridden by derived classes if needed
    // For now, we'll extract it from typeid
    std::string typeName = typeid(*this).name();
    
    // Remove numbers and decorations from typeid output
    std::string result;
    for (char c : typeName) {
        if (std::isalpha(c)) {
            result += c;
        }
    }
    
    // If the result is empty or mangled, return a generic name
    if (result.empty()) {
        return "Piece";
    }
    
    return result;
}

// Get the letter representation of the piece (used in algebraic notation)
std::string Piece::getPieceLetter() const {
    std::string type = getType();
    
    if (type.find("King") != std::string::npos) return "K";
    if (type.find("Queen") != std::string::npos) return "Q";
    if (type.find("Rook") != std::string::npos) return "R";
    if (type.find("Bishop") != std::string::npos) return "B";
    if (type.find("Knight") != std::string::npos) return "N";
    if (type.find("Pawn") != std::string::npos) return "";  // Pawns don't have a letter
    
    return "";
}

// Get abbreviated piece string (e.g., "wQ" for white queen)
std::string Piece::getPieceStr() const {
    std::string colorPrefix = (color == "white") ? "w" : "b";
    std::string letter = getPieceLetter();
    
    if (letter.empty()) {
        // For pawns
        return colorPrefix + "P";
    }
    
    return colorPrefix + letter;
}

// toString delegates to getSymbol
std::string Piece::toString() const {
    return getSymbol();
}
