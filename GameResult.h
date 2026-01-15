#ifndef GAMERESULT_H
#define GAMERESULT_H
#include <string>
class GameResult
{
    public:
    enum ResultType {
        ONGOING,      // Game is still in progress
        WHITE_WIN,    // White won (by checkmate or resignation)
        BLACK_WIN,    // Black won (by checkmate or resignation)
        DRAW,         // Game ended in a draw (agreed or stalemate)
        WHITE_RESIGNED,  // White resigned
        BLACK_RESIGNED   // Black resigned
    };
    GameResult();
    GameResult(ResultType resultType, const std::string& reason);
    ResultType getResultType() const;
    std::string getReason() const;
    void setResult(ResultType resultType, const std::string& reason);
    bool isGameOver() const;
    std::string getResultMessage() const;

    private:

    ResultType resultType;
    std::string reason;

};


#endif