#ifndef PGNREADER_H
#define PGNREADER_H

#include "ChessEngine.h"
#include "AlgebraicNotationParser.h"
#include <string>
#include <memory>

class PGNReader {
private:
    std::shared_ptr<ChessEngine> engine;
    std::shared_ptr<AlgebraicNotationParser> parser;

public:
    // Constructor
    explicit PGNReader(std::shared_ptr<ChessEngine> engine);

    // Read and parse a PGN file, executing the moves on the engine
    void readPGN(const std::string& filePath);
};

#endif // PGNREADER_H
