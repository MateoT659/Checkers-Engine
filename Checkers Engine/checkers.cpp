#include "Includes.h"
#include "Board.h"
SDL_Window* window;
SDL_Renderer* renderer;


	//at some point, filter movemaps to force jumps

	//move map: maps each piece that can move (key, char) to spots where it can move (value, char. maybe use unordered set for this one).
	//use for actual game, each board should be able to generate its own map.
	//for doubles, id like to make circles for each of the consecutive jumps (darker)

	//in game, use booleans for each side: "use best moves" automates best moves (HAVE WAITING)
	//also, have button on right "Get best move" which highlights the best move possible in the current board.
	//also, have the collected pieces on the sides.
	//have buttons on the side near the best move button that can change the depth of the tree (3 - 10?/15?)
	//include the button for whos turn it is 

	//current goals:
	//optimize (alpha-beta is first, then try these options)

/*
* optimize so when theres 1 move it just plays the goddamn move instead of looking into the future for no reason
* 
* find the complexity? also maybe some sets/maps can become unordered sets/maps so lookup is faster.
* make gametree logic iterative instead of recursive (recursion is slow because of the insane stack usage, this might also make alphabeta easier to do) 
* optimize getleafmovemap and the move pruning when there is a jump (maybe make getleafmovemap return a set of pairs?)
* optimize rating systems 
* 
*/


	//Garnish for after everything: (mostly text-based stuff)
	/* optimize the hell out of "Board::getMoveMap" and "Board::getLeafMoveMap"
	* on win, say who won.then, wait for "ENTER" to play again
	* have text displaying bot mode and status
	* display move number
	* "tie" button (agree to draw)
	*/


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

	char debugboard[32] =
	{
		0, 0, 0, 'R',
		0, 'b', 'B', 0,
		0, 0, 0, 0,
		0, 0, 'b', 'b',
		0, 0, 0, 0,
		0, 0, 0, 'b',
		0, 'r', 'R', 'B',
		0, 0, 0, 0
	};
	Board* currentboard = new Board();
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


						std::pair<char, QTreeNode*> pair = getBestMove(currentboard, 4);

						square = pair.first;
						usedMove = pair.second;

						//itearte through all moves and find the best
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
						std::pair<char, QTreeNode*> pair = getBestMove(currentboard, 6);

						square = pair.first;
						usedMove = pair.second;
					}
						break;
					case BOTHARD:
					{
						std::pair<char, QTreeNode*> pair = getBestMove(currentboard, 7); 
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
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void initSDL() {
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

	if (TTF_Init() == -1) {
		std::cout << "SDL_font init error: " << TTF_GetError() << "\n";
		exit(1);
	}

}
void freeMoveMap(std::map<char, std::set<QTreeNode*>> movemap){
	//deletes all of the pointers of tree nodes in the trees that are in the sets that are in the map. im going insane
	for (auto i = movemap.begin(); i != movemap.end(); i++) {
		for (QTreeNode* ptr : i->second) {
			delete ptr;
		}
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

std::set<QTreeNode*> getLeafSet(std::set<QTreeNode*> from) {
	//returns a set with the leaf nodes from a set with the root nodes.
	std::set<QTreeNode*> leafset;

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
			if (currsize == dfs.size()) leafset.insert(curr);
		}
	}

	return leafset;
}

Board* getBoardFromMove(Board* top, char square, QTreeNode* move) {
	//the same but no animation
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
	//start drawing arrow from piece to piece
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
	//first, check if the board is a win or a tie
	switch (board->isWinningBoardGen()) {
	case 'b':
		return -100;
	case 'r':
		return 100;
	}

	std::map<char, std::set<QTreeNode*>> moves = board->getLeafMoveMap();
	if (moves.empty()) return board->turn ? -100 : 100;
	if (depth <= 1) {
		return board->rating();
	}
	if (moves.empty()) return 0;

	int bestrating = board->turn ? -100:100;
	bool rated = false;

	Board* tempboard;
	int temprating;
	//iterate through all possible boards, run a max/min on their rating based on board->turn (true->max, false->min)
	for (auto i = moves.begin(); i != moves.end(); i++) {
		for (QTreeNode* treenode : i->second) {
			tempboard = getBoardFromMove(board, i->first, treenode);
			temprating = getRating(tempboard, depth-1, bestrating);
			delete tempboard;
			if (board->turn) {
				if (!rated || temprating > bestrating) {
					rated = true;
					bestrating = temprating;
					if (bestrating >= parentM) { //alpha-beta pruning 
						return bestrating;
					}
				}
			}
			else {
				if (!rated || temprating < bestrating) {
					rated = true;
					bestrating = temprating;
					if (bestrating <= parentM) { //alpha-beta pruning.
						return bestrating;
					}
				}
			}
		}
	}
	return bestrating;
}

std::pair<char, QTreeNode*> getBestMove(Board* board, int depth) {
	std::map<char, std::set<QTreeNode*>> moves = board->getLeafMoveMap(); 
	if (moves.size() == 1) { //this happens often when a jump is forced
		if (moves.begin()->second.size() == 1) {
			return std::pair<char, QTreeNode*>(moves.begin()->first, *(moves.begin()->second.begin()));
		}
	}
	std::pair<char, QTreeNode*> returnMove;
	int bestrating = board->turn?-100:100;
	bool rated = false;

	Board* tempboard;
	int temprating;
	//iterate through all possible boards, run a max/min on their rating based on board->turn (true->max, false->min)
	for (auto i = moves.begin(); i != moves.end(); i++) {
		for (QTreeNode* treenode : i->second) {
			tempboard = getBoardFromMove(board, i->first, treenode);
			temprating = getRating(tempboard, depth, bestrating);
			delete tempboard;
			if (board->turn) {
				if (!rated || temprating > bestrating) {
					rated = true;
					bestrating = temprating;
					returnMove.first = i->first;
					returnMove.second = treenode;
				}
			}
			else {
				if (!rated || temprating < bestrating) {
					rated = true;
					bestrating = temprating;
					returnMove.first = i->first;
					returnMove.second = treenode;
				}
			}
		}
	}

	return returnMove;
}