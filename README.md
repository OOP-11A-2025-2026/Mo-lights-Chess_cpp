# Chess C++ Implementation

This is a C++ implementation of chess pieces and squares, ported from the Java implementation at [Mo-Lights-Chess](https://github.com/OOP-11A-2025-2026/Mo-Lights-Chess).

## Project Structure

```
Mo-lights-Chess_cpp/
├── Piece.h / Piece.cpp         # Abstract base class for all pieces
├── Square.h / Square.cpp       # Chess square representation
├── pieces/
│   ├── Pawn.h / Pawn.cpp      # Pawn piece implementation
│   ├── Rook.h / Rook.cpp      # Rook piece implementation
│   ├── Knight.h / Knight.cpp  # Knight piece implementation
│   ├── Bishop.h / Bishop.cpp  # Bishop piece implementation
│   ├── Queen.h / Queen.cpp    # Queen piece implementation
│   └── King.h / King.cpp      # King piece implementation
├── test_pieces.cpp             # Test program
├── Makefile                    # Build configuration
└── README.md                   # This file
```

## Features Implemented

### Square Class
- Position tracking (row, col from 0-7)
- Piece management (add, remove, check presence)
- Algebraic notation conversion (e.g., "e4", "a1")
- Light/dark square detection
- Display methods with ANSI color codes

### Piece Base Class (Abstract)
- Color management (white/black)
- Movement tracking (hasMoved flag)
- Pure virtual methods:
  - `getPossibleMoves(row, col)` - returns all theoretical moves
  - `getSymbol()` - returns Unicode chess symbol
- Helper methods:
  - `getType()` - returns piece type name
  - `getPieceLetter()` - returns algebraic notation letter (K, Q, R, B, N)
  - `getPieceStr()` - returns abbreviated notation (e.g., "wQ", "bR")

### Individual Piece Classes
All pieces inherit from `Piece` and implement:
- **Pawn**: Forward movement (1 or 2 squares from start), diagonal capture squares
- **Rook**: Horizontal and vertical movement (up to 7 squares)
- **Knight**: L-shaped movement (2+1 squares in any direction)
- **Bishop**: Diagonal movement (up to 7 squares)
- **Queen**: Combined rook and bishop movement (8 directions)
- **King**: One square in any direction (8 adjacent squares)

## Building and Running

### Compile the code:
```bash
make
```

### Run the test program:
```bash
make test
```

### Clean build files:
```bash
make clean
```

### Manual compilation:
```bash
g++ -std=c++17 -Wall -Wextra -g -c Piece.cpp Square.cpp pieces/*.cpp test_pieces.cpp
g++ -std=c++17 -Wall -Wextra -g -o test_pieces *.o pieces/*.o
./test_pieces
```

## Key Design Decisions

1. **Smart Pointers**: Uses `std::shared_ptr<Piece>` for automatic memory management
2. **Standard Library**: Utilizes `std::vector` for move lists and `std::string` for text
3. **Const Correctness**: Methods that don't modify state are marked `const`
4. **Exception Handling**: Validates input and throws `std::invalid_argument` for invalid data
5. **Unicode Support**: Uses Unicode chess symbols (♔♕♖♗♘♙) for display

## Differences from Java Implementation

- C++ uses smart pointers instead of garbage collection
- Header/implementation file split (.h/.cpp) instead of single .java files
- Manual memory management through RAII and smart pointers
- Different type system (no `List<int[]>`, uses `std::vector<std::vector<int>>`)
- `getType()` implementation uses `typeid` (can be overridden for clarity)

## Requirements

- C++17 or later
- g++ compiler (or compatible)
- Terminal with Unicode support for chess symbols

## Future Enhancements

To complete the chess implementation, you would need:
- Board class to manage the 8x8 grid
- Move validation (checking for piece blocking, captures, etc.)
- Game logic (turn management, check/checkmate detection)
- Special moves (castling, en passant, promotion)
- Move history and notation

## License

This is an educational project ported from the Java implementation.
