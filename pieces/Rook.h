#ifndef ROOK_H
#define ROOK_H

#include "../Piece.h"

class Rook : public Piece {
public:
    // Constructor
    Rook(const std::string& color);

    // Override abstract methods
    std::vector<std::vector<int>> getPossibleMoves(int row, int col) const override;
    std::string getSymbol() const override;
    std::string getType() const override;
};

#endif // ROOK_H
