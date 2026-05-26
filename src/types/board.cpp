#include "board.h"

Board::Board() {
	for (int i = 0; i < 64; i++) {
		squares[i] = 0;
	}

	squares[0] = Piece::Black | Piece::Knight;
}