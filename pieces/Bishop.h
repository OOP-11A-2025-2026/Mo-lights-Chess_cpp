#ifndef BISHOP_H
#define BISHOP_H

#include "../Piece.h"

class Bishop : public Piece {
public:
    // Constructor
    Bishop(const std::string& color);

    // Override abstract methods
    std::vector<std::vector<int>> getPossibleMoves(int row, int col) const override;
    std::string getSymbol() const override;
    std::string getType() const override;
};

#endif // BISHOP_H
