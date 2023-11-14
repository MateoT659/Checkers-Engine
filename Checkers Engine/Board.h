#pragma once
#include "Includes.h"
class Board
{
public:
	char *pieces; // '' is empty 'w' is white, 'W' is white (kinged), 'r' is red, 'R', is red (kinged)
	bool turn; //TRUE == white, FALSE == red;

	Board(); //default board
	Board(char array[32], bool turn); //custom boards


	char getTouched(); //returns the index of the touched square. If not touched, returns 32

	//unordered_set<char> getMoves(char ind); //returns set of possible places to go from ind
	//map<char, char> getMoveMap(); // returns full move map: keys represent indices of movable pieces, and values are the indices of locations to where
	//vector<Board*> getChildren(); //returns all possible boards that can result from this position.
	Board* move(char from, char to); //returns board that results from this move. nullptr on failure

	int rating();
	
	void print(); //for debugging

	void render();
};

