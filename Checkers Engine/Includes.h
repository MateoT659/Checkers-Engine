#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_set>
#include <map>
#include <set>
#include <vector>
#include "Vec2.h"
#include <stack>
#include <unordered_map>

const int SCREEN_HEIGHT = 600, SCREEN_WIDTH = 800;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

class Board;
//qtreenode struct for jumps
class QTreeNode
{
public:
	char index;
	QTreeNode* UR, * UL, * DR, * DL, *parent;
	QTreeNode(char ind) { index = ind;}
	QTreeNode(char ind, QTreeNode* parent) { index = ind; this->parent = parent; }
	~QTreeNode() {
		//"post order" destructor that recursively deletes the nodes from leaves to root
		if (UR) delete UR;
		if (UL) delete UL;
		if (DR) delete DR;
		if (DL) delete DL;
	}
};

typedef enum PlayerType {
	HUMAN, //Human input, moves pieces.
	BOTEASY, //best move finding engine - low depth. (depth = 4)
	BOTMID, //best move finding engine - mid depth. (depth = 8)
	BOTHARD //best move finding engine - high depth. (depth = 12)
}PlayerType;

const int TYPENUM = 4;

//potentially add more for different types: BOTRANDOM (chooses random move), BOTBAD (plays worst move?)

//this will help differentiate between playing with different engine depths and human players.

//colors
const SDL_Color BOARD_LIGHT = { 111, 78, 55 ,255 };
const SDL_Color BOARD_DARK = { 253, 214, 167, 255};
const SDL_Color SILVER = { 120,120,120,255 };
const SDL_Color DARK_GRAY = { 60, 60, 60,255 };
const SDL_Color BLACK = { 0, 0, 0, 255 };
const SDL_Color RED = { 194, 56, 47 , 255 };
const SDL_Color DARK_RED = { 120, 24, 24 , 255 };
const SDL_Color GOLD = { 219,172,52, 255 };
const SDL_Color WHITE = { 255,255,255,255 };
const SDL_Color GREEN = { 50,205,50 , 255 };

//main.cpp
void initSDL();
void freeMoveMap(std::map<char, std::set<QTreeNode*>> movemap);
std::unordered_map<char, QTreeNode*> getLeafMap(std::set<QTreeNode*> from);
std::set<QTreeNode*> getLeafSet(std::set<QTreeNode*> from);
Board* performMove(Board* original, char piece, QTreeNode* move);
Board* getBoardFromMove(Board* top, char square, QTreeNode* move);
int getRating(Board* board, int depth);
std::pair<char, QTreeNode*> getBestMove(Board* board, int depth);

//graphics.cpp
void drawCircle(Vec2 pos, int radius);
void drawCircle(int cx, int cy, int radius);
void drawCircle(int cx, int cy, int radius, SDL_Color color);
void drawCircle(Vec2 pos, int radius, SDL_Color color);
void drawOpenCircle(Vec2 pos, int radius);
void setRenderColor(SDL_Color color);
void setRenderColor(SDL_Color rgbColor, int alpha);
void setRenderColor(int r, int g, int b, int alpha);
void drawPoint(int x, int y);
void drawPoint(Vec2 pos);
void drawLine(int x1, int y1, int x2, int y2);
void drawLine(Vec2 point1, Vec2 point2);
void clearScreen(SDL_Color color);
void drawFilledRectangle(int x, int y, int w, int h, SDL_Color color);
void drawFilledRectangle(SDL_Rect rect, SDL_Color color);
void drawFilledRectangle(int x, int y, int w, int h);
void drawFilledRectangle(SDL_Rect rect);
void drawOpenRectangle(int x, int y, int w, int h, SDL_Color color);
void drawOpenRectangle(SDL_Rect rect, SDL_Color color);
void drawOpenRectangle(int x, int y, int w, int h);
void drawOpenRectangle(SDL_Rect rect);
bool colorEquals(SDL_Color color1, SDL_Color color2);
void drawCrown(int x, int y);
void drawCrown(int x, int y, SDL_Color color);
void drawCrown(Vec2 pos);
void drawCrown(Vec2 pos, SDL_Color color);
void highlightSquare(char square);
void highlightSquare(char square, SDL_Color color);
void shadowSquare(char square, bool small);
void drawPlayerType(int x, int y, PlayerType type);
void drawPausePlay(int x, int y, bool paused);
bool rectIsTouched(SDL_Rect rect, int x, int y);
bool rectIsTouched(SDL_Rect rect, Vec2 pos);
void drawArrow(Vec2 p1, Vec2 p2, SDL_Color color);
void drawArrow(int x1, int y1, int x2, int y2, SDL_Color color);

Vec2 getMousePos();
Vec2 getMousePos(SDL_Event event);
char getUR(char piece);
char getUL(char piece);
char getDR(char piece);
char getDL(char piece);
Vec2 getPosition(char piece);