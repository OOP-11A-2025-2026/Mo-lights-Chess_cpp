#ifndef CHESSENGINE_H
#define CHESSENGINE_H
#include "Board.h"
#include "Move.h"
#include "GameResult.h"
#include <vector>

class ChessEngine
{
    std::shared_ptr<Board> board;
    std::string currentTurn;
    std::vector<Move> moveLog;
    std::string drawRequestedBy;
    std::shared_ptr<GameResult> gameResult;

    public:
    ChessEngine();
    std::shared_ptr<Board> getBoard() const;
    std::vector<Move> getMoveLog() const;
    std::string getCurrentTurn() const;
    std::shared_ptr<GameResult> getGameResult() const;
    std::string getDrawRequestedBy() const;
    void clearDrawRequest();
    void setCurrentTurn(const std::string& currentTurn);
    void makeMove(Move& move);
    void undoMove();
    std::shared_ptr<Square> findKing(const std::string& color) const;
    bool isSquareUnderAttack(int row, int col, const std::string& byColor);
    bool isInCheck(const std::string& color);
    std::vector<Move> getAllLegalMoves();
    bool isCheckmate();
    bool isStalemate();
    std::vector<Move> getAllPossibleMoves();
    void getPawnMoves(const std::shared_ptr<Square> startSquare, std::vector<Move>& possibleMoves);
    void getRookMoves(const std::shared_ptr<Square> startSquare, std::vector<Move>& possibleMoves);
    void getBishopMoves(const std::shared_ptr<Square> startSquare, std::vector<Move>& possibleMoves);
    void getQueenMoves(const std::shared_ptr<Square> startSquare, std::vector<Move>& possibleMoves);
    void getKnightMoves(const std::shared_ptr<Square> startSquare, std::vector<Move>& possibleMoves);
    void getKingMoves(const std::shared_ptr<Square> startSquare, std::vector<Move>& possibleMoves);
    void resign();
    bool requestDraw();
    void acceptDraw();
    void declineDraw();

    private:
    void makeMoveTesting(Move move);
    void undoMoveTesting();
};




#endif