#include "Pawn.h"

Pawn::Pawn(const std::string& color) : Piece(color) {}

std::vector<std::vector<int>> Pawn::getPossibleMoves(int row, int col) const {
    std::vector<std::vector<int>> moves;
    
    // Determine direction based on color
    // White pawns move up the board (decreasing row), black pawns move down (increasing row)
    int direction = (color == "white") ? -1 : 1;
    
    // Forward move (one square)
    int newRow = row + direction;
    if (newRow >= 0 && newRow <= 7) {
        moves.push_back({newRow, col});
    }
    
    // Initial two-square move
    if (!moved) {
        int twoSquareRow = row + (2 * direction);
        if (twoSquareRow >= 0 && twoSquareRow <= 7) {
            moves.push_back({twoSquareRow, col});
        }
    }
    
    // Diagonal captures (left and right)
    int captureRow = row + direction;
    if (captureRow >= 0 && captureRow <= 7) {
        // Left diagonal
        if (col - 1 >= 0) {
            moves.push_back({captureRow, col - 1});
        }
        // Right diagonal
        if (col + 1 <= 7) {
            moves.push_back({captureRow, col + 1});
        }
    }
    
    return moves;
}

std::string Pawn::getSymbol() const {
    return (color == "white") ? "♙" : "♟";
}

std::string Pawn::getType() const {
    return "Pawn";
}
