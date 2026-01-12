#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <vector>

class Piece {
protected:
    std::string color;
    bool moved;

public:
    // Constructor
    Piece(const std::string& color);

    // Virtual destructor for proper cleanup of derived classes
    virtual ~Piece() = default;

    // Pure virtual methods (must be implemented by derived classes)
    virtual std::vector<std::vector<int>> getPossibleMoves(int row, int col) const = 0;
    virtual std::string getSymbol() const = 0;

    // Concrete methods
    std::string getColor() const;
    bool hasMoved() const;
    void setMoved(bool moved);
    virtual std::string getType() const;
    std::string getPieceLetter() const;
    std::string getPieceStr() const;
    std::string toString() const;
};

#endif // PIECE_H
