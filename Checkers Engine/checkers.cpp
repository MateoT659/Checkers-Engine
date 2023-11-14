#include "Includes.h"
#include "Board.h"
int main() {
	
	Board* board = new Board();

	//move map: maps each piece that can move (key, char) to spots where it can move (value, char. maybe use unordered set for this one).
	//use for actual game, each board should be able to generate its own map.
	//for doubles, id like to make circles for each of the consecutive jumps (darker)

	//in game, use booleans for each side: "use best moves" automates best moves (HAVE WAITING)
	//also, have button on left, "Get best move" which highlights the best move possible in the current board.
	//also, have the collected pieces on the sides.
	//have buttons on the side near the best move button that can change the depth of the tree (3-10?-15?)

	//on win, say who won. then, wait for "ENTER" to play again

	return 0;
}