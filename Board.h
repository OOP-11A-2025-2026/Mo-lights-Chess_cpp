#ifndef BOARD_H
#define BOARD_H
#include "Piece.h"
#include "Square.h"
#include "pieces/Pawn.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"
#include <stdexcept>
class Board
{
    std::shared_ptr<Square> squares[8][8];

    public:
    Board();
    void initEmptyBoard(); 
    void setStartingPosition();
    std::shared_ptr<Square> getSquare(int row, int col);
    std::shared_ptr<Square> getSquare(const std::string& position);
    void printBoard() const;
};



#endif