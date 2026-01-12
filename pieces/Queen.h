#ifndef QUEEN_H
#define QUEEN_H

#include "../Piece.h"

class Queen : public Piece {
public:
    // Constructor
    Queen(const std::string& color);

    // Override abstract methods
    std::vector<std::vector<int>> getPossibleMoves(int row, int col) const override;
    std::string getSymbol() const override;
    std::string getType() const override;
};

#endif // QUEEN_H
