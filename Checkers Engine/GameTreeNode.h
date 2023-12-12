#pragma once
#include "Includes.h"
#include "Board.h"

class GameTreeNode
{
public:
	Board* board;
	std::vector<Board*> children;
	int rating;

	GameTreeNode(Board* board) {
		this->board = board;
	}
};

//this class is not actually used at all but I used this "class" as a basis for conceptual graph tree stuff when I was figuring out the algorithm,
//kept it to show thought process/ how I view the game tree conceptually