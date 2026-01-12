#include "Knight.h"

Knight::Knight(const std::string& color) : Piece(color) {}

std::vector<std::vector<int>> Knight::getPossibleMoves(int row, int col) const {
    std::vector<std::vector<int>> moves;
    
    // All possible L-shaped knight moves
    int knightMoves[8][2] = {
        {-2, -1}, {-2, 1},  // Two up, one left/right
        {-1, -2}, {-1, 2},  // One up, two left/right
        {1, -2},  {1, 2},   // One down, two left/right
        {2, -1},  {2, 1}    // Two down, one left/right
    };
    
    // Check each possible knight move
    for (int i = 0; i < 8; i++) {
        int newRow = row + knightMoves[i][0];
        int newCol = col + knightMoves[i][1];
        
        // Check if within board boundaries
        if (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7) {
            moves.push_back({newRow, newCol});
        }
    }
    
    return moves;
}

std::string Knight::getSymbol() const {
    return (color == "white") ? "♘" : "♞";
}

std::string Knight::getType() const {
    return "Knight";
}
