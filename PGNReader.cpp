#include "PGNReader.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <stdexcept>
#include <algorithm>

PGNReader::PGNReader(std::shared_ptr<ChessEngine> engine) 
    : engine(engine), parser(std::make_shared<AlgebraicNotationParser>(engine)) {}

void PGNReader::readPGN(const std::string& filePath) {
    // Read file content
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    // Remove PGN headers (lines starting with [ and ending with ])
    std::regex headerRegex(R"(\[.*?\])");
    content = std::regex_replace(content, headerRegex, "");

    // Remove comments in braces
    std::regex commentRegex(R"(\{[^}]*\})");
    content = std::regex_replace(content, commentRegex, "");

    // Remove game result notations
    std::regex resultRegex(R"(\s*(1-0|0-1|1/2-1/2|\*)\s*)");
    content = std::regex_replace(content, resultRegex, " ");

    // Normalize whitespace - replace multiple spaces/newlines with single space
    std::regex whitespaceRegex(R"(\s+)");
    content = std::regex_replace(content, whitespaceRegex, " ");

    // Trim leading and trailing whitespace
    content.erase(0, content.find_first_not_of(" \t\n\r"));
    content.erase(content.find_last_not_of(" \t\n\r") + 1);

    // Split content into tokens
    std::istringstream iss(content);
    std::string token;
    std::vector<std::string> tokens;

    while (iss >> token) {
        tokens.push_back(token);
    }

    // Filter out move numbers (e.g., "1.", "2.", "10.")
    std::vector<std::string> moves;
    std::regex moveNumberRegex(R"(\d+\.)");

    for (const auto& tok : tokens) {
        if (!std::regex_match(tok, moveNumberRegex) && !tok.empty()) {
            // Also filter out result indicators that might have slipped through
            if (tok != "1-0" && tok != "0-1" && tok != "1/2-1/2" && tok != "*") {
                moves.push_back(tok);
            }
        }
    }

    // Parse and execute each move
    for (const auto& moveStr : moves) {
        try {
            Move move = parser->parseMove(moveStr);
            engine->makeMove(move);
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to parse move '" + moveStr + "': " + e.what());
        }
    }
}
