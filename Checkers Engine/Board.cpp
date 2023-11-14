#include "Board.h"

Board::Board()
{
	pieces = new char[32]{'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 0, 0, 0, 0, 0, 0, 0, 0, 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'};
	turn = true;
}

Board::Board(char piece[32], bool turn)
{
	this->pieces = piece;
	this->turn = turn;
}

void Board::print()
{
	for (int i = 0; i < 32; i++) {
		if ((i / 4) % 2 == 0) {
			std::cout << " ";
		}
		std::cout << pieces[i];
		if ((i / 4) % 2 == 1) {
			std::cout << " ";
		}
		if (i % 4 == 3) {
			std::cout << "\n";
		}
	}
}
