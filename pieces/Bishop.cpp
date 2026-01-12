#include "Bishop.h"

Bishop::Bishop(const std::string& color) : Piece(color) {}

std::vector<std::vector<int>> Bishop::getPossibleMoves(int row, int col) const {
    std::vector<std::vector<int>> moves;
    
    // Diagonal directions: down-right, down-left, up-right, up-left
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    
    // For each diagonal direction
    for (int d = 0; d < 4; d++) {
        int dRow = directions[d][0];
        int dCol = directions[d][1];
        
        // Move up to 7 squares in this direction (or until board edge)
        for (int i = 1; i <= 7; i++) {
            int newRow = row + (i * dRow);
            int newCol = col + (i * dCol);
            
            // Check if within board boundaries
            if (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7) {
                moves.push_back({newRow, newCol});
            } else {
                break;  // Out of bounds, stop in this direction
            }
        }
    }
    
    return moves;
}

std::string Bishop::getSymbol() const {
    return (color == "white") ? "♗" : "♝";
}

std::string Bishop::getType() const {
    return "Bishop";
}
