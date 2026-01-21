#include "PGNWriter.h"
#include "AlgebraicNotationParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <ctime>

PGNWriter::PGNWriter(std::shared_ptr<ChessEngine> engine) 
    : engine(engine) {}

void PGNWriter::writePGN(const std::string& filePath,
                         const std::string& event,
                         const std::string& site,
                         const std::string& whitePlayer,
                         const std::string& blackPlayer,
                         const std::string& result) {
    std::ostringstream pgnContent;

    // Get current date
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    
    char dateBuffer[11];
    std::strftime(dateBuffer, sizeof(dateBuffer), "%Y.%m.%d", localTime);

    // Write PGN headers
    pgnContent << "[Event \"" << event << "\"]\n";
    pgnContent << "[Site \"" << site << "\"]\n";
    pgnContent << "[Date \"" << dateBuffer << "\"]\n";
    pgnContent << "[Round \"?\"]\n";
    pgnContent << "[White \"" << whitePlayer << "\"]\n";
    pgnContent << "[Black \"" << blackPlayer << "\"]\n";
    pgnContent << "[Result \"" << result << "\"]\n\n";

    // Get move log
    std::vector<Move> moveLog = engine->getMoveLog();

    // Create a temporary engine to replay moves and generate proper notation
    auto tempEngine = std::make_shared<ChessEngine>();
    auto parser = std::make_shared<AlgebraicNotationParser>(tempEngine);

    std::ostringstream movesContent;
    int moveNumber = 1;

    for (size_t i = 0; i < moveLog.size(); ++i) {
        try {
            // Get the move from the log
            Move move = moveLog[i];
            
            // Find the corresponding move in the temp engine's legal moves
            // This ensures we have the right move with all flags set
            auto legalMoves = tempEngine->getAllLegalMoves();
            Move* matchingMove = nullptr;
            
            for (auto& legalMove : legalMoves) {
                if (legalMove.getStartSquare()->getRow() == move.getStartSquare()->getRow() &&
                    legalMove.getStartSquare()->getCol() == move.getStartSquare()->getCol() &&
                    legalMove.getEndSquare()->getRow() == move.getEndSquare()->getRow() &&
                    legalMove.getEndSquare()->getCol() == move.getEndSquare()->getCol()) {
                    matchingMove = &legalMove;
                    break;
                }
            }
            
            std::string notation;
            if (matchingMove != nullptr) {
                // Generate algebraic notation for this move
                try {
                    notation = parser->toAlgebraicNotation(*matchingMove);
                } catch (const std::exception& e) {
                    // Fallback to basic notation if conversion fails
                    notation = move.toString();
                }
                
                // Execute move on temporary engine BEFORE adding check/mate symbols
                tempEngine->makeMove(*matchingMove);
                
                // Add check or checkmate notation
                std::string opponentColor = (matchingMove->getPieceMoved()->getColor() == "white") ? "black" : "white";
                if (tempEngine->isCheckmate()) {
                    notation += "#";
                } else if (tempEngine->isInCheck(opponentColor)) {
                    notation += "+";
                }
                
                // Add move number for white's moves
                if (i % 2 == 0) {
                    movesContent << moveNumber << ". ";
                }

                movesContent << notation << " ";

                // Increment move number after black's move
                if (i % 2 == 1) {
                    moveNumber++;
                }
            } else {
                // Couldn't find matching legal move, use toString
                if (i % 2 == 0) {
                    movesContent << moveNumber << ". ";
                }
                movesContent << move.toString() << " ";
                if (i % 2 == 1) {
                    moveNumber++;
                }
            }

        } catch (const std::exception& e) {
            // If there's an error, continue with next move
            continue;
        }
    }

    // Add result at the end
    movesContent << result;

    pgnContent << movesContent.str() << "\n";

    // Write to file
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filePath);
    }

    outFile << pgnContent.str();
    outFile.close();
}
