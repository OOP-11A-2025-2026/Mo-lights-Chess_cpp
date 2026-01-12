#ifndef PAWN_H
#define PAWN_H

#include "../Piece.h"

class Pawn : public Piece {
public:
    // Constructor
    Pawn(const std::string& color);

    // Override abstract methods
    std::vector<std::vector<int>> getPossibleMoves(int row, int col) const override;
    std::string getSymbol() const override;
    std::string getType() const override;
};

#endif // PAWN_H
