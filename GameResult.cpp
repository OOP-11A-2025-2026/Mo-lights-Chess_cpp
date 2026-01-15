#include "GameResult.h"


GameResult::GameResult() 
{
    this->resultType = ResultType::ONGOING;
    this->reason = "Game in progress";
}

GameResult::GameResult(ResultType resultType, const std::string& reason) {
    this->resultType = resultType;
    this->reason = reason;
}

GameResult::ResultType GameResult::getResultType() const 
{
    return this->resultType;
}

std::string GameResult::getReason() const 
{
    return this->reason;
}

void GameResult::setResult(ResultType resultType, const std::string& reason) 
{
    this->resultType = resultType;
    this->reason = reason;
}

bool GameResult::isGameOver() const 
{
    return this->resultType != ResultType::ONGOING;
}

std::string GameResult::getResultMessage() const 
{
    switch(this->resultType) 
    {
        case ResultType::ONGOING:
            return "Game in progress";
        case ResultType::WHITE_WIN:
            return "White wins!";
        case ResultType::BLACK_WIN:
            return "Black wins!";
        case ResultType::DRAW:
            return "Game ends in a draw";
        case ResultType::WHITE_RESIGNED:
            return "White resigned - Black wins!";
        case ResultType::BLACK_RESIGNED:
            return "Black resigned - White wins!";
        default:
            return "Unknown result";
    }
}