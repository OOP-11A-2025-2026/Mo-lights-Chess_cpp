#ifndef KING_H
#define KING_H

#include "../Piece.h"

class King : public Piece {
public:
    // Constructor
    King(const std::string& color);

    // Override abstract methods
    std::vector<std::vector<int>> getPossibleMoves(int row, int col) const override;
    std::string getSymbol() const override;
    std::string getType() const override;
};

#endif // KING_H
