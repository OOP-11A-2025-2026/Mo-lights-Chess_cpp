#ifndef MOVE_H
#define MOVE_H

#include"Piece.h"
#include"Square.h"

class Move
{
    private:
    std::shared_ptr<Square> startSquare;
    std::shared_ptr<Square> endSquare;
    std::shared_ptr<Piece> pieceMoved;
    std::shared_ptr<Piece> pieceCaptured;
    
    //has pieceMoved been moved before this move
    bool hadPieceBeenMoved;

    //en passant 
    bool isEnPassantMove;
    std::shared_ptr<Square> enPassantCapturingSquare;

    //pawn promotion
    bool isPawnPromotionMove;
    std::shared_ptr<Piece> pawnPromotionPiece;

    //castle
    bool isKingSideCastleMove, isQueenSideCastleMove;

    public:
    //Constructor
    Move(std::shared_ptr<Square> startSquare, std::shared_ptr<Square> endSquare);

    Move(const Move& other);

    // getters 
    std::shared_ptr<Square> getStartSquare() const;
    std::shared_ptr<Square> getEndSquare() const;
    bool getIsEnpassantMove() const;
    bool getIsPawnPromotionMove() const;
    std::shared_ptr<Piece> getPawnPromotionPiece() const;
    std::shared_ptr<Piece> getPieceMoved() const;
    std::shared_ptr<Piece> getPieceCaptured() const;
    bool getIsKingSideCastle() const;
    bool getIsQueenSideCastle() const;
    bool getHadPieceBeenMoved() const;
    std::shared_ptr<Square> getEnPassantCapturingSquare() const;

    //setters
    void setEnpassant();
    void setPawnPromotion();
    void setPawnPromotionPiece(std::shared_ptr<Piece> piece);
    void setPieceMoved(std::shared_ptr<Piece>& piece);
    void setPieceCaptured(const std::shared_ptr<Piece>& piece);
    void setHadPieceBeenMoved(bool hadPieceBeenMoved);
    void setEnPassantCapturingSquare(std::shared_ptr<Square> square);
    void setKingSideCastle();
    void setQueenSideCastle();
    //compering 2 moves object with the same atributes
    bool areTwoMovesEqual(const Move& other) const;
    // Display methods
    std::string toString() const; 

};


#endif