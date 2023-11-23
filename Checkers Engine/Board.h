#pragma once
#include "Includes.h"
class Board
{
public:
	char* pieces; // '' is empty 'b' is BLACK, 'B' is black (kinged), 'r' is red, 'R', is red (kinged)
	bool turn; //TRUE == red, FALSE == black;

	int numRedTaken; // the number of red pieces taken by black
	int numBlackTaken; //will be used in render to visually show amount of pieces taken, display on opposite sides)


	Board(); //default board
	Board(char* pieces, bool turn, int numBlackTaken, int numRedTaken); //custom boards
	~Board();// destructor

	char getTouched(Vec2 pos); //returns the index of the touched square. If not touched, returns 32


	void getJumpTree(QTreeNode *tree, bool kinged, std::unordered_set<char> jumped);

	std::set<QTreeNode*> getMoves(char ind, bool forceJumps); //returns set of possible places to go from ind
	std::map<char, std::set<QTreeNode*>> getMoveMap();

	std::map<char, std::set<QTreeNode*>> getLeafMoveMap();
	
	
	// returns full move map: keys represent indices of movable pieces, 
	//and values are the indices of locations to where (paths, because of double jumping)
	//vector<Board*> getChildren(); //returns all possible boards that can result from this position. 
	Board* move(char piece, QTreeNode* move); //returns board that results from this move. nullptr on failure

	char isWinningBoard(); //returns 'b' on black win, 'r' on red win, 0 on no win; uses numtaken
	char isWinningBoardGen(); //uses order n traversal for un-standard boards. mostly for debug

	int rating();
	
	void print(); //for debugging

	void render();
};

