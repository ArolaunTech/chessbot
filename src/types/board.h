#ifndef BOARD_H
#define BOARD_H

class Piece {
public:
	const static char None = 0;
	const static char Pawn = 1;
	const static char King = 2;
	const static char Knight = 3;
	const static char Bishop = 4;
	const static char Rook = 5;
	const static char Queen = 6;

	const static char Black = 0;
	const static char White = 8;
};

class Board {
private:
	char squares[64];
public:
	Board();
};

#endif