#include "Includes.h"

//many helper functions for SDL rendering and some for indexing.

void setRenderColor(SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
void setRenderColor(SDL_Color color, int a) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, a);
}
void setRenderColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void clearScreen(SDL_Color color) {
	setRenderColor(color);
	SDL_RenderClear(renderer);
}

void drawPoint(int x, int y) {
	SDL_RenderDrawPoint(renderer, x, y);
}
void drawPoint(Vec2 pos) {
	SDL_RenderDrawPoint(renderer, (int)pos.x, (int)pos.y);
}

void drawLine(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
void drawLine(Vec2 point1, Vec2 point2) {
	SDL_RenderDrawLine(renderer, (int)point1.x, (int)point1.y, (int)point2.x, (int)point2.y);
}

void drawOpenRectangle(int x, int y, int w, int h, SDL_Color color) {
	drawOpenRectangle({ x, y, w, h }, color);
}
void drawOpenRectangle(int x, int y, int w, int h) {
	drawOpenRectangle({ x, y, w, h });
}
void drawOpenRectangle(SDL_Rect rect, SDL_Color color) {
	setRenderColor(color);
	drawOpenRectangle(rect);
}
void drawOpenRectangle(SDL_Rect rect) {
	if (rect.w < 0) {
		rect.x += rect.w;
		rect.w *= -1;
	}
	if (rect.h < 0) {
		rect.y += rect.h;
		rect.h *= -1;
	}
	SDL_RenderDrawRect(renderer, &rect);
}

void drawFilledRectangle(SDL_Rect rect, SDL_Color color) {
	setRenderColor(color);
	drawFilledRectangle(rect);
}
void drawFilledRectangle(SDL_Rect rect) {

	if (rect.w < 0) {
		rect.x += rect.w;
		rect.w *= -1;
	}
	if (rect.h < 0) {
		rect.y += rect.h;
		rect.h *= -1;
	}

	while ((rect.w > 0 && rect.h > 0)) {
		SDL_RenderDrawRect(renderer, &rect);
		rect.x++;
		rect.y++;
		rect.h -= 2;
		rect.w -= 2;
	}
}
void drawFilledRectangle(int x, int y, int w, int h, SDL_Color color) {
	drawFilledRectangle({ x, y, w, h }, color);
}
void drawFilledRectangle(int x, int y, int w, int h) {
	drawFilledRectangle({ x, y, w, h });
}

void drawArrow(Vec2 p1, Vec2 p2, SDL_Color color) {
	drawArrow(p1.x, p1.y, p2.x, p2.y, color);
}

void drawArrow(int x1, int y1, int x2, int y2, SDL_Color color) {
	setRenderColor(color);

	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

	double dx = x2 - x1;
	double dy = y2 - y1;
	double tx = x2, ty = y2;
	double mag = sqrt(dx * dx + dy * dy);
	dx /= mag; dy /= mag;

	for (int i = 10; i > 0; i--) {
		SDL_RenderDrawLine(renderer, (int)(tx - i * dx - i * dy),
			(int)(ty - i * dy + i * dx),
			(int)(tx - i * dx + i * dy),
			(int)(ty - i * dy - i * dx));
	}
}

void drawCircle(Vec2 pos, int radius) {
	drawCircle((int)pos.x, (int)pos.y, radius);
}

void drawOpenCircle(Vec2 pos, int radius) {
	int x = (int)pos.x, y = (int)pos.y;
	int dx = radius, dy = 0;

	while (dx >= dy) {
		drawPoint(x + dx, y + dy);
		drawPoint(x + dy, y + dx);
		drawPoint(x - dx, y + dy);
		drawPoint(x - dy, y + dx);
		drawPoint(x + dx, y - dy);
		drawPoint(x + dy, y - dx);
		drawPoint(x - dx, y - dy);
		drawPoint(x - dy, y - dx);
		dy++;
		while (radius * radius < dx * dx + dy * dy) {
			dx--;
		}
	}
}

void drawCircle(int cx, int cy, int radius) {

	int x = cx, y = cy;
	int dx = radius, dy = 0, prevdx = -1;

	while (dx >= dy) {
		drawLine(x + dx, y + dy, x - dx, y + dy);
		if (dy != 0)
			drawLine(x + dx, y - dy, x - dx, y - dy);

		if (prevdx != dx && dy > 0 && dy != dx) {
			drawLine(x + dy, y + dx, x - dy, y + dx);
			drawLine(x + dy, y - dx, x - dy, y - dx);
			prevdx = dx;
		}
		dy++;
		while (radius * radius < dx * dx + dy * dy) {
			dx--;
		}

	}
}

void drawCircle(int cx, int cy, int radius, SDL_Color color)
{
	setRenderColor(color);
	drawCircle(cx, cy, radius);
}

void drawCircle(Vec2 pos, int radius, SDL_Color color) {
	setRenderColor(color); 
	drawCircle((int)pos.x, (int)pos.y, radius);
}

bool colorEquals(SDL_Color c1, SDL_Color c2) {
	return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a;
}

void drawCrown(int x, int y) {
	//where x and y are the center of the base
	//this is not what i wanted but it looks cool so ima keep it
	drawFilledRectangle(x - 12, y - 10, 25, 10);
	for (int i = 0; i <=5; i++) {
		drawLine(x - 12, y - 16 + i, x - 12 + i, y - 16 + i);
	}
	for (int i = 0; i <= 7; i++) {
		drawLine(x - 7 + i, y - 10 - i, x + 7 - i, y - 10 - i);
	}
	for (int i = 0; i <= 5; i++) {
		drawLine(x + 12, y - 16 + i, x +12- i, y - 16 + i);
	}
}

void drawCrown(int x, int y, SDL_Color color) {
	setRenderColor(color);
	drawCrown(x, y);
}

void drawCrown(Vec2 pos) {
	drawCrown((int)pos.x, (int)pos.y);
}

void drawCrown(Vec2 pos, SDL_Color color) {
	setRenderColor(color);
	drawCrown((int)pos.x, (int)pos.y);
}

Vec2 getMousePos() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return Vec2(x, y);
}

Vec2 getMousePos(SDL_Event event) {
	return Vec2(event.button.x, event.button.y);
	
}

void highlightSquare(char square) {
	if (square < 0 || square>35) return;
	//drawOpenRectangle(((square / 4) % 2 != 0 ? 0 : 75) + 150 * (square % 4)+1, (square / 4) * 75+1, 73, 73);
	drawOpenRectangle(((square / 4) % 2 != 0 ? 0 : 75) + 150 * (square % 4) + 2, (square / 4) * 75 + 2, 71, 71);
	drawOpenRectangle(((square / 4) % 2 != 0 ? 0 : 75) + 150 * (square % 4) + 3, (square / 4) * 75 + 3, 69, 69);
}

void highlightSquare(char square, SDL_Color color) {
	setRenderColor(color);
	highlightSquare(square);
}

void shadowSquare(char i, bool small) {
	if(small)
		drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 10, {60, 60, 60, 100});
	else
		drawCircle(((i / 4) % 2 != 0 ? 0 : 75) + 150 * (i % 4) + 37, (i / 4) * 75 + 37, 18, {60, 60, 60, 150});
}


char getUR(char piece) {
	if ((piece / 4)%2 == 0) {
		return piece/4 == (piece-3)/4 ? 32:piece - 3;
	}
	else {
		return piece - 4;
	}
}
char getUL(char piece) {
	if ((piece / 4) % 2 == 1) {
		return (piece/4)%2 == ((piece -5) / 4)%2 ? 32 : piece - 5;
	}
	else {
		return piece - 4;
	}
}
char getDR(char piece) {
	//inverse of getUL
	if ((piece / 4) % 2 == 0) {
		return (piece / 4) % 2 == ((piece + 5) / 4) % 2 ? 32 : piece + 5;
	}
	else {
		return piece + 4;
	}
}
char getDL(char piece) {
	//inverse of getUR
	if ((piece / 4) % 2 == 1) {
		return piece / 4 == (piece + 3) / 4 ? 32 : piece + 3;
	}
	else {
		return piece + 4;
	}
}

Vec2 getPosition(char piece) {
	if (piece < 0 || piece >= 32) return Vec2();
	return Vec2(((piece / 4) % 2 != 0 ? 0 : 75) + 150 * (piece % 4) + 37, (piece / 4) * 75 + 37);
}

void drawPlayerType(int x, int y, PlayerType type) {
	//x y is at the corner (easy access for sdl_rect)
	switch (type) {
	case HUMAN:
		drawOpenRectangle(x, y, 50, 40, SILVER);
		break;
	case BOTHARD:
		drawFilledRectangle(x + 55, y + 1, 15, 10, RED);
	case BOTMID:
		drawFilledRectangle(x + 55, y + 15, 15, 10, GOLD);
	case BOTEASY:
		drawFilledRectangle(x, y, 50, 40, SILVER);
		drawCircle(x + 15, y + 14, 6, DARK_GRAY);
		drawCircle(x + 35, y + 14, 6, DARK_GRAY);
		drawOpenRectangle(x + 12, y + 25, 26, 10, DARK_GRAY);
		for (int i = -1; i < 2; i++) {
			drawLine(x + 25 + 7 * i, y + 25, x + 25 + 7 * i, y + 34);
		}
		setRenderColor(SILVER);
		drawLine(x + 20, y, x + 10, y - 15);
		drawCircle(x + 10, y - 15, 3);
		drawLine(x + 30, y, x + 40, y - 15);
		drawCircle(x + 40, y - 15, 3);
		drawFilledRectangle(x + 55, y + 29, 15, 10, GREEN);
	}
	
	switch (type) {
	case BOTEASY:
		drawCircle(x + 15, y + 14, 2, GREEN);
		drawCircle(x + 35, y + 14, 2, GREEN);
		break;
	case BOTMID:
		drawCircle(x + 15, y + 14, 2, GOLD);
		drawCircle(x + 35, y + 14, 2, GOLD);
		break;
	case BOTHARD:
		drawCircle(x + 15, y + 14, 2, RED);
		drawCircle(x + 35, y + 14, 2, RED);
		break;
	}
}

void drawPausePlay(int x, int y, bool paused) {
	//true - draw PLAY button: false - draw PAUSE button
	drawFilledRectangle(x, y, 40, 40, GOLD);
	drawOpenRectangle(x, y, 40, 40, BOARD_LIGHT);
	if (paused) {
		drawFilledRectangle(x + 11, y + 10, 7, 20);
		drawFilledRectangle(x + 22, y + 10, 7, 20);
	}
	else {
		for (int i = 0; i < 11; i++) {
			drawLine(x + 10, y + 10 + i, x + 10 + 2 * i, y + 10 + i);
			drawLine(x + 10, y + 30 - i, x + 10 + 2 * i, y + 30 - i);
		}
	}
}

bool rectIsTouched(SDL_Rect rect, int x, int y) {
	if (rect.w < 0) {
		rect.x += rect.w;
		rect.w *= -1;
	}
	if (rect.h < 0) {
		rect.y += rect.h;
		rect.h *= -1;
	}

	return (x >= rect.x) && (x <= rect.x + rect.w) && (y >= rect.y) && (y <= rect.y + rect.h);
}
bool rectIsTouched(SDL_Rect rect, Vec2 pos) {
	return rectIsTouched(rect, pos.x, pos.y);
}