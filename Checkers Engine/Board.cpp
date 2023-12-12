#include "Board.h"

Board::Board()
{
	pieces = new char[32]{'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b', 0, 0, 0, 0, 0, 0, 0, 0, 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r'};
	turn = true;
	numBlackTaken = 0;
	numRedTaken = 0;
}

Board::Board(char* piece, bool turn, int bt, int rt)
{
	//piece should already be allocated - only use of this constructor is in Board::move()
	pieces = piece;
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
		//red's turn
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
					getJumpTree(tree->DL, true, newjumped);
				}
			}
			temp = getDR(tree->index);
			if (temp >= 0 && temp < 32 && (pieces[temp] == 'b' || pieces[temp] == 'B') && jumped.find(temp) == jumped.end()) {
				std::unordered_set<char> newjumped = jumped;
				newjumped.insert(temp);
				temp = getDR(temp);
				if (temp >= 0 && temp < 32 && pieces[temp] == 0) {

					tree->DR = new QTreeNode(temp, tree);
					getJumpTree(tree->DR, true, newjumped);
				}
			}
		}

	}
	else {
		//black's turn
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
					getJumpTree(tree->UL, true, newjumped);
				}
			}
			temp = getUR(tree->index);
			if (temp >= 0 && temp < 32 && (pieces[temp] == 'r' || pieces[temp] == 'R') && jumped.find(temp) == jumped.end()) {
				std::unordered_set<char> newjumped = jumped;
				newjumped.insert(temp);
				temp = getUR(temp);
				if (temp >= 0 && temp < 32 && pieces[temp] == 0) {

					tree->UR = new QTreeNode(temp, tree);
					getJumpTree(tree->UR, true, newjumped);
				}
			}
		}
	}

}

std::set<QTreeNode*> Board::getMoves(char ind, bool forcejump)
{
	//returns a set of 4-nary tree nodes representing move destinations for a piece.
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
	char jumppoint = 0;

	for (char i = 0; i < 32; i++) {
		if (pieces[i] == (turn ? 'r':'b') || pieces[i] == (turn ? 'R':'B')) {
			tempset = getMoves(i, forcedjump);
			if (!tempset.empty()) {
				movemap[i] = tempset;
				if (!forcedjump) {
					for (QTreeNode* jump : tempset) {
						if ((jump->index / 4) % 2 == (i / 4) % 2) {
							forcedjump = true;
							jumppoint = i+1;
							
							//other way to do this is just reset the movemap and reallocate everything with forcedjumps on.
							//freeMoveMap(movemap);
							//movemap.clear();
							//i = -1; //this is -1 because of i++! it would skip 0 otherwise
						}
					}
				}
			}
		}
	}
	std::vector<QTreeNode*> todelete;
	for (char i = 0; i < jumppoint; i++) {
		//prune out the non-jumps from the sets.
		if (movemap.find(i) != movemap.end()) {
			for (QTreeNode* jump : movemap[i]) {
				if ((jump->index / 4) % 2 != (i / 4) % 2) {
					todelete.push_back(jump);
				}
			}
			for (QTreeNode* jump : todelete) {
				movemap[i].erase(jump);
			}
			if (movemap[i].empty()) {
				movemap.erase(i);
			}
		}
	}

	return movemap;
}

std::map<char, std::set<QTreeNode*>> Board::getLeafMoveMap() 
{
	//usually, this doesnt do anything since branching jumps are rare, but it is neccessary in case it does happen.
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
	char* newboard = new char[32];
	int inddiv = move->index / 4;
	for (int i = 0; i < 32; i++) {
		newboard[i] = pieces[i];
	}
	
	newboard[move->index] = pieces[piece];
	newboard[piece] = 0;
	
	if (inddiv % 2 == (piece / 4) % 2 ){
		newboard[(move->index + piece+ ((inddiv%2==1)?-1:1))/2] = 0;
		takes = true;
	}

	if (inddiv == 0 && newboard[move->index] == 'r') {
		newboard[move->index] = 'R';
	}
	else if (inddiv == 7 && newboard[move->index] == 'b') {
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
	else if (red && !black) return 'r';
	
	return 0;
}

int Board::rating()
{
	//possibly the most important method in the entire program.
	//defines behavior of the AI
	//all rating functions rate king pieces as 5, and normal pieces as 3, then add sugar to determine a different "playstyle"


	if (currentTurn) {
		//red
		return ratingWeighted();
	}
	else {
		//black
		return ratingOffensive();
	}

	
}

int Board::ratingBasic()
{
	//rates solely on material advantage.
	int rating = 0;
	for (int i = 0; i < 32; i++) {
		switch (pieces[i]) {
		case 'R':
			rating += 5;
			break;
		case 'r':
			rating += 2;
			break;
		case 'B':
			rating -= 5;
			break;
		case 'b':
			rating -= 2;
			break;
		}
	}
	return rating;
}

int Board::ratingDefensive()
{
	//promotes keeping back rank pawns on the back rank

	int rating = 0;
	for (int i = 0; i < 32; i++) {
		switch (pieces[i]) {
		case 'R':
			rating += 5;
			break;
		case 'r':
			rating += 3;
			if (i > 27) rating++;
			break;
		case 'B':
			rating -= 5;
			break;
		case 'b':
			rating -= 3;
			if (i < 4) rating--;
			break;
		}
	}
	return rating;
}

int Board::ratingOffensive()
{
	//gives a small bonus for back-rank pieces and for pieces in the center of the board.
	int rating = 0;
	int i = 0;
	for (i; i < 7; i++) {
		switch (pieces[i]) {
		case 'R':
			rating += 5;
			break;
		case 'r':
			rating+=3;
			break;
		case 'B':
			rating -= 5;
			break;
		case 'b':
			rating-=3;
			if (i < 4) rating--;
			break;
		}
	}
	int mod = 0;
	bool bonus = false;
	for (i; i < 23; i++) {
		mod = i % 4;
		bonus = mod == 2 || mod == 3;
		switch (pieces[i]) {
		case 'R':
			rating += 5;
			if (bonus) rating++;
			break;
		case 'r':
			rating += 3;
			if (bonus) rating++;
			break;
		case 'B':
			rating -= 5;
			if (bonus) rating--;
			break;
		case 'b':
			rating -= 3;
			if (bonus) rating--;
			break;
		}
	}
	
	for (i; i < 32; i++) {
		switch (pieces[i]) {
		case 'R':
			rating += 5;
			break;
		case 'r':
			rating += 3;
			if (i > 27) rating++;
			break;
		case 'B':
			rating -= 5;
			break;
		case 'b':
			rating -= 3;
			break;
		}
	} 
	return rating;
}

int Board::ratingWeighted()
{
	//tends to rate positions more extremely when there are fewer pieces.
	//ex: a 2-1 advantage is better than a 10-9 advantage with this rater. likes to trade.
	
	int rating = 0;
	
	int numpieces = 0;
	for (int i = 0; i < 32; i++) {
		switch (pieces[i]) {
		case 'R':
			rating += 5;
			numpieces++;
			break;
		case 'r':
			rating += 3;
			if (i > 27) rating++;
			numpieces++;
			break;
		case 'B':
			rating -= 5;
			numpieces++;
			break;
		case 'b':
			rating -= 3;
			if (i < 4) rating--;
			numpieces++;
			break;
		}
	}
	return rating * (25 - numpieces);
}

void Board::print()
{
	//prints to console, solely for debugging
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
	//draws board to screen
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
