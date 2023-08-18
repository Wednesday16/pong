#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FPS 60

void ResetRects(SDL_Rect* ball, SDL_Rect* rightPad, SDL_Rect* leftPad) {
	// defaultPosition rects are obligatory because immediate definition doesn't work
	SDL_Rect rightPadDefaultPosition = { 610, 300, 20, 120 };
	SDL_Rect leftPadDefaultPosition = { 10, 60, 20, 120 };
	SDL_Rect ballDefaultPosition = { 310, 230, 20, 20 };

	*rightPad = rightPadDefaultPosition;
	*leftPad = leftPadDefaultPosition;
	*ball = ballDefaultPosition;
}

bool CheckCollision(SDL_Rect* obj1, SDL_Rect* obj2) {
	// collision in person
	if (((obj1->x > obj2->x) && (obj1->x < (obj2->x + obj2->w)))
		&& ((obj1->y > obj2->y) && (obj1->y < (obj2->y + obj2->h)))) return true;
	else return false;
}

void MovePadUp(SDL_Rect* pad, int* padSpeed, int* numkeys) {
	if (pad->y > 0) {
		pad->y -= *padSpeed;
	}
}

void MovePadDown(SDL_Rect* pad, int* padSpeed, int* numkeys) {
	if ((pad->y + pad->h) <= SCREEN_HEIGHT) {
		pad->y += *padSpeed;
	}
}

void MoveBall(SDL_Rect* ball, int* ballSpeedX, int* ballSpeedY, SDL_Rect* rightPad, SDL_Rect* leftPad) {

	// collision checking
	if (CheckCollision(ball, rightPad)) {
		*ballSpeedX = -(*ballSpeedX);
	}

	if (CheckCollision(ball, leftPad)) {
		*ballSpeedX = -(*ballSpeedX);
	}

	// moves the ball around
	ball->x += *ballSpeedX;
	ball->y += *ballSpeedY;

	// checks for boundaries
	if ((ball->y >= SCREEN_HEIGHT) || (ball->y < 0)) {
		*ballSpeedY = -(*ballSpeedY);
	}

	// resets everything after scoring
	if ((ball->x >= SCREEN_WIDTH) || (ball->x < 0)) {
		*ballSpeedX = -(*ballSpeedX);
		ResetRects(ball, rightPad, leftPad);
		*ballSpeedX = 7;
		*ballSpeedY = 7;
		SDL_Delay(1000);
	}
}


int main(int argc, char* args[]) {
	// initialization
	SDL_Window* window = SDL_CreateWindow("pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Error occured while creating window, error %s\n", SDL_GetError());
		return 1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("Error occured while creating renderer, error %s\n", SDL_GetError());
		return 1;
	}
	SDL_Event event;
	bool isGame = true;
	SDL_Rect ball;
	SDL_Rect rightPad;
	SDL_Rect leftPad;
	int padSpeed = 5;
	int ballSpeedX = 7;
	int ballSpeedY = 7;
	const Uint8* numkeys;
	numkeys = SDL_GetKeyboardState(NULL);
	double delayTime = (1. / (double)FPS) * 1000.;

	ResetRects(&ball, &rightPad, &leftPad);

	while (isGame) {
		// updates keys
		while (SDL_PollEvent(&event) != 0)
		{
			// quits
			if (event.key.keysym.sym == SDLK_ESCAPE) isGame = false;
		}

		// checks for key updates
		if (numkeys[SDL_SCANCODE_W]) {
			MovePadUp(&leftPad, &padSpeed, &numkeys);
		}

		if (numkeys[SDL_SCANCODE_UP]) {
			MovePadUp(&rightPad, &padSpeed, &numkeys);
		}

		if (numkeys[SDL_SCANCODE_S]) {
			MovePadDown(&leftPad, &padSpeed, &numkeys);
		}

		if (numkeys[SDL_SCANCODE_DOWN]) {
			MovePadDown(&rightPad, &padSpeed, &numkeys);
		}

		MoveBall(&ball, &ballSpeedX, &ballSpeedY, &rightPad, &leftPad);

		// renders the pads and the ball
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		SDL_RenderFillRect(renderer, &rightPad);
		SDL_RenderFillRect(renderer, &leftPad);
		SDL_RenderFillRect(renderer, &ball);

		SDL_RenderPresent(renderer);

		SDL_Delay(delayTime);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}