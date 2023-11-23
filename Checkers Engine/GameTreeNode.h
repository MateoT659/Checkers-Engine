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

