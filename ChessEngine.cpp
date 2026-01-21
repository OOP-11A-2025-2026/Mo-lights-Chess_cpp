#include "ChessEngine.h"
#include <iostream>

using namespace std;

ChessEngine::ChessEngine()
{
    board = make_shared<Board>();
    currentTurn = "white";
    gameResult = make_shared<GameResult>();
    drawRequestedBy = "";
}
    
    
shared_ptr<Board> ChessEngine::getBoard() const 
{
    return this->board; 
}

vector<Move> ChessEngine::getMoveLog() const 
{ 
    return this->moveLog; 
}

string ChessEngine::getCurrentTurn() const 
{ 
    return this->currentTurn; 
}

shared_ptr<GameResult> ChessEngine::getGameResult() const
{
    return this->gameResult;
}

string ChessEngine::getDrawRequestedBy() const
{
    return this->drawRequestedBy;
}
    
void ChessEngine::clearDrawRequest()
{
    this->drawRequestedBy = "";
}
    
void ChessEngine::setCurrentTurn(const string& currentTurn)
{
    if((currentTurn != "white") && (currentTurn != "black"))
    {
        throw invalid_argument("current turn must be white or black");
    }
    this->currentTurn = currentTurn;
}
    
void ChessEngine::makeMove(Move& move)
{
    if(this->currentTurn != move.getPieceMoved()->getColor())
    {
        throw invalid_argument("It is not " + move.getPieceMoved()->getColor() + " turn");
    }
    shared_ptr<Piece> pieceMoved = move.getPieceMoved();
    shared_ptr<Square> start = move.getStartSquare();
    shared_ptr<Square> end = move.getEndSquare();
    
    if(move.getIsPawnPromotionMove())
    {
        // For pawn promotion, place the promotion piece directly
        shared_ptr<Piece> promotionPiece = move.getPawnPromotionPiece();
        
        if(promotionPiece == nullptr)
        {
            throw invalid_argument("Pawn promotion piece must be set before calling makeMove");
        }
        
        end->setPiece(promotionPiece);
        start->removePiece();
        promotionPiece->setMoved(true);
    }
    else
    {
        // Normal move
        end->setPiece(pieceMoved);
        start->removePiece();
        pieceMoved->setMoved(true);
    }
    
    if(move.getIsEnpassantMove()) // en passant move
    {
        move.getEnPassantCapturingSquare()->removePiece();
    }
    // Handle castling - move the rook as well
    if(move.getIsKingSideCastle()) {
        int row = start->getRow();
        shared_ptr<Square> rookStart = this->board->getSquare(row, 7);
        shared_ptr<Square> rookEnd = this->board->getSquare(row, 5);
        shared_ptr<Piece> rook = rookStart->getPiece();
        rookEnd->setPiece(rook);
        rookStart->removePiece();
        rook->setMoved(true);
    }
    if(move.getIsQueenSideCastle()) {
        int row = start->getRow();
        shared_ptr<Square> rookStart = this->board->getSquare(row, 0);
        shared_ptr<Square> rookEnd = this->board->getSquare(row, 3);
        shared_ptr<Piece> rook = rookStart->getPiece();
        rookEnd->setPiece(rook);
        rookStart->removePiece();
        rook->setMoved(true);
    }
    this->currentTurn = this->currentTurn == "white" ? "black" : "white";
    this->moveLog.push_back(move);
       
}
    
void ChessEngine::undoMove()
{
    if(this->moveLog.empty())
    {
        throw invalid_argument("Cannot undo move from starting position");
    }
    Move lastMove = this->moveLog.back(); 
    this->moveLog.pop_back();
    shared_ptr<Piece> pieceMoved = lastMove.getPieceMoved();
    shared_ptr<Piece> pieceCaptured = lastMove.getPieceCaptured();
    shared_ptr<Square> start = lastMove.getStartSquare();
    shared_ptr<Square> end = lastMove.getEndSquare();
    if(lastMove.getIsEnpassantMove()) // undo en passant move
    {
        start->setPiece(pieceMoved);
        end->removePiece();     
        lastMove.getEnPassantCapturingSquare()->setPiece(pieceCaptured);
        pieceMoved->setMoved(lastMove.getHadPieceBeenMoved());
        this->currentTurn = this->currentTurn == "white" ? "black" : "white";
        return;
    }
    // Handle undoing castling
    if(lastMove.getIsKingSideCastle() || lastMove.getIsQueenSideCastle()) {
        int row = start->getRow();
        start->setPiece(pieceMoved);
        end->removePiece();
        pieceMoved->setMoved(lastMove.getHadPieceBeenMoved());
        
        if(lastMove.getIsKingSideCastle()) 
        {
            shared_ptr<Square> rookEnd = this->board->getSquare(row, 5);
            shared_ptr<Square> rookStart = this->board->getSquare(row, 7);
            shared_ptr<Piece> rook = rookEnd->getPiece();
            rookStart->setPiece(rook);
            rookEnd->removePiece();
            rook->setMoved(false);
        } 
        else 
        { // queenside castle
            shared_ptr<Square> rookEnd = this->board->getSquare(row, 3);
            shared_ptr<Square> rookStart = this->board->getSquare(row, 0);
            shared_ptr<Piece> rook = rookEnd->getPiece();
            rookStart->setPiece(rook);
            rookEnd->removePiece();
            rook->setMoved(false);
        }
        
        this->currentTurn = this->currentTurn == "white" ? "black" : "white";
        return;
    }
    // pawn promotion is the same undo move as normal move
    start->setPiece(pieceMoved);
    end->setPiece(pieceCaptured);
    pieceMoved->setMoved(lastMove.getHadPieceBeenMoved());
    this->currentTurn = this->currentTurn == "white" ? "black" : "white";
}
    
shared_ptr<Square> ChessEngine::findKing(const string& color) const
{
    for(int row = 0; row<8; row++) 
    {
        for(int col = 0; col < 8; col++) 
        {
            shared_ptr<Square> square = this->board->getSquare(row, col);
            if(square->hasPiece()) 
            {
                shared_ptr<Piece> piece = square->getPiece();
                if(piece->getType() == "King" && piece->getColor() == color) 
                {
                    return square;
                }
            }
        }
    }
    throw out_of_range("King not found for color: " + color);
}
   
// Check if a square is under attack by the specified color
bool ChessEngine::isSquareUnderAttack(int row, int col, const string& byColor)
{
 // Save current turn
    string originalTurn = this->currentTurn;
        
    // Temporarily switch turn to get opponent's moves
    this->currentTurn = byColor;
        
    // Get all possible moves for the attacking color (not legal moves to avoid recursion)
    vector<Move> opponentMoves = this->getAllPossibleMoves();
        
    // Restore original turn
    this->currentTurn = originalTurn;
        
    // Check if any opponent move targets this square
    for(const auto& move : opponentMoves) 
    {
        if(move.getEndSquare()->getRow() == row && move.getEndSquare()->getCol() == col) 
        {
            return true;
        }
    }
        
    return false;
}
    
bool ChessEngine::isInCheck(const string& color)
{
    shared_ptr<Square> kingSquare = this->findKing(color);
    string opponentColor = color == "white" ? "black" : "white";
    return this->isSquareUnderAttack(kingSquare->getRow(), kingSquare->getCol(), opponentColor);
}
    
vector<Move> ChessEngine::getAllLegalMoves()
{
    vector<Move> possibleMoves = this->getAllPossibleMoves();
    vector<Move> legalMoves;
        
    // Test each move to see if it leaves the king in check
    for(Move move : possibleMoves) 
    {
        // Special handling for castling - check that king doesn't castle through check
        if(move.getIsKingSideCastle() || move.getIsQueenSideCastle()) {
            string kingColor = move.getPieceMoved()->getColor();
            string opponentColor = kingColor == "white" ? "black" : "white";
            int row = move.getStartSquare()->getRow();
            int startCol = move.getStartSquare()->getCol();
            
            // King cannot castle out of check
            if(this->isInCheck(kingColor)) 
            {
                continue;
            }
            
            // Check intermediate square(s)
            bool passesThroughCheck = false;
            if(move.getIsKingSideCastle()) 
            {
                // King moves from col 4 to col 6, passing through col 5
                if (this->isSquareUnderAttack(row, startCol + 1, opponentColor)) 
                {
                    passesThroughCheck = true;
                }
            } 
            else 
            { 
                // Queenside castle
                // King moves from col 4 to col 2, passing through col 3
                if(this->isSquareUnderAttack(row, startCol - 1, opponentColor)) {
                    passesThroughCheck = true;
                }
            }
            
            if(passesThroughCheck) 
            {
                continue;
            }
        }
        
        // Make the move temporarily
        try 
        {
            this->makeMoveTesting(move);
            
            // Check if this leaves our king in check
            if(!this->isInCheck(move.getPieceMoved()->getColor())) 
            {
                legalMoves.push_back(move);
            }
            
            // Undo the move
            this->undoMoveTesting();
        } 
        catch(exception e) 
        {
            // If there's an error, skip this move
            try 
            {
                // Try to undo anyway
                if(!this->moveLog.empty()) 
                {
                    this->undoMoveTesting();
                }
            } 
            catch(exception undoError) 
            {
                // Ignore undo errors
            }
        }
    }
        
    return legalMoves;
}
    
bool ChessEngine::isCheckmate()
{
    return this->getAllLegalMoves().empty() && this->isInCheck(this->currentTurn);
}
    
bool ChessEngine::isStalemate()
{
    return this->getAllLegalMoves().empty() && !this->isInCheck(this->currentTurn);
}
    
vector<Move> ChessEngine::getAllPossibleMoves()
{
    vector<Move> possibleMoves;
    string cols = "abcdefgh";
    string rows = "87654321";
    for(int r = 0; r < rows.length(); r++) 
    {
        for(int c = 0; c < cols.length(); c++) 
        {
            shared_ptr<Square> currentSquare = this->board->getSquare(r, c);
            shared_ptr<Piece> currentPiece = currentSquare->getPiece();
            if(currentPiece != nullptr)
            {
                if(currentPiece->getColor() == this->currentTurn)
                {
                    if(currentPiece->getType() == "Pawn")
                    {
                        this->getPawnMoves(currentSquare, possibleMoves);
                    }
                    if(currentPiece->getType() == "Rook")
                    {
                        this->getRookMoves(currentSquare, possibleMoves);
                    }
                    if(currentPiece->getType() == "Bishop")
                    {
                        this->getBishopMoves(currentSquare, possibleMoves);
                    }
                    if(currentPiece->getType() == "Queen")
                    {
                        this->getQueenMoves(currentSquare, possibleMoves);
                    }
                    if(currentPiece->getType() == "Knight")
                    {
                        this->getKnightMoves(currentSquare, possibleMoves);
                    }
                    if(currentPiece->getType() == "King")
                    {
                        this->getKingMoves(currentSquare, possibleMoves);
                    }
                }
            }
        }
    }

    return possibleMoves;
} 
    
void ChessEngine::getPawnMoves(const shared_ptr<Square> startSquare, vector<Move>& possibleMoves)
{
    auto pawn = static_pointer_cast<Pawn>(startSquare->getPiece());
    vector<vector<int>> possiblePawnMoves = pawn->getPossibleMoves(startSquare->getRow(), startSquare->getCol());
    for(const auto& moveCoords : possiblePawnMoves)
    {
        int newRow = moveCoords[0];
        int newCol = moveCoords[1];
        
        shared_ptr<Square> targetSquare = this->board->getSquare(newRow, newCol);
        shared_ptr<Piece> targetPiece = targetSquare->getPiece();

        bool sameColumn = (newCol == startSquare->getCol());
        bool diagonalMove = abs(newCol - startSquare->getCol()) == 1;
        bool moveAdded = false;
        if(sameColumn)
        {
            if(targetPiece != nullptr) continue;
            if(abs(newRow - startSquare->getRow()) == 2)
            {
                int midRow = (newRow + startSquare->getRow()) / 2;
                if(board->getSquare(midRow, newCol)->getPiece() != nullptr) continue;
            }
            possibleMoves.push_back(Move(startSquare, targetSquare));
            moveAdded = true;
        }
        if(diagonalMove)
        {
            if(targetPiece == nullptr)
            {
                //en passant 
                if(!this->moveLog.empty()) 
                {
                    const Move& lastMove = this->moveLog.back();
                    if(lastMove.getPieceMoved()->getType() == "Pawn")
                    {
                        int pawnRow = startSquare->getRow();
                        int pawnCol = startSquare->getCol();
                        int lastStartRow = lastMove.getStartSquare()->getRow();
                        int lastEndRow = lastMove.getEndSquare()->getRow();
                        int lastEndCol = lastMove.getEndSquare()->getCol();

                        if(abs(lastStartRow - lastEndRow) == 2 && lastEndRow == pawnRow && lastEndCol == newCol) 
                        {
                            
                            if(abs(lastEndCol - pawnCol) == 1) 
                            {
                                int direction = (pawn->getColor() == "white") ? -1 : 1;
                                int epRow = pawnRow + direction;
                                int epCol = lastEndCol; 
                            
                                shared_ptr<Square> epSquare = this->board->getSquare(epRow, epCol);
                                if(!epSquare->hasPiece()) 
                                {
                                    Move enPassantMove(startSquare, epSquare);
                                    enPassantMove.setEnpassant();
                                    enPassantMove.setPieceCaptured(lastMove.getPieceMoved());
                                    enPassantMove.setEnPassantCapturingSquare(lastMove.getEndSquare());
                                    possibleMoves.push_back(enPassantMove);
                                }
                            }    
                        }
                    }
                }
                continue;
            }

            if(targetPiece->getColor() != pawn->getColor())
            {
                possibleMoves.push_back(Move(startSquare, targetSquare));
                moveAdded = true;
            }
        }
        if(moveAdded) // pawn promotion
        {
            Move lastAddedMove = possibleMoves.back(); // Make a copy instead of reference
            int moveRow = lastAddedMove.getEndSquare()->getRow();
            if(moveRow == 0 || moveRow == 7)
            {
                string color = lastAddedMove.getPieceMoved()->getColor();
                
                // Remove the move we just added since we'll replace it with 4 promotion options
                possibleMoves.pop_back();
                
                // Create the four promotion moves
                // NOTE: We set promotion pieces to nullptr initially
                // They will be created fresh when the move is actually made
                Move pawnToQueen = lastAddedMove;
                pawnToQueen.setPawnPromotion();
                pawnToQueen.setPawnPromotionPiece(nullptr); // Will be set later
                possibleMoves.push_back(pawnToQueen);

                Move pawnToRook = lastAddedMove; 
                pawnToRook.setPawnPromotion();
                pawnToRook.setPawnPromotionPiece(nullptr);
                possibleMoves.push_back(pawnToRook);

                Move pawnToBishop = lastAddedMove;
                pawnToBishop.setPawnPromotion();
                pawnToBishop.setPawnPromotionPiece(nullptr);
                possibleMoves.push_back(pawnToBishop);

                Move pawnToKnight = lastAddedMove;
                pawnToKnight.setPawnPromotion();
                pawnToKnight.setPawnPromotionPiece(nullptr);
                possibleMoves.push_back(pawnToKnight);
            }
        }

    }
            
}

void ChessEngine::getRookMoves(const shared_ptr<Square> startSquare, vector<Move>& possibleMoves)
{
    int row = startSquare->getRow();
    int col = startSquare->getCol();
    string myColor = startSquare->getPiece()->getColor();

    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for(auto& dir : directions) 
    {
        for(int i = 1; i < 8; i++) 
        {
            int nRow = row + dir[0] * i;
            int nCol = col + dir[1] * i;

            if(nRow >= 0 && nRow < 8 && nCol >= 0 && nCol < 8) 
            {
                shared_ptr<Square> targetSquare = this->board->getSquare(nRow, nCol);
                
                if(!targetSquare->hasPiece()) 
                {
                    possibleMoves.push_back(Move(startSquare, targetSquare));
                } 
                else 
                {
                    if(targetSquare->getPiece()->getColor() != myColor) 
                    {
                        possibleMoves.push_back(Move(startSquare, targetSquare));
                    }
                    break; 
                }    
            } 
            else 
            {
                break; 
            }
        }
    }
}
    
void ChessEngine::getBishopMoves(const shared_ptr<Square> startSquare, vector<Move>& possibleMoves)
{
    int row = startSquare->getRow();
    int col = startSquare->getCol();
    string myColor = startSquare->getPiece()->getColor();

    int directions[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

    for(auto& dir : directions) 
    {
        for(int i = 1; i < 8; i++) 
        {
            int nRow = row + dir[0] * i;
            int nCol = col + dir[1] * i;

            if(nRow >= 0 && nRow < 8 && nCol >= 0 && nCol < 8) 
            {
                shared_ptr<Square> targetSquare = this->board->getSquare(nRow, nCol);
                
                if(!targetSquare->hasPiece()) 
                {
                    possibleMoves.push_back(Move(startSquare, targetSquare));
                } 
                else 
                {
                    if(targetSquare->getPiece()->getColor() != myColor) 
                    {
                        possibleMoves.push_back(Move(startSquare, targetSquare));
                    }
                    break; 
                }    
            } 
            else 
            {
                break; 
            }
        }
    }
}
    
void ChessEngine::getQueenMoves(const shared_ptr<Square> startSquare, vector<Move>& possibleMoves)
{
    getRookMoves(startSquare, possibleMoves);
    getBishopMoves(startSquare, possibleMoves);
}
    
void ChessEngine::getKnightMoves(const shared_ptr<Square> startSquare, vector<Move>& possibleMoves)
{
    auto knight = static_pointer_cast<Knight>(startSquare->getPiece());
    
    vector<vector<int>> possibleKnightMoves = knight->getPossibleMoves(startSquare->getRow(), startSquare->getCol());  

    for(const auto& move : possibleKnightMoves)
    {
        int newRow = move[0];
        int newCol = move[1];

        shared_ptr<Square> targetSquare = this->board->getSquare(newRow, newCol);
        shared_ptr<Piece> targetPiece = targetSquare->getPiece();

        if(targetPiece == nullptr)
        {
            possibleMoves.push_back(Move(startSquare, targetSquare));
        }
        else
        {
            if(targetPiece->getColor() != knight->getColor())
            {
                possibleMoves.push_back(Move(startSquare, targetSquare));
            }
        }
    }
}
    
void ChessEngine::getKingMoves(const shared_ptr<Square> startSquare, vector<Move>& possibleMoves)
{
    auto king = static_pointer_cast<King>(startSquare->getPiece());
    
    vector<vector<int>> possibleKingMoves = king->getPossibleMoves(startSquare->getRow(), startSquare->getCol());
    for(const auto& coords : possibleKingMoves) 
    {
        int newRow = coords[0];
        int newCol = coords[1];
        
        shared_ptr<Square> targetSquare = this->board->getSquare(newRow, newCol);
        shared_ptr<Piece> targetPiece = targetSquare->getPiece();
        
        if(targetPiece == nullptr) 
        {
            possibleMoves.push_back(Move(startSquare, targetSquare));
        } 
        else
        {
            if(targetPiece->getColor() != king->getColor()) 
            {
                possibleMoves.push_back(Move(startSquare, targetSquare));
            }
        }
    }

    if(!king->hasMoved()) 
    {
        int kingRow = startSquare->getRow();
        int kingCol = startSquare->getCol();

        shared_ptr<Square> kingsideRookSquare = this->board->getSquare(kingRow, 7);
        if(kingsideRookSquare->hasPiece() && 
            kingsideRookSquare->getPiece()->getType() == "Rook" &&
            kingsideRookSquare->getPiece()->getColor() == king->getColor() &&
            !kingsideRookSquare->getPiece()->hasMoved()) 
        {
            bool pathClear = true;
            for(int col = kingCol + 1; col<7; col++) 
            {
                if(this->board->getSquare(kingRow, col)->hasPiece()) 
                {
                    pathClear = false;
                    break;
                }
            }

            if(pathClear) 
            {
                shared_ptr<Square> targetSquare = this->board->getSquare(kingRow, kingCol + 2);
                Move castleMove(startSquare, targetSquare);
                castleMove.setKingSideCastle(); 
                possibleMoves.push_back(castleMove);
            }
        }

        shared_ptr<Square> queensideRookSquare = this->board->getSquare(kingRow, 0);
        if(queensideRookSquare->hasPiece() && 
            queensideRookSquare->getPiece()->getType() == "Rook" &&
            queensideRookSquare->getPiece()->getColor() == king->getColor() &&
            !queensideRookSquare->getPiece()->hasMoved()) 
        {

            bool pathClear = true;
            for(int col = 1; col<kingCol; col++) 
            {
                if(this->board->getSquare(kingRow, col)->hasPiece()) 
                {
                    pathClear = false;
                    break;
                }
            }

            if(pathClear) 
            {
                shared_ptr<Square> targetSquare = this->board->getSquare(kingRow, kingCol - 2);
                Move castleMove(startSquare, targetSquare);
                castleMove.setQueenSideCastle();
                possibleMoves.push_back(castleMove);
            }
        }
    }
}
    
void ChessEngine::resign()
{
    string winner = this->currentTurn == "white" ? "BLACK" : "WHITE";
    GameResult::ResultType resultType = (this->currentTurn == "white") ? 
        GameResult::ResultType::WHITE_RESIGNED : GameResult::ResultType::BLACK_RESIGNED;
    this->gameResult->setResult(resultType, this->currentTurn + " resigned");
}
    
bool ChessEngine::requestDraw()
{
    if(this->drawRequestedBy == "")
    {
        this->drawRequestedBy = this->currentTurn;
        return false;
    }
    else if(this->drawRequestedBy == this->currentTurn)
    {
        return false;
    }
    else
    {
        this->gameResult->setResult(GameResult::ResultType::DRAW, "Draw agreed by both players");
        this->drawRequestedBy = "";
        return true;
    }
}
    
void ChessEngine::acceptDraw()
{
    if(this->drawRequestedBy == "") 
    {
        throw std::runtime_error("No draw request to accept");
    }
    if(this->drawRequestedBy == this->currentTurn) {
        throw std::runtime_error("You cannot accept your own draw request");
    }

    this->gameResult->setResult(GameResult::ResultType::DRAW, "Draw agreed by both players");
    this->drawRequestedBy = "";
}
    
void ChessEngine::declineDraw()
{   
    if(this->drawRequestedBy == "") 
    {
        throw std::runtime_error("No draw request to decline");
    }
    this->drawRequestedBy = "";
}

void ChessEngine::makeMoveTesting(Move move)
{
    shared_ptr<Piece> pieceMoved = move.getPieceMoved();
    shared_ptr<Square> start = move.getStartSquare();
    shared_ptr<Square> end = move.getEndSquare();
    
    // Store the original moved status of the piece that's actually moving
    bool hadMoved = pieceMoved->hasMoved();
    move.setHadPieceBeenMoved(hadMoved);
    
    if(move.getIsPawnPromotionMove()) 
    {
        // For pawn promotion testing, just promote to queen for simplicity
        // The actual promotion piece will be set when the move is really made
        shared_ptr<Piece> promotionPiece = move.getPawnPromotionPiece();
        if(promotionPiece == nullptr)
        {
            // Create a temporary queen for testing purposes
            promotionPiece = make_shared<Queen>(pieceMoved->getColor());
        }
        end->setPiece(promotionPiece);
        start->removePiece();
        promotionPiece->setMoved(true);
    }
    else
    {
        // Normal move
        end->setPiece(pieceMoved);
        start->removePiece();
        pieceMoved->setMoved(true);
    }

    if(move.getIsEnpassantMove()) 
    {
        shared_ptr<Square> captureSquare = move.getEnPassantCapturingSquare();
        if(captureSquare) 
        {
            captureSquare->removePiece();
        }
    }
    if(move.getIsKingSideCastle()) 
    {
        int row = start->getRow();
        shared_ptr<Square> rookStart = this->board->getSquare(row, 7);
        shared_ptr<Square> rookEnd = this->board->getSquare(row, 5);
        shared_ptr<Piece> rook = rookStart->getPiece();
        rookEnd->setPiece(rook);
        rookStart->removePiece();
        rook->setMoved(true);
    }
    if(move.getIsQueenSideCastle()) 
    {
        int row = start->getRow();
        shared_ptr<Square> rookStart = this->board->getSquare(row, 0);
        shared_ptr<Square> rookEnd = this->board->getSquare(row, 3);
        shared_ptr<Piece> rook = rookStart->getPiece();
        rookEnd->setPiece(rook);
        rookStart->removePiece();
        rook->setMoved(true);
    }
    this->moveLog.push_back(move);
}
    
void ChessEngine::undoMoveTesting()
{
    if(this->moveLog.empty()) 
    {
        return;
    }
    Move lastMove = this->moveLog.back();
    this->moveLog.pop_back();
    shared_ptr<Piece> pieceMoved = lastMove.getPieceMoved();
    shared_ptr<Piece> pieceCaptured = lastMove.getPieceCaptured();
    shared_ptr<Square> start = lastMove.getStartSquare();
    shared_ptr<Square> end = lastMove.getEndSquare();
    if(lastMove.getIsEnpassantMove()) 
    {
        start->setPiece(pieceMoved);
        end->removePiece();
        shared_ptr<Square> epCaptureSquare = lastMove.getEnPassantCapturingSquare();
        if(epCaptureSquare) 
        {
            epCaptureSquare->setPiece(pieceCaptured);
        }
        pieceMoved->setMoved(lastMove.getHadPieceBeenMoved());
        return;
    }
    if(lastMove.getIsKingSideCastle() || lastMove.getIsQueenSideCastle()) 
    {
        int row = start->getRow();
        start->setPiece(pieceMoved);
        end->removePiece();
        pieceMoved->setMoved(lastMove.getHadPieceBeenMoved());

        if(lastMove.getIsKingSideCastle()) 
        {
            shared_ptr<Square> rookEnd = this->board->getSquare(row, 5);
            shared_ptr<Square> rookStart = this->board->getSquare(row, 7);
            shared_ptr<Piece> rook = rookEnd->getPiece();
            rookStart->setPiece(rook);
            rookEnd->removePiece();
            rook->setMoved(false); 
        }
        else 
        {
            shared_ptr<Square> rookEnd = this->board->getSquare(row, 3);
            shared_ptr<Square> rookStart = this->board->getSquare(row, 0);
            shared_ptr<Piece> rook = rookEnd->getPiece();
            rookStart->setPiece(rook);
            rookEnd->removePiece();
            rook->setMoved(false);
        }
        return;   
    }
    start->setPiece(pieceMoved);
    end->setPiece(pieceCaptured);   
    pieceMoved->setMoved(lastMove.getHadPieceBeenMoved());
}


