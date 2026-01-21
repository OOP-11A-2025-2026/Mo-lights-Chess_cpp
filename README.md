# Mo-Lights Chess C++

A fully-featured chess engine and interactive game implemented in C++17, complete with PGN support, algebraic notation, and a command-line interface.

## Features

- **Complete Chess Rules**: All standard chess rules including castling, en passant, and pawn promotion
- **Legal Move Validation**: Automatic detection of check, checkmate, and stalemate
- **Multiple Input Methods**: 
  - Coordinate notation (e.g., `e2 e4`)
  - Algebraic notation (e.g., `e4`, `Nf3`, `O-O`)
- **PGN Support**: 
  - Save games to PGN format
  - Load and replay games from PGN files
- **Interactive Commands**:
  - Undo moves
  - View all legal moves
  - Resign or offer draws
  - Save/load games
- **Clean Terminal UI**: Visual board with Unicode chess pieces

## Building

### Compile
```bash
make
```

### Clean Build
```bash
make clean
make
```

## Running

Start a new game:
```bash
./chess
```

## How to Play

### Starting a Game
When you run the program, you'll see the main menu:
1. **Start New Game** - Begin a new chess game
2. **Load Game from PGN** - Load a previously saved game
3. **Quit** - Exit the program

### Making Moves

You can enter moves in two ways:

#### 1. Coordinate Notation
Specify the starting and ending squares:
```
white > e2 e4
black > e7 e5
white > g1 f3
```

#### 2. Algebraic Notation
Use standard chess notation:
```
white > e4
black > e5
white > Nf3
black > Nc6
white > Bb5
```

#### Pawn Promotion
When promoting a pawn, include the piece in the notation:
```
white > e8=Q    (promote to Queen)
black > a1=N    (promote to Knight)
```

Or enter just the move and you'll be prompted for the promotion piece.

#### Castling
```
white > O-O     (kingside castling)
black > O-O-O   (queenside castling)
```

### Available Commands

During a game, you can use these commands:

- **`undo`** - Undo the last move
- **`moves`** - Show all legal moves for the current position
- **`resign`** - Resign from the game
- **`draw`** - Request or accept a draw
- **`decline`** - Decline a draw offer
- **`save <filename>`** - Save the game to a PGN file
- **`menu`** - Return to the main menu
- **`help`** - Show the commands list

## Game Rules Implemented

### Standard Rules
- All piece movements (King, Queen, Rook, Bishop, Knight, Pawn)
- Pawn double-move on first move
- Pawn promotion to Queen, Rook, Bishop, or Knight
- En passant captures
- Castling (kingside and queenside)

### Game States
- Check detection
- Checkmate detection
- Stalemate detection
- Draw by agreement

### Move Validation
- Moves are validated to ensure they don't leave the king in check
- Cannot castle through check or while in check
- Cannot castle if king or rook has moved

## Project Structure

```
Mo-lights-Chess_cpp/
├── main.cpp                    # Main game loop and UI
├── ChessEngine.cpp/h           # Core game logic and move generation
├── Board.cpp/h                 # Board representation
├── Move.cpp/h                  # Move data structure
├── Square.cpp/h                # Square representation
├── Piece.cpp/h                 # Base piece class
├── GameResult.cpp/h            # Game state management
├── AlgebraicNotationParser.cpp/h  # Algebraic notation parsing
├── PGNReader.cpp/h             # PGN file reader
├── PGNWriter.cpp/h             # PGN file writer
├── pieces/
│   ├── Pawn.cpp/h
│   ├── Rook.cpp/h
│   ├── Knight.cpp/h
│   ├── Bishop.cpp/h
│   ├── Queen.cpp/h
│   └── King.cpp/h
├── exceptions/
│   └── ChessException.h
└── Makefile
```

## Examples

### Example Game
```
white > e4
black > e5
white > Nf3
black > Nc6
white > Bb5
black > a6
white > Ba4
black > Nf6
white > O-O
```

### Saving a Game
```
white > save mygame.pgn
```

### Loading a Game
From the main menu, select option 2 and enter the filename.

## Technical Details

### Architecture
- **Object-Oriented Design**: Each piece type is a derived class from the base `Piece` class
- **Smart Pointers**: Uses `shared_ptr` for automatic memory management
- **Move Generation**: Generates all possible moves, then filters for legal moves by checking if they leave the king in check

### Key Classes
- **ChessEngine**: Manages game state, move validation, and legal move generation
- **Board**: 8x8 grid of squares, manages piece placement
- **Move**: Represents a move with metadata (castling, en passant, promotion)
- **AlgebraicNotationParser**: Converts algebraic notation to moves

## Known Limitations

- No threefold repetition detection
- No fifty-move rule
- No time controls
- Command-line interface only (no GUI)

## Future Improvements

- Add AI opponent
- Implement draw by repetition
- Add game analysis features
- Create a graphical interface
- Add multiplayer over network

## License

This project is open source and available for educational purposes.

## Author

Mo-Lights Chess Engine - A C++ implementation of chess with full rules support.
