#include "King.h"

King::King(const std::string& color) : Piece(color) {}

std::vector<std::vector<int>> King::getPossibleMoves(int row, int col) const {
    std::vector<std::vector<int>> moves;
    
    // All 8 adjacent squares (horizontal, vertical, and diagonal)
    // Right, left, down, up, down-right, down-left, up-right, up-left
    int directions[8][2] = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},      // Horizontal and vertical
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}     // Diagonals
    };
    
    // Check each adjacent square
    for (int i = 0; i < 8; i++) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        
        // Check if within board boundaries
        if (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7) {
            moves.push_back({newRow, newCol});
        }
    }
    
    return moves;
}

std::string King::getSymbol() const {
    return (color == "white") ? "♔" : "♚";
}

std::string King::getType() const {
    return "King";
}
