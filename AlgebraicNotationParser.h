#ifndef ALGEBRAICNOTATIONPARSER_H
#define ALGEBRAICNOTATIONPARSER_H

#include <string>
#include <memory>
#include <vector>

// Forward declarations
class ChessEngine;
class Move;

class AlgebraicNotationParser {
private:
    std::shared_ptr<ChessEngine> engine;

    // Helper method to check if a move matches the given SAN notation
    bool moveMatchesSAN(const Move& move, const std::string& san) const;

    // Helper method to determine if disambiguation is needed
    std::string getDisambiguation(const Move& move, const std::vector<Move>& legalMoves) const;

public:
    // Constructor
    explicit AlgebraicNotationParser(std::shared_ptr<ChessEngine> engine);

    // Convert a Move object to algebraic notation (e.g., "e4", "Nf3", "Bxe5+")
    std::string toAlgebraicNotation(const Move& move);

    // Parse algebraic notation string and return the corresponding Move
    Move parseMove(const std::string& san);
};

#endif // ALGEBRAICNOTATIONPARSER_H
