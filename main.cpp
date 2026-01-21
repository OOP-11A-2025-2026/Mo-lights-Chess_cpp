#include "ChessEngine.h"
#include "PGNReader.h"
#include "PGNWriter.h"
#include "AlgebraicNotationParser.h"
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

void printMenu() {
    std::cout << "\n========== Chess Engine Menu ==========" << std::endl;
    std::cout << "1. Start new game" << std::endl;
    std::cout << "2. Load game from PGN file" << std::endl;
    std::cout << "3. Save game to PGN file" << std::endl;
    std::cout << "4. Display current board" << std::endl;
    std::cout << "5. Make a move" << std::endl;
    std::cout << "6. Show move history" << std::endl;
    std::cout << "7. Exit" << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "Enter choice: ";
}

int main() {
    auto engine = std::make_shared<ChessEngine>();
    bool running = true;

    std::cout << "Welcome to Mo-Lights Chess C++!" << std::endl;
    std::cout << "Starting new game..." << std::endl;
    engine->getBoard()->printBoard();

    while (running) {
        printMenu();

        int choice;
        std::cin >> choice;

        // Handle invalid input
        if (std::cin.fail()) {
            std::cin.clear(); // Clear error flags
            std::cin.ignore(10000, '\n'); // Discard invalid input
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        switch (choice) {
            case 1: {
                // Start new game
                engine = std::make_shared<ChessEngine>();
                std::cout << "\nNew game started!" << std::endl;
                engine->getBoard()->printBoard();
                break;
            }

            case 2: {
                // Load PGN
                std::string filename;
                std::cout << "Enter PGN filename: ";
                std::cin >> filename;

                try {
                    engine = std::make_shared<ChessEngine>();
                    PGNReader reader(engine);
                    reader.readPGN(filename);
                    std::cout << "\nSuccessfully loaded game from " << filename << std::endl;
                    std::cout << "Loaded " << engine->getMoveLog().size() << " moves" << std::endl;
                    engine->getBoard()->printBoard();
                } catch (const std::exception& e) {
                    std::cerr << "Error loading PGN: " << e.what() << std::endl;
                }
                break;
            }

            case 3: {
                // Save PGN
                std::string filename, event, white, black, result;

                std::cout << "Enter PGN filename: ";
                std::cin >> filename;

                std::cin.ignore(); // Clear newline

                std::cout << "Enter event name (default: Casual Game): ";
                std::getline(std::cin, event);
                if (event.empty()) event = "Casual Game";

                std::cout << "Enter white player name (default: White): ";
                std::getline(std::cin, white);
                if (white.empty()) white = "White";

                std::cout << "Enter black player name (default: Black): ";
                std::getline(std::cin, black);
                if (black.empty()) black = "Black";

                std::cout << "Enter result (1-0, 0-1, 1/2-1/2, or * for ongoing): ";
                std::getline(std::cin, result);
                if (result.empty()) result = "*";

                try {
                    PGNWriter writer(engine);
                    writer.writePGN(filename, event, "Local", white, black, result);
                    std::cout << "\nSuccessfully saved game to " << filename << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error saving PGN: " << e.what() << std::endl;
                }
                break;
            }

            case 4: {
                // Display board
                std::cout << "\nCurrent turn: " << engine->getCurrentTurn() << std::endl;
                engine->getBoard()->printBoard();
                break;
            }

            case 5: {
                // Make a move
                std::string moveInput;
                std::cout << "Enter move (e.g., 'e2 e4' or algebraic like 'e4'): ";
                std::cin.ignore(); // Clear any leftover newline
                std::getline(std::cin, moveInput);
                
                std::string from, to;
                
                // Parse the input - support both "e2 e4" and algebraic notation like "e4"
                std::istringstream iss(moveInput);
                std::string firstToken, secondToken;
                iss >> firstToken >> secondToken;
                
                try {
                    // Try algebraic notation first (e.g., "e4", "Nf3")
                    if (secondToken.empty()) {
                        AlgebraicNotationParser parser(engine);
                        Move parsedMove = parser.parseMove(firstToken);
                        engine->makeMove(parsedMove);
                        
                        std::cout << "\nMove executed: " << firstToken << std::endl;
                        engine->getBoard()->printBoard();
                        
                        if (engine->isCheckmate()) {
                            std::string loser = engine->getCurrentTurn();
                            std::string winner = (loser == "white") ? "black" : "white";
                            std::cout << "\nCheckmate! " << winner << " wins!" << std::endl;
                        } else if (engine->isStalemate()) {
                            std::cout << "\nStalemate! Game is a draw." << std::endl;
                        } else if (engine->isInCheck(engine->getCurrentTurn())) {
                            std::cout << "\nCheck!" << std::endl;
                        }
                        break;
                    }
                    
                    // Otherwise use "from to" format
                    from = firstToken;
                    to = secondToken;
                    
                    auto startSq = engine->getBoard()->getSquare(from);
                    auto endSq = engine->getBoard()->getSquare(to);

                    if (!startSq->hasPiece()) {
                        std::cout << "No piece at " << from << std::endl;
                        break;
                    }

                    // Check if the piece belongs to the current player
                    if (startSq->getPiece()->getColor() != engine->getCurrentTurn()) {
                        std::cout << "It's " << engine->getCurrentTurn() << "'s turn!" << std::endl;
                        break;
                    }

                    // Create the move and check if it's legal
                    Move proposedMove(startSq, endSq);
                    std::vector<Move> legalMoves = engine->getAllLegalMoves();

                    bool isLegal = false;
                    Move legalMove = proposedMove;

                    for (auto& legalMv : legalMoves) {
                        if (legalMv.getStartSquare()->getRow() == startSq->getRow() &&
                            legalMv.getStartSquare()->getCol() == startSq->getCol() &&
                            legalMv.getEndSquare()->getRow() == endSq->getRow() &&
                            legalMv.getEndSquare()->getCol() == endSq->getCol()) {
                            isLegal = true;
                            legalMove = legalMv;
                            break;
                        }
                    }

                    if (!isLegal) {
                        std::cout << "Illegal move! That move is not allowed." << std::endl;
                        break;
                    }

                    engine->makeMove(legalMove);

                    std::cout << "\nMove executed: " << from << " to " << to << std::endl;
                    engine->getBoard()->printBoard();

                    if (engine->isCheckmate()) {
                        std::string loser = engine->getCurrentTurn();
                        std::string winner = (loser == "white") ? "black" : "white";
                        std::cout << "\nCheckmate! " << winner << " wins!" << std::endl;
                    } else if (engine->isStalemate()) {
                        std::cout << "\nStalemate! Game is a draw." << std::endl;
                    } else if (engine->isInCheck(engine->getCurrentTurn())) {
                        std::cout << "\nCheck!" << std::endl;
                    }

                } catch (const std::exception& e) {
                    std::cerr << "Error making move: " << e.what() << std::endl;
                }
                break;
            }

            case 6: {
                // Show move history
                std::cout << "\nMove History:" << std::endl;
                auto moveLog = engine->getMoveLog();

                if (moveLog.empty()) {
                    std::cout << "No moves yet." << std::endl;
                } else {
                    for (size_t i = 0; i < moveLog.size(); ++i) {
                        if (i % 2 == 0) {
                            std::cout << (i/2 + 1) << ". ";
                        }
                        std::cout << moveLog[i].toString();
                        if (i % 2 == 0) {
                            std::cout << " ";
                        } else {
                            std::cout << std::endl;
                        }
                    }
                    if (moveLog.size() % 2 == 1) {
                        std::cout << std::endl;
                    }
                }
                break;
            }

            case 7: {
                // Exit
                std::cout << "Thanks for playing Mo-Lights Chess!" << std::endl;
                running = false;
                break;
            }

            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }

    return 0;
}
