#pragma once
#include "Includes.h"

//class that represents a checkers board (encodes location of pieces, number of taken pieces, the current turn)

class Board
{
public:
	char* pieces; // '\0' is empty 'b' is BLACK, 'B' is black (kinged), 'r' is red, 'R', is red (kinged)
	bool turn; //TRUE == red's turned, FALSE == black;

	int numRedTaken; // the number of red pieces taken by black
	int numBlackTaken; // used to determine if board is won (once 12 pieces are taken, the other wins)


	Board(); //default board
	Board(char* pieces, bool turn, int numBlackTaken, int numRedTaken); //custom boards
	~Board();// destructor

	char getTouched(Vec2 pos); 
	//returns the index of the touched square (for user input). If not touched, returns 32 (out of bounds)

	void getJumpTree(QTreeNode *tree, bool kinged, std::unordered_set<char> jumped);
	//for checkers that can jump multiple times, this changes *tree to add second or third jumps as children.

	std::set<QTreeNode*> getMoves(char ind, bool forceJumps); 
	//returns set of possible moves (treenodes) for the piece at ind.
	
	std::map<char, std::set<QTreeNode*>> getMoveMap();
	//returns a map from the pieces that can move to their set of possible moves.

	std::map<char, std::set<QTreeNode*>> getLeafMoveMap();
	//returns a map from the pieces that can move to their set of possible moves. if any moves are double jumps, all children are put in set.
	
	Board* move(char piece, QTreeNode* move);
	//returns board that results from this move. nullptr on failure

	char isWinningBoard(); //returns 'b' on black win, 'r' on red win, 0 on no win; uses numtaken
	char isWinningBoardGen(); //scans board to see # of remaining pieces for non-standard boards. mostly for debugging

	int rating();

	int ratingBasic();
	int ratingDefensive();
	int ratingOffensive();
	int ratingWeighted();
	//returns an evaulation of the board. defines the "behavior" of the "AI".
	
	void print(); 
	//for debugging

	void render();
	//renders the board onto the screen

	

};

