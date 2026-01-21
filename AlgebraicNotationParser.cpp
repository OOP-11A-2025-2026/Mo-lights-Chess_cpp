#include "AlgebraicNotationParser.h"
#include "ChessEngine.h"
#include "Move.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <sstream>

AlgebraicNotationParser::AlgebraicNotationParser(std::shared_ptr<ChessEngine> engine) 
    : engine(engine) {}

std::string AlgebraicNotationParser::toAlgebraicNotation(const Move& move) {
    std::string notation;

    // Handle castling
    if (move.getIsKingSideCastle()) {
        notation = "O-O";
    } else if (move.getIsQueenSideCastle()) {
        notation = "O-O-O";
    } else {
        auto piece = move.getPieceMoved();
        std::string pieceType = piece->getType();
        
        // Add piece letter (except for pawns)
        if (pieceType != "Pawn") {
            notation += piece->getPieceLetter();
        }
        
        // Get all legal moves to check for disambiguation
        std::vector<Move> legalMoves = engine->getAllLegalMoves();
        
        // Add disambiguation if needed
        std::string disambiguation = getDisambiguation(move, legalMoves);
        notation += disambiguation;
        
        // Add capture symbol
        if (move.getPieceCaptured() != nullptr || move.getIsEnpassantMove()) {
            // For pawn captures, add the file letter
            if (pieceType == "Pawn" && disambiguation.empty()) {
                notation += move.getStartSquare()->getAlgebraicNotation()[0];
            }
            notation += "x";
        }
        
        // Add destination square
        notation += move.getEndSquare()->getAlgebraicNotation();
        
        // Add promotion
        if (move.getIsPawnPromotionMove()) {
            notation += "=";
            notation += move.getPawnPromotionPiece()->getPieceLetter();
        }
    }
    
    // Note: Check/checkmate detection would require making the move temporarily
    // For now, we'll skip the + and # suffixes to avoid complexity
    // These can be added by examining the position after the move is made
    
    return notation;
}

Move AlgebraicNotationParser::parseMove(const std::string& san) {
    // Remove check and checkmate indicators
    std::string cleanSan = san;
    if (!cleanSan.empty() && (cleanSan.back() == '+' || cleanSan.back() == '#')) {
        cleanSan.pop_back();
    }
    
    // Handle castling
    if (cleanSan == "O-O" || cleanSan == "0-0") {
        // King-side castling
        for (const auto& move : engine->getAllLegalMoves()) {
            if (move.getIsKingSideCastle()) {
                return move;
            }
        }
        throw std::runtime_error("Invalid move: King-side castling not available");
    }
    
    if (cleanSan == "O-O-O" || cleanSan == "0-0-0") {
        // Queen-side castling
        for (const auto& move : engine->getAllLegalMoves()) {
            if (move.getIsQueenSideCastle()) {
                return move;
            }
        }
        throw std::runtime_error("Invalid move: Queen-side castling not available");
    }
    
    // Find matching move from legal moves
    std::vector<Move> legalMoves = engine->getAllLegalMoves();
    
    for (const auto& move : legalMoves) {
        if (moveMatchesSAN(move, cleanSan)) {
            return move;
        }
    }
    
    throw std::runtime_error("Invalid move: No legal move matches notation '" + san + "'");
}

bool AlgebraicNotationParser::moveMatchesSAN(const Move& move, const std::string& san) const {
    auto piece = move.getPieceMoved();
    std::string pieceType = piece->getType();
    std::string destination = move.getEndSquare()->getAlgebraicNotation();
    
    // Check if destination matches (SAN must contain the destination square)
    // For example: "e4" contains "e4", "exd5" contains "d5", "Nf3" contains "f3"
    if (san.find(destination) == std::string::npos) {
        return false;
    }
    
    // Additional check: for pawn moves without captures, SAN should equal destination
    // This prevents "e4" from matching a move to "e5" (since "e5".find("e4") fails anyway)
    // But we need to allow for things like "e4=Q" (pawn promotion)
    
    // Determine if SAN starts with a piece letter (K, Q, R, B, N)
    bool sanStartsWithPiece = (san[0] == 'K' || san[0] == 'Q' || san[0] == 'R' || 
                                san[0] == 'B' || san[0] == 'N');
    
    // Handle pawn moves - no piece letter at start (unless capturing)
    if (pieceType == "Pawn") {
        // If SAN starts with a piece letter, this can't be a pawn move
        if (sanStartsWithPiece) {
            return false;
        }
        
        // Pawn capture: starts with file letter (e.g., "exd5")
        if (move.getPieceCaptured() != nullptr || move.getIsEnpassantMove()) {
            char startFile = move.getStartSquare()->getAlgebraicNotation()[0];
            if (san.length() >= 2 && san[0] == startFile && san.find('x') != std::string::npos) {
                // Check for promotion
                if (move.getIsPawnPromotionMove()) {
                    if (san.find('=') != std::string::npos) {
                        // Extract the promotion piece letter from the notation
                        char promoPieceLetter = san[san.find('=') + 1];
                        // Match based on the notation letter (Q, R, B, N)
                        // Since promotion pieces are now null, we just verify it's a valid promotion
                        return (promoPieceLetter == 'Q' || promoPieceLetter == 'R' || 
                                promoPieceLetter == 'B' || promoPieceLetter == 'N');
                    }
                    return false;
                }
                return true;
            }
            return false;
        } else {
            // Regular pawn move: starts with file letter (e.g., "e4")
            if (san[0] >= 'a' && san[0] <= 'h') {
                // Check for promotion
                if (move.getIsPawnPromotionMove()) {
                    if (san.find('=') != std::string::npos) {
                        // Extract the promotion piece letter from the notation
                        char promoPieceLetter = san[san.find('=') + 1];
                        // Match based on the notation letter (Q, R, B, N)
                        return (promoPieceLetter == 'Q' || promoPieceLetter == 'R' || 
                                promoPieceLetter == 'B' || promoPieceLetter == 'N');
                    }
                    return false;
                }
                return true;
            }
            return false;
        }
    }
    
    // Handle other pieces - MUST start with piece letter
    if (!sanStartsWithPiece) {
        return false;  // Non-pawn moves must start with piece letter
    }
    
    char pieceLetter = piece->getPieceLetter()[0];
    if (san[0] != pieceLetter) {
        return false;
    }
    
    // Check for capture
    bool hasCapture = (move.getPieceCaptured() != nullptr);
    bool sanHasCapture = (san.find('x') != std::string::npos);
    if (hasCapture != sanHasCapture) {
        return false;
    }
    
    // Check disambiguation
    std::string startNotation = move.getStartSquare()->getAlgebraicNotation();
    
    // Extract disambiguation part from SAN (between piece letter and destination)
    size_t piecePos = 1;
    size_t destPos = san.find(destination);
    if (destPos == std::string::npos) return false;
    
    size_t xPos = san.find('x');
    size_t disambigEnd = (xPos != std::string::npos && xPos < destPos) ? xPos : destPos;
    
    if (disambigEnd > piecePos) {
        std::string disambig = san.substr(piecePos, disambigEnd - piecePos);
        
        // Check if disambiguation matches
        if (!disambig.empty()) {
            if (disambig.length() == 2) {
                // Full square disambiguation
                return startNotation == disambig;
            } else if (disambig.length() == 1) {
                // File or rank disambiguation
                char c = disambig[0];
                if (c >= 'a' && c <= 'h') {
                    // File disambiguation
                    return startNotation[0] == c;
                } else if (c >= '1' && c <= '8') {
                    // Rank disambiguation
                    return startNotation[1] == c;
                }
            }
        }
    }
    
    return true;
}

std::string AlgebraicNotationParser::getDisambiguation(const Move& move, const std::vector<Move>& legalMoves) const {
    auto piece = move.getPieceMoved();
    std::string pieceType = piece->getType();
    
    // Pawns don't need disambiguation (except for captures, handled separately)
    if (pieceType == "Pawn") {
        return "";
    }
    
    auto endSquare = move.getEndSquare();
    auto startSquare = move.getStartSquare();
    
    // Find other pieces of the same type that can move to the same square
    bool needsFileDisambig = false;
    bool needsRankDisambig = false;
    
    for (const auto& otherMove : legalMoves) {
        if (&otherMove == &move) continue;
        
        auto otherPiece = otherMove.getPieceMoved();
        auto otherStart = otherMove.getStartSquare();
        auto otherEnd = otherMove.getEndSquare();
        
        // Check if same piece type and same destination
        if (otherPiece->getType() == pieceType &&
            otherEnd->getRow() == endSquare->getRow() &&
            otherEnd->getCol() == endSquare->getCol()) {
            
            // Different starting square - need disambiguation
            // If same file (column), we need rank disambiguation
            // If same rank (row), we need file disambiguation  
            // If different file and rank, file disambiguation is preferred
            if (otherStart->getCol() == startSquare->getCol()) {
                // Same column, different row -> need rank (e.g., R1a3 vs R8a3)
                needsRankDisambig = true;
            }
            if (otherStart->getRow() == startSquare->getRow()) {
                // Same row, different column -> need file (e.g., Ra1 vs Rh1)
                needsFileDisambig = true;
            }
            if (otherStart->getCol() != startSquare->getCol() && 
                otherStart->getRow() != startSquare->getRow()) {
                // Different file and row -> prefer file disambiguation
                needsFileDisambig = true;
            }
        }
    }
    
    std::string notation = startSquare->getAlgebraicNotation();
    
    if (needsFileDisambig && needsRankDisambig) {
        // Need full square disambiguation
        return notation;
    } else if (needsRankDisambig) {
        // Need rank disambiguation
        return std::string(1, notation[1]);
    } else if (needsFileDisambig) {
        // Need file disambiguation
        return std::string(1, notation[0]);
    }
    
    return "";
}
