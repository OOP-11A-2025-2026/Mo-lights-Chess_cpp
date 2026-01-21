#include "ChessEngine.h"
#include "PGNReader.h"
#include "PGNWriter.h"
#include "AlgebraicNotationParser.h"
#include "exceptions/ChessException.h"
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>

// Helper function to validate square notation
bool isValidSquare(const std::string& square) {
    if (square.length() != 2) return false;
    char file = square[0];
    char rank = square[1];
    return (file >= 'a' && file <= 'h') && (rank >= '1' && rank <= '8');
}

// Helper function to trim whitespace
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void displayWelcome() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║     Welcome to Mo-Lights Chess C++    ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
}

void displayMainMenu() {
    std::cout << "\n========== Main Menu ==========" << std::endl;
    std::cout << "1. Start New Game" << std::endl;
    std::cout << "2. Load Game from PGN" << std::endl;
    std::cout << "3. Quit" << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "Choose an option: ";
}

void displayGameCommands() {
    std::cout << "\n--- Game Commands ---" << std::endl;
    std::cout << "• Enter move: <from> <to> (e.g., 'e2 e4') or algebraic (e.g., 'e4')" << std::endl;
    std::cout << "• 'undo' - Undo last move" << std::endl;
    std::cout << "• 'moves' - Show all legal moves" << std::endl;
    std::cout << "• 'resign' - Resign from the game" << std::endl;
    std::cout << "• 'draw' - Request/accept a draw" << std::endl;
    std::cout << "• 'decline' - Decline draw offer" << std::endl;
    std::cout << "• 'save <filename>' - Save game to PGN file" << std::endl;
    std::cout << "• 'menu' - Return to main menu" << std::endl;
    std::cout << "• 'help' - Show this help message" << std::endl;
    std::cout << "---------------------\n" << std::endl;
}

void playGame(std::shared_ptr<ChessEngine> engine) {
    AlgebraicNotationParser parser(engine);
    bool inGame = true;
    
    std::cout << "\nGame started!" << std::endl;
    displayGameCommands();
    
    while (inGame) {
        // Check for game over conditions first
        if (engine->getGameResult()->isGameOver()) {
            std::cout << "\n" << engine->getGameResult()->getResultMessage() << std::endl;
            inGame = false;
            continue;
        }
        
        // Display current state
        std::cout << "\nCurrent turn: " << engine->getCurrentTurn() << std::endl;
        
        // Show draw offer if pending
        if (!engine->getDrawRequestedBy().empty() && engine->getDrawRequestedBy() != engine->getCurrentTurn()) {
            std::cout << "⚡ " << engine->getDrawRequestedBy() << " has requested a draw. Type 'draw' to accept or 'decline' to refuse." << std::endl;
        }
        
        try {
            engine->getBoard()->printBoard();
        } catch (const std::exception& e) {
            std::cout << "Error printing board: " << e.what() << std::endl;
        }
        
        // Check for checkmate and stalemate
        if (engine->isCheckmate()) {
            std::string winner = (engine->getCurrentTurn() == "white") ? "Black" : "White";
            std::cout << "\n🏆 Checkmate! " << winner << " wins! 🏆" << std::endl;
            inGame = false;
            continue;
        }
        
        if (engine->isStalemate()) {
            std::cout << "\n🤝 Stalemate! The game is a draw. 🤝" << std::endl;
            inGame = false;
            continue;
        }
        
        if (engine->isInCheck(engine->getCurrentTurn())) {
            std::cout << "⚠️  Check! ⚠️" << std::endl;
        }
        
        // Get player input
        std::cout << "\n" << engine->getCurrentTurn() << " > ";
        std::string input;
        std::getline(std::cin, input);
        input = trim(input);
        
        if (input.empty()) continue;
        
        // Convert to lowercase for command matching
        std::string command = input;
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);
        
        try {
            // Handle commands
            if (command == "help") {
                displayGameCommands();
            }
            else if (command == "menu") {
                std::cout << "Returning to main menu..." << std::endl;
                inGame = false;
            }
            else if (command == "undo") {
                if (engine->getMoveLog().empty()) {
                    std::cout << "No moves to undo!" << std::endl;
                } else {
                    engine->undoMove();
                    std::cout << "Move undone." << std::endl;
                }
            }
            else if (command == "moves") {
                auto legalMoves = engine->getAllLegalMoves();
                std::cout << "\nLegal moves (" << legalMoves.size() << "):" << std::endl;
                int count = 0;
                for (const auto& move : legalMoves) {
                    std::cout << move.getStartSquare()->getAlgebraicNotation() << "->"
                              << move.getEndSquare()->getAlgebraicNotation() << "  ";
                    if (++count % 6 == 0) std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            else if (command == "resign") {
                std::cout << "Are you sure you want to resign? (y/n): ";
                std::string confirm;
                std::getline(std::cin, confirm);
                confirm = trim(confirm);
                if (confirm == "y" || confirm == "Y") {
                    engine->resign();
                    std::cout << engine->getCurrentTurn() << " has resigned." << std::endl;
                    inGame = false;
                }
            }
            else if (command == "draw") {
                if (engine->requestDraw()) {
                    std::cout << "Draw agreed! The game is a draw." << std::endl;
                    inGame = false;
                } else {
                    if (engine->getDrawRequestedBy() == engine->getCurrentTurn()) {
                        std::cout << "You have already requested a draw. Wait for opponent's response." << std::endl;
                    } else {
                        std::cout << "Draw offer sent to opponent." << std::endl;
                    }
                }
            }
            else if (command == "decline") {
                try {
                    engine->declineDraw();
                    std::cout << "Draw offer declined." << std::endl;
                } catch (const std::exception& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (command.substr(0, 4) == "save") {
                std::string filename = trim(input.substr(4));
                if (filename.empty()) {
                    std::cout << "Usage: save <filename>" << std::endl;
                } else {
                    PGNWriter writer(engine);
                    writer.writePGN(filename, "Game", "Local", "White", "Black", "*");
                    std::cout << "Game saved to " << filename << std::endl;
                }
            }
            else {
                // Try to parse as a move
                std::istringstream iss(input);
                std::string firstToken, secondToken;
                iss >> firstToken >> secondToken;
                
                Move* moveToMakePtr = nullptr;
                bool foundMove = false;
                
                // Try algebraic notation first if only one token
                if (secondToken.empty()) {
                    try {
                        Move parsedMove = parser.parseMove(firstToken);
                        // Now find this move in legal moves to get the proper reference
                        auto legalMoves = engine->getAllLegalMoves();
                        for (auto& move : legalMoves) {
                            if (move.getStartSquare()->getRow() == parsedMove.getStartSquare()->getRow() &&
                                move.getStartSquare()->getCol() == parsedMove.getStartSquare()->getCol() &&
                                move.getEndSquare()->getRow() == parsedMove.getEndSquare()->getRow() &&
                                move.getEndSquare()->getCol() == parsedMove.getEndSquare()->getCol()) {
                                moveToMakePtr = &move;
                                foundMove = true;
                                break;
                            }
                        }
                        if (!foundMove) {
                            std::cout << "Invalid move notation: " << firstToken << std::endl;
                            continue;
                        }
                    } catch (const std::exception& e) {
                        std::cout << "Invalid move notation: " << firstToken << std::endl;
                        continue;
                    }
                } else {
                    // Parse as coordinate notation (from to)
                    if (!isValidSquare(firstToken) || !isValidSquare(secondToken)) {
                        std::cout << "Invalid square notation. Use format like 'e2 e4'" << std::endl;
                        continue;
                    }
                    
                    auto fromSq = engine->getBoard()->getSquare(firstToken);
                    auto toSq = engine->getBoard()->getSquare(secondToken);
                    
                    if (!fromSq->hasPiece()) {
                        std::cout << "No piece at " << firstToken << std::endl;
                        continue;
                    }
                    
                    // Find matching legal move
                    auto legalMoves = engine->getAllLegalMoves();
                    for (auto& legalMove : legalMoves) {
                        if (legalMove.getStartSquare()->getAlgebraicNotation() == firstToken &&
                            legalMove.getEndSquare()->getAlgebraicNotation() == secondToken) {
                            moveToMakePtr = &legalMove;
                            foundMove = true;
                            break;
                        }
                    }
                    
                    if (!foundMove) {
                        std::cout << "Illegal move: " << firstToken << " to " << secondToken << std::endl;
                        continue;
                    }
                }
                
                if (foundMove && moveToMakePtr != nullptr) {
                    // Make a copy of the move to execute
                    Move moveToExecute = *moveToMakePtr;
                    
                    // Handle pawn promotion
                    if (moveToExecute.getIsPawnPromotionMove() && moveToExecute.getPawnPromotionPiece() == nullptr) {
                        std::cout << "Promote pawn to (Q/R/B/N): ";
                        std::string promoChoice;
                        std::getline(std::cin, promoChoice);
                        promoChoice = trim(promoChoice);
                        std::transform(promoChoice.begin(), promoChoice.end(), promoChoice.begin(), ::toupper);
                        
                        std::string color = moveToExecute.getPieceMoved()->getColor();
                        if (promoChoice == "Q") moveToExecute.setPawnPromotionPiece(std::make_shared<Queen>(color));
                        else if (promoChoice == "R") moveToExecute.setPawnPromotionPiece(std::make_shared<Rook>(color));
                        else if (promoChoice == "B") moveToExecute.setPawnPromotionPiece(std::make_shared<Bishop>(color));
                        else if (promoChoice == "N") moveToExecute.setPawnPromotionPiece(std::make_shared<Knight>(color));
                        else moveToExecute.setPawnPromotionPiece(std::make_shared<Queen>(color)); // Default to Queen
                    }
                    
                    engine->makeMove(moveToExecute);
                    std::cout << "Move executed." << std::endl;
                }
            }
            
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
    
    // After game ends, ask to save
    std::cout << "\nWould you like to save this game? (y/n): ";
    std::string saveChoice;
    std::getline(std::cin, saveChoice);
    if (saveChoice == "y" || saveChoice == "Y") {
        std::cout << "Enter filename: ";
        std::string filename;
        std::getline(std::cin, filename);
        filename = trim(filename);
        if (!filename.empty()) {
            try {
                PGNWriter writer(engine);
                std::string result = "*";
                if (engine->isCheckmate()) {
                    result = (engine->getCurrentTurn() == "white") ? "0-1" : "1-0";
                } else if (engine->isStalemate()) {
                    result = "1/2-1/2";
                }
                writer.writePGN(filename, "Game", "Local", "White", "Black", result);
                std::cout << "Game saved to " << filename << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Failed to save: " << e.what() << std::endl;
            }
        }
    }
}

int main() {
    displayWelcome();
    
    bool running = true;
    
    while (running) {
        displayMainMenu();
        
        std::string choice;
        std::getline(std::cin, choice);
        choice = trim(choice);
        
        if (choice == "1") {
            // Start new game
            auto engine = std::make_shared<ChessEngine>();
            playGame(engine);
        }
        else if (choice == "2") {
            // Load game from PGN
            std::cout << "Enter PGN filename: ";
            std::string filename;
            std::getline(std::cin, filename);
            filename = trim(filename);
            
            if (!filename.empty()) {
                try {
                    auto engine = std::make_shared<ChessEngine>();
                    PGNReader reader(engine);
                    reader.readPGN(filename);
                    std::cout << "Game loaded successfully! (" << engine->getMoveLog().size() << " moves)" << std::endl;
                    playGame(engine);
                } catch (const std::exception& e) {
                    std::cout << "Failed to load game: " << e.what() << std::endl;
                }
            }
        }
        else if (choice == "3") {
            std::cout << "\nThanks for playing Mo-Lights Chess! Goodbye! 👋" << std::endl;
            running = false;
        }
        else {
            std::cout << "Invalid option. Please choose 1, 2, or 3." << std::endl;
        }
    }
    
    return 0;
}
