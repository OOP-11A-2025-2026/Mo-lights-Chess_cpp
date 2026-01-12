#ifndef SQUARE_H
#define SQUARE_H

#include <string>
#include <memory>

class Piece;

class Square {
private:
    int row;
    int col;
    std::shared_ptr<Piece> piece;

public:
    // Constructors
    Square(int row, int col);
    Square(int row, int col, std::shared_ptr<Piece> piece);

    // Getters
    int getRow() const;
    int getCol() const;
    std::shared_ptr<Piece> getPiece() const;

    // Piece management
    void setPiece(std::shared_ptr<Piece> piece);
    void removePiece();
    bool hasPiece() const;

    // Square information
    std::string getAlgebraicNotation() const;
    bool isLightSquare() const;

    // Display methods
    std::string toString() const;
    std::string toStringDetailed() const;
};

#endif // SQUARE_H
