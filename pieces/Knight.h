#ifndef KNIGHT_H
#define KNIGHT_H

#include "../Piece.h"

class Knight : public Piece {
public:
    // Constructor
    Knight(const std::string& color);

    // Override abstract methods
    std::vector<std::vector<int>> getPossibleMoves(int row, int col) const override;
    std::string getSymbol() const override;
    std::string getType() const override;
};

#endif // KNIGHT_H
