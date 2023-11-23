#include "Board.h"

Board::Board()
{
	pieces = new char[32]{'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 0, 0, 0, 0, 0, 0, 0, 0, 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r'};
	turn = true;
	numBlackTaken = 0;
	numRedTaken = 0;
}

Board::Board(char* piece, bool turn, int bt, int rt)
{ //piece should already be initialzied
	pieces = new char[32];
	for (int i = 0; i < 32; i++)
		pieces[i] = piece[i];
	this->turn = turn;
	numBlackTaken = bt;
	numRedTaken = rt;
}

Board::~Board()
{
	delete[] pieces;
}

char Board::getTouched(Vec2 pos)
{
	if (pos.x > 600) {
		return 32;
	}
	int x = (int)pos.x, y = (int)pos.y;
	x /= 75; y /= 75;
	if ((x + y) % 2 == 0) return 32;

	x /= 2;

	return y * 4 + x;
}




void Board::getJumpTree(QTreeNode *tree, bool kinged, std::unordered_set<char> jumped)
{
	//check all children, recursively call :(
	char temp = 0;
	if (turn == true) {
		temp = getUL(tree->index);
		if (temp >= 0 && temp < 32 && (pieces[temp] == 'b' || pieces[temp] == 'B') && jumped.find(temp) == jumped.end()) {
			std::unordered_set<char> newjumped = jumped;
			newjumped.insert(temp);
			temp = getUL(temp);
			if (temp >= 0 && temp < 32 && pieces[temp] == 0) {
				tree->UL = new QTreeNode(temp, tree);
				getJumpTree(tree->UL, kinged || temp / 4 == 0, newjumped);
			}
		}
		temp = getUR(tree->index);
		if (temp >= 0 && temp < 32 && (pieces[temp] == 'b' || pieces[temp] == 'B') && jumped.find(temp) == jumped.end()) {
			std::unordered_set<char> newjumped = jumped;
			newjumped.insert(temp);
			temp = getUR(temp);
			if (temp >= 0 && temp < 32 && pieces[temp] == 0) {

				tree->UR = new QTreeNode(temp, tree);
				getJumpTree(tree->UR, kinged || temp / 4 == 0, newjumped);
			}
		}

		if (kinged) {
			temp = getDL(tree->index);
			if (temp >= 0 && temp < 32 && (pieces[temp] == 'b' || pieces[temp] == 'B') && jumped.find(temp) == jumped.end()) {
				std::unordered_set<char> newjumped = jumped;
				newjumped.insert(temp);
				temp = getDL(temp);
				if (temp >= 0 && temp < 32 && pieces[temp] == 0) {

					tree->DL = new QTreeNode(temp, tree);
					getJumpTree(tree->DL, kinged || temp / 4 == 0, newjumped);
				}
			}
			temp = getDR(tree->index);
			if (temp >= 0 && temp < 32 && (pieces[temp] == 'b' || pieces[temp] == 'B') && jumped.find(temp) == jumped.end()) {
				std::unordered_set<char> newjumped = jumped;
				newjumped.insert(temp);
				temp = getDR(temp);
				if (temp >= 0 && temp < 32 && pieces[temp] == 0) {

					tree->DR = new QTreeNode(temp, tree);
					getJumpTree(tree->DR, kinged || temp / 4 == 0, newjumped);
				}
			}
		}

	}
	else {
		//black turn
		temp = getDL(tree->index);
		if (temp >= 0 && temp < 32 && (pieces[temp] == 'r' || pieces[temp] == 'R') && jumped.find(temp) == jumped.end()) {
			std::unordered_set<char> newjumped = jumped;
			newjumped.insert(temp);
			temp = getDL(temp);
			if (temp >= 0 && temp < 32 && pieces[temp] == 0) {
				tree->DL = new QTreeNode(temp, tree);
				getJumpTree(tree->DL, kinged || temp / 4 == 7, newjumped);
			}
		}
		temp = getDR(tree->index);
		if (temp >= 0 && temp < 32 && (pieces[temp] == 'r' || pieces[temp] == 'R') && jumped.find(temp) == jumped.end()) {
			std::unordered_set<char> newjumped = jumped;
			newjumped.insert(temp);
			temp = getDR(temp);
			if (temp >= 0 && temp < 32 && pieces[temp] == 0) {

				tree->DR = new QTreeNode(temp, tree);
				getJumpTree(tree->DR, kinged || temp / 4 == 7, newjumped);
			}
		}

		if (kinged) {
			temp = getUL(tree->index);
			if (temp >= 0 && temp < 32 && (pieces[temp] == 'r' || pieces[temp] == 'R') && jumped.find(temp) == jumped.end()) {
				std::unordered_set<char> newjumped = jumped;
				newjumped.insert(temp);
				temp = getUL(temp);
				if (temp >= 0 && temp < 32 && pieces[temp] == 0) {

					tree->UL = new QTreeNode(temp, tree);
					getJumpTree(tree->UL, kinged || temp / 4 == 7, newjumped);
				}
			}
			temp = getUR(tree->index);
			if (temp >= 0 && temp < 32 && (pieces[temp] == 'r' || pieces[temp] == 'R') && jumped.find(temp) == jumped.end()) {
				std::unordered_set<char> newjumped = jumped;
				newjumped.insert(temp);
				temp = getUR(temp);
				if (temp >= 0 && temp < 32 && pieces[temp] == 0) {

					tree->UR = new QTreeNode(temp, tree);
					getJumpTree(tree->UR, kinged || temp / 4 == 7, newjumped);
				}
			}
		}
	}

}

std::set<QTreeNode*> Board::getMoves(char ind, bool forcejump)
{
	//each vector represents a move (you can chain jumps in this game so you need the path to know which pieces to get rid of
	/*
	* unordered_set is not used here because hashing a vector would be order n in the length of the path,
	* but finding a vector in a set is logn where n is the number of paths possible. on average this is just as fast.
	* im using an array-based quadtree for paths that include a string of jumps. 
	*/
	std::set<QTreeNode*> moves;
	if (ind < 0 || ind >= 32) return moves; //no moves for something that doesnt exist
	char temp = 0;
	if (turn == true) {
		//reds turn
		switch (pieces[ind]) {
		case 'R':
			temp = getDL(ind);
			if (temp >= 0 && temp < 32) {
				switch (pieces[temp]) {
				case 'r':
				case 'R':
					//cant jump yourself
					break;
				case 'b':
				case 'B':
					//jump
					temp = getDL(temp);
					if (pieces[temp] == 0) {
						QTreeNode* tree = new QTreeNode(temp);
						getJumpTree(tree, true, std::unordered_set<char>());
						moves.insert(tree);
					}
					break;
				default:
					if(!forcejump)
						moves.insert(new QTreeNode(temp));
					break;
				}
			}
			temp = getDR(ind);
			if (temp >= 0 && temp < 32) {
				switch (pieces[temp]) {
				case 'r':
				case 'R':
					//cant jump yourself
					break;
				case 'b':
				case 'B':
					//jump
					temp = getDR(temp);
					if (pieces[temp] == 0) {
						QTreeNode* tree = new QTreeNode(temp);
						getJumpTree(tree, true, std::unordered_set<char>());
						moves.insert(tree);
					}
					break;
				default:
					if (!forcejump)
					moves.insert(new QTreeNode(temp));
					break;
				}
			}

			//no break here because r is a subset of R (a kinged piece always has the moves that its normal counterpart has)
		case 'r':
			temp = getUL(ind);
			if (temp >= 0 && temp < 32) {
				switch (pieces[temp]) {
				case 'r':
				case 'R':
					//cant jump yourself
					break;
				case 'b':
				case 'B':
					//jump
					temp = getUL(temp);
					if (pieces[temp] == 0) {
						QTreeNode *tree = new QTreeNode(temp);
						getJumpTree(tree, pieces[ind] == 'R' || temp/4 == 0, std::unordered_set<char>());
						moves.insert(tree);
					}
					break;
				default:
					if (!forcejump)
					moves.insert(new QTreeNode(temp));
					break;
				}
			}
			temp = getUR(ind);
			if (temp >= 0 && temp < 32) {
				switch (pieces[temp]) {
				case 'r':
				case 'R':
					//cant jump yourself
					break;
				case 'b':
				case 'B':
					//jump
					temp = getUR(temp);
					if (pieces[temp] == 0) {
						QTreeNode* tree = new QTreeNode(temp);
						getJumpTree(tree, pieces[ind] == 'R' || temp/4 == 0, std::unordered_set<char>());
						moves.insert(tree);
					}
					break;
				default:
					if (!forcejump)
					moves.insert(new QTreeNode(temp));
					break;
				}
			}
			break;
		}
	}
	else {
		switch (pieces[ind]) {
		case 'B':
			temp = getUL(ind);
			if (temp >= 0 && temp < 32) {
				switch (pieces[temp]) {
				case 'b':
				case 'B':
					//cant jump yourself
					break;
				case 'r':
				case 'R':
					//jump
					temp = getUL(temp);
					if (pieces[temp] == 0) {
						QTreeNode* tree = new QTreeNode(temp);
						getJumpTree(tree, true, std::unordered_set<char>());
						moves.insert(tree);
					}
					break;
				default:
					if (!forcejump)
					moves.insert(new QTreeNode(temp));
					break;
				}
			}
			temp = getUR(ind);
			if (temp >= 0 && temp < 32) {
				switch (pieces[temp]) {
				case 'b':
				case 'B':
					//cant jump yourself
					break;
				case 'r':
				case 'R':
					//jump
					temp = getUR(temp);
					if (pieces[temp] == 0) {
						QTreeNode* tree = new QTreeNode(temp);
						getJumpTree(tree, true, std::unordered_set<char>());
						moves.insert(tree);
					}
					break;
				default:
					if (!forcejump)
					moves.insert(new QTreeNode(temp));
					break;
				}
			}

			//no break here because r is a subset of R (a kinged piece always has the moves that its normal counterpart has)
		case 'b':
			temp = getDL(ind);
			if (temp >= 0 && temp < 32) {
				switch (pieces[temp]) {
				case 'b':
				case 'B':
					//cant jump yourself
					break;
				case 'r':
				case 'R':
					//jump
					temp = getDL(temp);
					if (pieces[temp] == 0) {
						QTreeNode* tree = new QTreeNode(temp);
						getJumpTree(tree, pieces[ind] == 'B' || temp/4 == 7, std::unordered_set<char>());
						moves.insert(tree);
					}
					break;
				default:
					if (!forcejump)
					moves.insert(new QTreeNode(temp));
					break;
				}
			}
			temp = getDR(ind);
			if (temp >= 0 && temp < 32) {
				switch (pieces[temp]) {
				case 'b':
				case 'B':
					//cant jump yourself
					break;
				case 'r':
				case 'R':
					//jump
					temp = getDR(temp);
					if (pieces[temp] == 0) {
						QTreeNode* tree = new QTreeNode(temp);
						getJumpTree(tree, pieces[ind] == 'B' || temp/4 == 7, std::unordered_set<char>());
						moves.insert(tree);
					}
					break;
				default:
					if (!forcejump)
					moves.insert(new QTreeNode(temp));
					break;
				}
			}
			break;
		}
	}

	return moves;
}

std::map<char, std::set<QTreeNode*>> Board::getMoveMap()
{
	std::map<char, std::set<QTreeNode*>> movemap;

	std::set<QTreeNode*> tempset;
	bool forcedjump = false; //if there is a jump/are multiple jumps available, they MUST be taken. therefore we prune the single hops.


	for (char i = 0; i < 32; i++) {
		if (pieces[i] == (turn ? 'r':'b') || pieces[i] == (turn ? 'R':'B')) {
			tempset = getMoves(i, forcedjump);
			if (!tempset.empty()) {
				movemap[i] = tempset;
				if (!forcedjump) {
					for (QTreeNode* jump : tempset) {
						if ((jump->index / 4) % 2 == (i / 4) % 2) {
							forcedjump = true;
							freeMoveMap(movemap);
							movemap.clear();
							i = -1; //this is -1 because of i++! it would skip 0 otherwise

							//reset with forced jumps (this is so wildy inefficient i am so mad that i cant think of how to make this better)
							//maybe save the index where its found so i postprocess if forcedjump and then do the thing. worst case its still trash but who knows.
							//this really isnt slow and does its job but maybe try it anyways for the experience. 
						}
					}
				}


			}
		}
	}

	return movemap;
}

std::map<char, std::set<QTreeNode*>> Board::getLeafMoveMap() 
{
	std::map<char, std::set<QTreeNode*>> movemap = getMoveMap();

	for (auto iter = movemap.begin(); iter != movemap.end(); iter++) {
		movemap[iter->first] = getLeafSet(iter->second);
	}

	return movemap;
}

Board* Board::move(char piece, QTreeNode* move)
{
	//returns next board based on move. in use, recursively from the base to the root.
	bool takes = false;
	char newboard[32];

	for (int i = 0; i < 32; i++) {
		newboard[i] = pieces[i];
	}
	
	newboard[move->index] = pieces[piece];
	newboard[piece] = 0;
	
	if ((move->index / 4) % 2 == (piece / 4) % 2 ){
		
		newboard[(move->index + piece+ (((move->index/4)%2==1)?-1:1))/2] = 0;
		takes = true;
	}
	if (move->index / 4 == 0 && newboard[move->index] == 'r') {
		newboard[move->index] = 'R';
	}
	if (move->index / 4 == 7 && newboard[move->index] == 'b') {
		newboard[move->index] = 'B';
	}
	return new Board(newboard, !this->turn, numBlackTaken + ((takes && turn)?1:0), numRedTaken + ((takes && !turn) ? 1 : 0));
}

char Board::isWinningBoard()
{
	if (numRedTaken >= 12 && numBlackTaken < 12) {
		return 'b';
	}
	if (numRedTaken < 12 && numBlackTaken >= 12) {
		return 'r';
	}
	return 0; //returns 0 also when both have no pieces
}

char Board::isWinningBoardGen()
{
	bool red = false, black = false;
	for (int i = 0; i < 32; i++) {
		red = red || pieces[i] == 'r' || pieces[i] == 'R';
		black = black || pieces[i] == 'b' || pieces[i] == 'B';
	}
	if (black && !red) return 'b';
	if (red && !black) return 'r';
	
	return 0;
}

int Board::rating()
{
	int rating = 0;
	//positive rating for red, negative rating for black.
	for (int i = 0; i < 32; i++) {
		switch (pieces[i]) {
		case 'R':
			rating += 3;
			break;
		case 'r':
			rating++;
			if (i > 27) rating++;
			break;
		case 'B':
			rating -= 3;
			break;
		case 'b':
			rating--;
			if (i < 4) rating--;
			break;
		}
	}
	return rating;
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

void Board::render()
{
	//600x600 square, render circles, blah blah
	drawFilledRectangle(0, 0, 600, 600, BOARD_LIGHT);
	for (int i = 0; i < 32; i++) {
		drawFilledRectangle(((i/4)%2 != 0 ? 75:0 )+ 150*(i%4),(i/4)*75, 75, 75, BOARD_DARK);
		switch (pieces[i]) {
		case 'b':
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 30, DARK_GRAY);
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 25, BLACK);
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 20, DARK_GRAY);
			break;
		case 'B':
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 30, DARK_GRAY);
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 25, BLACK);
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 20, DARK_GRAY);
			drawCrown(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 45, BLACK);
			break;
		case 'r':
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 30, RED);
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 25, DARK_RED);
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 20, RED);
			break;
		case 'R':
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 30, RED);
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 25, DARK_RED);
			drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 20, RED);
			drawCrown(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 45, DARK_RED);
			break;
		default:
			break;
		}

		for (int i = 0; i < numRedTaken; i++) {
			drawCircle(622 + i * 14, 20, 18, RED);
			drawCircle(622 + i * 14, 20, 15, DARK_RED);
			drawCircle(622 + i * 14, 20, 12, RED);
		}
		for (int i = 0; i < numBlackTaken; i++) {
			drawCircle(622 + i * 14, 578, 18, DARK_GRAY);
			drawCircle(622 + i * 14, 578, 15, BLACK);
			drawCircle(622 + i * 14, 578, 12, DARK_GRAY);
		}
		//render the current turn in the main (sprinkles)

	}

}
