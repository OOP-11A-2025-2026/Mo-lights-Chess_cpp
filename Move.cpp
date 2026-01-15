#include "Move.h"
#include <iostream>
#include <stdexcept>
using namespace std;



Move::Move(shared_ptr<Square> startSquare, shared_ptr<Square> endSquare)
    {   
        if(startSquare->getPiece() == nullptr)
        {
            throw invalid_argument("You can't move from an empty square");
        }
        if(startSquare->getAlgebraicNotation() == endSquare->getAlgebraicNotation())
        {
            throw invalid_argument("End square must be different from the start square");
        }
        this->startSquare = startSquare;
        this->endSquare = endSquare;
        this->pieceMoved = startSquare->getPiece();
        this->pieceCaptured = endSquare->getPiece();
        this->isEnPassantMove = false;
        this->isEnPassantMove = false;
        this->isPawnPromotionMove = false;
        this->isKingSideCastleMove = false;
        this->isQueenSideCastleMove = false;
        this->hadPieceBeenMoved = this->pieceMoved->hasMoved();
        this->pawnPromotionPiece = nullptr;
        this->enPassantCapturingSquare = nullptr;
    }

Move::Move(const Move& other) 
    {
    this->startSquare = other.startSquare;
    this->endSquare = other.endSquare;
    this->pieceMoved = other.pieceMoved;
    this->pieceCaptured = other.pieceCaptured;
    this->isEnPassantMove = other.isEnPassantMove;
    this->isPawnPromotionMove = other.isPawnPromotionMove;
    this->pawnPromotionPiece = other.pawnPromotionPiece;
    this->isKingSideCastleMove = other.isKingSideCastleMove;
    this->isQueenSideCastleMove = other.isQueenSideCastleMove;
    this->hadPieceBeenMoved = other.hadPieceBeenMoved;
    this->enPassantCapturingSquare = other.enPassantCapturingSquare;
    }

shared_ptr<Square> Move::getStartSquare() const 
{ 
    return this->startSquare; 
}

shared_ptr<Square> Move::getEndSquare() const 
{
    return this->endSquare; 
}

shared_ptr<Piece> Move::getPieceMoved() const 
{
    return this->pieceMoved;
}

shared_ptr<Piece> Move::getPieceCaptured() const 
{
    return this->pieceCaptured; 
}

bool Move::getIsEnpassantMove() const 
{
    return this->isEnPassantMove; 
}

shared_ptr<Square> Move::getEnPassantCapturingSquare() const 
{
    return this->enPassantCapturingSquare; 
}

bool Move::getIsPawnPromotionMove() const 
{ 
    return this->isPawnPromotionMove; 
}

shared_ptr<Piece> Move::getPawnPromotionPiece() const 
{ 
    return this->pawnPromotionPiece; 
}

bool Move::getIsKingSideCastle() const 
{ 
    return this->isKingSideCastleMove; 
}

bool Move::getIsQueenSideCastle() const 
{ 
    return this->isQueenSideCastleMove; 
}

bool Move::getHadPieceBeenMoved() const 
{ 
    return this->hadPieceBeenMoved; 
}

// --- Setters ---

void Move::setEnpassant() 
{ 
    this->isEnPassantMove = true; 
}

void Move::setEnPassantCapturingSquare(shared_ptr<Square> square) 
{ 
    this->enPassantCapturingSquare = square; 
}

void Move::setPawnPromotion() 
{ 
    this->isPawnPromotionMove = true; 
}

void Move::setPawnPromotionPiece(shared_ptr<Piece> piece) 
{ 
    this->pawnPromotionPiece = piece; 
}

void Move::setPieceCaptured(const shared_ptr<Piece>& piece) 
{ 
    this->pieceCaptured = piece; 
}

void Move::setPieceMoved(shared_ptr<Piece>& piece)
{
    this->pieceMoved = piece;
}

void Move::setHadPieceBeenMoved(bool val) 
{ 
    this->hadPieceBeenMoved = val; 
}

void Move::setKingSideCastle() 
{ 
    this->isKingSideCastleMove = true; 
}

void Move::setQueenSideCastle() 
{ 
    this->isQueenSideCastleMove = true; 
}

bool Move::areTwoMovesEqual(const Move& other) const 
{
    return (this->startSquare == other.startSquare && 
            this->endSquare == other.endSquare && 
            this->pieceMoved == other.pieceMoved);
}

string Move::toString() const 
{
    if(this->isKingSideCastleMove) 
        return "0-0";
    if(this->isQueenSideCastleMove) 
        return "0-0-0";

    string result = "";
    string pieceLetter = this->pieceMoved->getPieceLetter();

    if(pieceLetter.empty()) 
    {
        if(this->pieceCaptured != nullptr || this->isEnPassantMove) 
        {
            result += this->startSquare->getAlgebraicNotation().substr(0, 1);
        }
    } 
    else 
    {
        result += pieceLetter;
    }

    if(this->pieceCaptured != nullptr || this->isEnPassantMove) 
    {
        result += "x";
    }

    result += this->endSquare->getAlgebraicNotation();

    if(this->isPawnPromotionMove && this->pawnPromotionPiece != nullptr) 
    {
        result += "=" + this->pawnPromotionPiece->getPieceLetter();
    }

    return result;
}