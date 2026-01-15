#include "Board.h"
#include <iostream>
using namespace std;
Board::Board()
{
    this->initEmptyBoard();
    this->setStartingPosition();
}

void Board::initEmptyBoard()
{
    for(int row = 0; row < 8; row++) 
    {
        for(int col = 0; col < 8; col++) 
        {
            this->squares[row][col] = make_shared<Square>(row, col);
        }
    }
}

void Board::setStartingPosition()
{
    for(int col = 0; col < 8; col++) 
    {
        squares[6][col]->setPiece(make_shared<Pawn>("white"));
        squares[1][col]->setPiece(make_shared<Pawn>("black"));

        if(col == 0 || col == 7) 
        {
            squares[7][col]->setPiece(make_shared<Rook>("white"));
            squares[0][col]->setPiece(make_shared<Rook>("black"));
        }
        else if(col == 1 || col == 6) 
        {
            squares[7][col]->setPiece(make_shared<Knight>("white"));
            squares[0][col]->setPiece(make_shared<Knight>("black"));
        }
        else if(col == 2 || col == 5) 
        {
            squares[7][col]->setPiece(make_shared<Bishop>("white"));
            squares[0][col]->setPiece(make_shared<Bishop>("black"));
        }
        else if(col == 3) 
        {
            squares[7][col]->setPiece(make_shared<Queen>("white"));
            squares[0][col]->setPiece(make_shared<Queen>("black"));
        }
        else if(col == 4) 
        {
            squares[7][col]->setPiece(make_shared<King>("white"));
            squares[0][col]->setPiece(make_shared<King>("black"));
        }
    }
}

shared_ptr<Square> Board::getSquare(int row, int col) 
{
    if(row < 0 || row > 7 || col < 0 || col > 7) 
    {
        throw out_of_range("Invalid board coordinates");
    }
    return squares[row][col];
}

shared_ptr<Square> Board::getSquare(const string& position) {
    if(position.length() != 2) throw invalid_argument("Invalid notation");
    
    string files = "abcdefgh";
    string ranks = "87654321";
    
    int col = files.find(position[0]);
    int row = ranks.find(position[1]);
    
    if(col == string::npos || row == string::npos) 
    {
        throw invalid_argument("Square not found");
    }
    
    return squares[row][col];
}

void Board::printBoard() const 
{
    const string whitePieceFg = "\033[97m";
    const string blackPieceFg = "\033[90m";
    const string reset = "\033[0m";

    cout << "  ┌────────────────┐" << endl;

    for(int row = 0; row < 8; row++) 
    {
        cout << (8 - row) << " │";
        for(int col = 0; col < 8; col++) 
        {
            shared_ptr<Square> square = squares[row][col];
            if(square->hasPiece()) 
            {
                auto piece = square->getPiece();
                string color = (piece->getColor() == "white") ? whitePieceFg : blackPieceFg;
                cout << color << piece->getSymbol() << " " << reset;
            } 
            else 
            {
                cout << "· ";
            }
        }
        cout << "│" << endl;
    }

    cout << "  └────────────────┘" << endl;
    cout << "    a b c d e f g h" << endl;
}



