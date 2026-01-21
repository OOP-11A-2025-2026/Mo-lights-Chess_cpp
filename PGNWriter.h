#ifndef PGNWRITER_H
#define PGNWRITER_H

#include "ChessEngine.h"
#include <string>
#include <memory>

class PGNWriter {
private:
    std::shared_ptr<ChessEngine> engine;

public:
    // Constructor
    explicit PGNWriter(std::shared_ptr<ChessEngine> engine);

    // Write the current game to a PGN file
    void writePGN(const std::string& filePath, 
                  const std::string& event = "Casual Game",
                  const std::string& site = "?",
                  const std::string& whitePlayer = "White",
                  const std::string& blackPlayer = "Black",
                  const std::string& result = "*");
};

#endif // PGNWRITER_H
