#include "Includes.h"
#include "Board.h"
SDL_Window* window;
SDL_Renderer* renderer;
bool currentTurn;
//main function located here. Also contains the core recursive functino "get rating" and "getBestMove", and other miscellaneous functions on sets.

int main(int argc, char* args[]) {
	SDL_Event event;
	bool running = true;
	bool play = true; //controls if the game can be played. used to stop two bots playing each other

	//determines which behavior the player will have (human input vs best moves)
	PlayerType blacktype = HUMAN; 
	PlayerType redtype = HUMAN; 

	//used to parse mouse events
	SDL_Rect blacktypebox = { 675, 75, 50, 40 };
	SDL_Rect redtypebox = { 675, 485, 50, 40 };
	SDL_Rect autoplaybox = { 750, 280, 40, 40 };

	char* debugboard = new char[32] {0,0,0,0,0,0,0,0,'b', 'b', 'b', 0, 0, 0, 0, 0, 'b', 'b', 'b', 0, 0, 0, 0, 0, 0, 'b', 'b', 0, 0, 0, 'R', 0};
	
	
	//Board* currentboard = new Board(debugboard, true, 0, 0);
	Board* currentboard = new Board();
	currentTurn = currentboard->turn;
	initSDL();
	char square = 32, temp = 32;
	std::map<char, std::set<QTreeNode*>> moves = currentboard->getMoveMap();
	std::unordered_map<char, QTreeNode*> leafMoves; //contains all of the playable moves of the current square (leaves of QTreeNodes)

	while (running) {
		
		while (SDL_PollEvent(&event)) {
			//input
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				Vec2 mousePos = getMousePos(event);
				if (rectIsTouched(autoplaybox, mousePos)) {
					play = !play;
				}
				else if (rectIsTouched(redtypebox, mousePos)) {
					play = false;
					redtype = (PlayerType)((redtype + 1) % TYPENUM);
				}
				else if (rectIsTouched(blacktypebox, mousePos)) {
					play = false;
					blacktype = (PlayerType)((blacktype + 1) % TYPENUM);
				}
				if (play) {
					QTreeNode* usedMove = nullptr; //this whole switch statement is about getting the move that will be done.
					//square will be the index of the piece that will be moving.
					switch (currentboard->turn ? redtype : blacktype) {
					case HUMAN:
						temp = currentboard->getTouched(mousePos);
						if (temp < 32 && temp >= 0 && square != temp ) {
							if (leafMoves.find(temp) != leafMoves.end()) {
								usedMove = leafMoves[temp];
							}
							else {
								square = temp;
								if (moves.find(square) != moves.end()) {
									leafMoves = getLeafMap(moves[square]);
								}
							}
						}
						else {
							square = 32;
							leafMoves.clear();
						}
						break;
					case BOTEASY:
					{
						std::pair<char, QTreeNode*> pair = getBestMove(currentboard, 5, createLeafMap(moves));

						square = pair.first;
						usedMove = pair.second;

						//iterate through all moves and find the best
						/*Board* temp;
						bool rated = false;
						int ratingmax = 0;
						for (auto sq = moves.begin(); sq != moves.end(); sq++) {
							leafMoves = getLeafMap(sq->second);
							for (auto iter = leafMoves.begin(); iter != leafMoves.end(); iter++) {						
								if (iter->second != nullptr) {
									temp = getBoardFromMove(currentboard, sq->first, iter->second);

									std::cout << "one\n";
									
									if (temp->rating() <= ratingmax || !rated) {
										rated = true;
										square = sq->first;
										ratingmax = temp->rating();
										usedMove = iter->second;
									}
									delete temp;
								}
							}
						}
						leafMoves.clear();*/
					}
						break;
					case BOTMID:
					{
						std::pair<char, QTreeNode*> pair = getBestMove(currentboard, 7, createLeafMap(moves));

						square = pair.first;
						usedMove = pair.second;
					}
						break;
					case BOTHARD:
					{
						std::pair<char, QTreeNode*> pair = getBestMove(currentboard, 9, createLeafMap(moves));
						
						//209,000 -> 5,000  function calls for 7

						square = pair.first;
						usedMove = pair.second;
					}
						break;
					}

					if (usedMove != nullptr) {
						Board* todelete = currentboard;
						currentboard = performMove(currentboard, square, usedMove);
						delete todelete;

						freeMoveMap(moves);
						moves = currentboard->getMoveMap();
						leafMoves.clear();
						square = 32;

						if (currentboard->isWinningBoardGen() || moves.size() == 0) {
							while (event.key.keysym.sym != SDLK_RETURN && event.type != SDL_QUIT) {
								SDL_PollEvent(&event);
							}
							if (event.type == SDL_QUIT) {
								SDL_Quit();
								exit(1);
							}
							delete currentboard;
							freeMoveMap(moves);
							currentboard = new Board();
							moves = currentboard->getMoveMap();
						}
					}

				}
				break;
			}

			//update

			//render
			clearScreen(BLACK);
			drawFilledRectangle(600, 582, 200, 18, currentboard->turn ? GOLD : DARK_GRAY);
			drawFilledRectangle(600, 0, 200, 18, currentboard->turn?DARK_GRAY:GOLD);
			currentboard->render();
			drawPlayerType(675, 75, blacktype);
			drawPlayerType(675, 485, redtype);
			drawPausePlay(750, 280, play);

			if (play) {
				for (auto iter = moves.begin(); iter != moves.end(); iter++) {
					highlightSquare(iter->first, GOLD);
				}
				if (moves.find(square) != moves.end()) {
					for (QTreeNode* move : moves[square]) {
						std::stack<QTreeNode*> dfs;
						dfs.push(move);
						QTreeNode* curr;
						while (!dfs.empty()) {
							curr = dfs.top(); dfs.pop();
							size_t currsize = dfs.size();
							if (curr->UL != nullptr) dfs.push(curr->UL);
							if (curr->UR != nullptr) dfs.push(curr->UR);
							if (curr->DL != nullptr) dfs.push(curr->DL);
							if (curr->DR != nullptr) dfs.push(curr->DR);

							shadowSquare(curr->index, currsize != dfs.size());
						}
					}
				}
				highlightSquare(square, WHITE);
			}

			SDL_RenderPresent(renderer);
			currentTurn = currentboard->turn;
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void initSDL() {
	//initiates SDL, opens window, creates renderer (SDL stuff, unrelated to game)
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "SDL_Init error: " << SDL_GetError << "\n";
		exit(1);
	}

	window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "SDL_CreateWindow error: " << SDL_GetError() << "\n";
		exit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}
void freeMoveMap(std::map<char, std::set<QTreeNode*>> movemap){
	//deletes all of the pointers of tree nodes in the trees that are in the sets that are in the map. im going insane
	for (auto i = movemap.begin(); i != movemap.end(); i++) {
		for (QTreeNode* ptr : i->second) {
			delete ptr;
		}
	}
}

void freeLeafMoveMap(std::map<char, std::set<QTreeNode*>> leafmovemap){
	//frees general map with leaf or root nodes. (goes back to the parent for every node)
	std::unordered_set<QTreeNode*> roots;
	for (auto i = leafmovemap.begin(); i != leafmovemap.end(); i++) {
		for (QTreeNode* ptr : i->second) {
			while (ptr->parent != nullptr) {
				ptr = ptr->parent;
			}
			roots.insert(ptr);
		}
	}
	for (QTreeNode* ptr : roots) {
		delete ptr;
	}
}

std::unordered_map<char, QTreeNode*> getLeafMap(std::set<QTreeNode*> from) {
	//returns a map where the char represents the index of QTreeNode* (for easy hashing)
	std::unordered_map<char, QTreeNode*> leafset;

	for (QTreeNode* node : from) {
		//dfs the node and add all the leaves to the leafset
		std::stack<QTreeNode*> dfs;
		QTreeNode* curr;
		dfs.push(node);
		size_t currsize; //used to determine if curr is leaf
		while (!dfs.empty()) {
			curr = dfs.top(); dfs.pop();
			currsize = dfs.size();
			if (curr->DL != nullptr) dfs.push(curr->DL);
			if (curr->UL != nullptr) dfs.push(curr->UL);
			if (curr->DR != nullptr) dfs.push(curr->DR);
			if (curr->UR != nullptr) dfs.push(curr->UR);
			
			//if the stack has the same number of elements as before, it means all of the children were null, meaning curr is a leaf.
			if (currsize == dfs.size()) leafset[curr->index] = curr;
		}
	}

	return leafset;
}

std::map<char, std::set<QTreeNode*>> createLeafMap(std::map<char, std::set<QTreeNode*>>from) {
	//takes map of sets of root nodes and expands them into their leaf nodes.
	std::map<char, std::set<QTreeNode*>> ret;
	for (auto iter = from.begin(); iter != from.end(); iter++) {
		ret[iter->first] = getLeafSet(iter->second);
	}
	return ret; //for easy use of leafmoves
}

std::set<QTreeNode*> getLeafSet(std::set<QTreeNode*> from) {
	//returns a set with the leaf nodes from a set with the root nodes.
	std::set<QTreeNode*> leafset;
	std::stack<QTreeNode*> dfs;
	QTreeNode* curr;
	size_t currsize; //used to determine if curr is leaf

	for (QTreeNode* node : from) {
		//dfs the node and add all the leaves to the leafset

		dfs.push(node);
		
		while (!dfs.empty()) {
			curr = dfs.top(); dfs.pop();
			currsize = dfs.size();
			if (curr->DL != nullptr) dfs.push(curr->DL);
			if (curr->UL != nullptr) dfs.push(curr->UL);
			if (curr->DR != nullptr) dfs.push(curr->DR);
			if (curr->UR != nullptr) dfs.push(curr->UR);
			
			//if the stack has the same number of elements as before, it means all of the children were null, meaning curr is a leaf.
			if (currsize == dfs.size()) leafset.insert(curr);
		}
	}

	return leafset;
}

Board* getBoardFromMove(Board* top, char square, QTreeNode* move) {
	//gets the board that would be created by performing this move onto the board.
	std::stack<QTreeNode*> moveSequence;
	QTreeNode* curr = move;

	while (curr != nullptr) {
		moveSequence.push(curr);
		curr = curr->parent;
	}
	Board* board = top;

	while (!moveSequence.empty()) {
		Board* todelete = board;
		board = board->move(square, moveSequence.top());
		if (todelete != top) delete todelete;
		square = moveSequence.top()->index;

		moveSequence.pop();
	}
	return board;
}

Board* performMove(Board* top, char square, QTreeNode* move) {
	//visually shows a move being made.
	std::stack<QTreeNode*> moveSequence; //this is a stack so they come out in reverse order due to LIFO

	QTreeNode* curr = move;
	Uint64 timer;
	while (curr != nullptr) {
		//parent chain is basically a linked list
		moveSequence.push(curr);
		curr = curr->parent;
	}
	Board* currentboard = top;
	while (!moveSequence.empty()) {
		//potential move clicked, do reverse stack moving to show process of chain-jumps and properly do them
		Board* todelete = currentboard;
		currentboard = currentboard->move(square, moveSequence.top());

		if (todelete != top) {
			delete todelete; //we dont want to delete currentboard actually
		}
		
		currentboard->render();
		drawArrow(getPosition(square), getPosition(moveSequence.top()->index), GOLD);
		SDL_RenderPresent(renderer);

		square = moveSequence.top()->index;
		moveSequence.pop();

		//half a second timer
		if (!moveSequence.empty()) {
			currentboard->turn = !currentboard->turn;
		}
		timer = SDL_GetTicks64();
		while (SDL_GetTicks64() - timer < 500);
	}

	currentboard->render();
	SDL_RenderPresent(renderer);
	return currentboard;
}

int getRating(Board* board, int depth, int parentM) {
	//recursively gets the rating of a board. if depth ==1, returns the static rating.
	switch (board->isWinningBoard()) {
	case 'b':
		return -100;
	case 'r':
		return 100;
	}
	if (depth <= 1) {
		//we dont want to generate a movemap for depth == 1: use O(1) static rating
		return board->rating();
	}

	std::map <char, std::set<QTreeNode*>> todelete = board->getMoveMap();
	std::map <char, std::set<QTreeNode*>> moves = createLeafMap(todelete); //uses the same ptrs, so we only have to free todelete
	if (moves.empty()) return 0;

	int bestrating = board->turn ? -101:101; 
	Board* tempboard;
	int temprating;
	//iterate through all possible boards, run a max/min on their rating based on board->turn (true->max, false->min)
	for (auto i = moves.begin(); i != moves.end(); i++) {
		for (QTreeNode* treenode : i->second) {
			tempboard = getBoardFromMove(board, i->first, treenode);
			temprating = getRating(tempboard, depth-1, bestrating);
			delete tempboard;
			if (board->turn) {
				if (temprating > bestrating) {
					bestrating = temprating;
					if (bestrating >= parentM) { //alpha-beta pruning 
						freeMoveMap(todelete);
						return bestrating;
					}
				}
			}
			else {
				if (temprating < bestrating) {
					bestrating = temprating;
					if (bestrating <= parentM) { //alpha-beta pruning.
						freeMoveMap(todelete);
						return bestrating;
					}
				}
			}
		}
	}
	freeMoveMap(todelete);
	return bestrating;
}

std::pair<char, QTreeNode*> getBestMove(Board* board, int depth, std::map<char,std::set<QTreeNode*>> moves) {
	//gets the bst move from the moves available in moves

	if (moves.size() == 1) { //this happens often when a jump is forced: allows us to skip the recursion and choose the only move - saves time.
		if (moves.begin()->second.size() == 1) {
			return std::pair<char, QTreeNode*>(moves.begin()->first, *(moves.begin()->second.begin()));
		}
	}
	std::pair<char, QTreeNode*> returnMove;
	int bestrating = board->turn?-101:101;

	Board* tempboard;
	int temprating;
	//iterate through all possible boards, run a max/min on their rating based on board->turn (true->max, false->min)
	for (auto i = moves.begin(); i != moves.end(); i++) {
		for (QTreeNode* treenode : i->second) {
			tempboard = getBoardFromMove(board, i->first, treenode);
			temprating = getRating(tempboard, depth, bestrating);

			if (board->turn) {
				if (temprating > bestrating) {
					bestrating = temprating;
					returnMove.first = i->first;
					returnMove.second = treenode;
				}
			}
			else {
				if (temprating < bestrating) {
					bestrating = temprating;
					returnMove.first = i->first;
					returnMove.second = treenode;
				}
			}
		}
	}
	return returnMove;
}