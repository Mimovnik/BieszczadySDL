#define _USE_MATH_DEFINES
#include<math.h>

#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>
#include <iostream>
#include "Vector.h"
#include "Rectangle.h"
#include "RigidBody.h"

#define FULLSCREEN_ON false
#define MAX_FPS 150
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

void displaySetUp(SDL_Surface** charset, SDL_Surface** screen, SDL_Texture** screenTexture, SDL_Window** window, SDL_Renderer** renderer) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		throw 1;
	}

	// tryb pe³noekranowy / fullscreen mode
	int rendererCreated;
	if (FULLSCREEN_ON) {
		rendererCreated = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
			window, renderer);
	}
	else {
		rendererCreated = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
			window, renderer);
	}

	if (rendererCreated != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		throw 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(*window, "Bieszczady");


	*screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	*screenTexture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	// wczytanie obrazka cs8x8.bmp
	*charset = SDL_LoadBMP("bmp/cs8x8.bmp");
	if (*charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(*screen);
		SDL_DestroyTexture(*screenTexture);
		SDL_DestroyWindow(*window);
		SDL_DestroyRenderer(*renderer);
		SDL_Quit();
		throw 1;
	};
	SDL_SetColorKey(*charset, true, 0x000000);
}

SDL_Surface* loadBMP(const char* fileName, SDL_Surface* charset, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer) {
	SDL_Surface* surface = SDL_LoadBMP(fileName);
	if (surface == NULL) {
		printf("SDL_LoadBMP(%s) error: %s\n", fileName, SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		throw 1;
	};
	SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 255, 0, 255));
	return surface;
}
// narysowanie napisu txt na powierzchni screen, zaczynając od punktu (x, y)
// charset to bitmapa 128x128 zawierająca znaki
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt środka obrazka sprite na ekranie
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y, Vector offset) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2 - offset.x;
	dest.y = y - sprite->h / 2 - offset.y;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};

// rysowanie linii o długości l w pionie (gdy dx = 0, dy = 1) 
// bądź poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

// rysowanie prostokąta o długości boków l i k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

void move(Vector* position, Vector* velocity, double gameTime) {
}

int main(int argc, char* args[]) {
	SDL_Surface* screen = nullptr;
	SDL_Texture* screenTexture = nullptr;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Surface* charset = nullptr,
		* theme = nullptr;

	SDL_Event event;

	Vector screenMiddle = Vector(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	try {
		displaySetUp(&charset, &screen, &screenTexture, &window, &renderer);
	}
	catch (const char* error) {
		std::cerr << error;
	}

	SDL_Surface* heroSurface = loadBMP("bmp/witcher200.bmp", charset, screen, screenTexture, window, renderer);
	RigidBody player = RigidBody(screenMiddle, heroSurface, 3);

	theme = loadBMP("bmp/theme.bmp", charset, screen, screenTexture, window, renderer);

	char text[128];
	int black = SDL_MapRGB(screen->format, 0, 0, 0);
	int silver = SDL_MapRGB(screen->format, 192, 192, 192);
	int green = SDL_MapRGB(screen->format, 0, 153, 51);
	int red = SDL_MapRGB(screen->format, 204, 0, 0);
	int blue = SDL_MapRGB(screen->format, 51, 204, 255);
	int brown = SDL_MapRGB(screen->format, 102, 51, 0);

	int currentTime, delta;
	int lastTime = 0, frames = 0;
	double fpsTimer = 0, fps = 0, realTime = 0, gameTime = 0;
	double gameDelta;
	bool quit = false;
	double timeFactor = 0.01;
	lastTime = SDL_GetTicks();
	Vector camera = player.position;

	double distance = 0;
	double speed = 0.5;

	while (!quit) {
		currentTime = SDL_GetTicks();
		delta = currentTime - lastTime;
		lastTime = currentTime;
		gameDelta = delta * timeFactor;
		if (delta < (1000 / MAX_FPS)) SDL_Delay(1000 / MAX_FPS - delta);


		realTime += delta;
		gameTime += gameDelta;

		distance += speed * gameDelta;

		SDL_FillRect(screen, NULL, black);

		camera = player.position;

		DrawSurface(screen, theme, screenMiddle.x, screenMiddle.y, camera);

		player.move(gameDelta);
		player.draw(screen, camera);

		fpsTimer += delta;
		if (fpsTimer > 500) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 500;
		};

		// tekst informacyjny 
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 36, silver, brown);

		sprintf_s(text, "Szablon drugiego zadania, czas trwania = %.1lf s  %.0lf klatek / s", realTime / 1000, fps);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		//	      "Esc - exit, \030 - faster, \031 - slower"
		sprintf_s(text, "Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

		SDL_UpdateTexture(screenTexture, NULL, screen->pixels, screen->pitch);

		SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
		SDL_RenderPresent(renderer);

		// obsługa zdarzeń (o ile jakieś zaszły)
		SDL_PumpEvents();//every gameSecond
		const Uint8* KeyState = SDL_GetKeyboardState(NULL);
		if (KeyState[SDL_SCANCODE_ESCAPE]) quit = 1;

		double speedLength;
		if (KeyState[SDL_SCANCODE_W] || KeyState[SDL_SCANCODE_S] || KeyState[SDL_SCANCODE_A] || KeyState[SDL_SCANCODE_D]) {
			speedLength = player.velocity.magnitude() + player.accelerationRate;
		}

		if (KeyState[SDL_SCANCODE_W]) {
			player.velocity = Vector::fromAngle(speedLength, 180);
		}
		if (KeyState[SDL_SCANCODE_S]) {
			player.velocity = Vector::fromAngle(speedLength, 0);
		}
		if (KeyState[SDL_SCANCODE_A]) {
			player.velocity = Vector::fromAngle(speedLength, -90);
		}
		if (KeyState[SDL_SCANCODE_D]) {
			player.velocity = Vector::fromAngle(speedLength, 90);
		}
		if (KeyState[SDL_SCANCODE_W] && KeyState[SDL_SCANCODE_A]) {
			player.velocity = Vector::fromAngle(speedLength, -135);
		}
		if (KeyState[SDL_SCANCODE_W] && KeyState[SDL_SCANCODE_D]) {
			player.velocity = Vector::fromAngle(speedLength, 135);
		}
		if (KeyState[SDL_SCANCODE_S] && KeyState[SDL_SCANCODE_A]) {
			player.velocity = Vector::fromAngle(speedLength, -45);
		}
		if (KeyState[SDL_SCANCODE_S] && KeyState[SDL_SCANCODE_D]) {
			player.velocity = Vector::fromAngle(speedLength, 45);
		}

		frames++;
	}

	// zwolnienie powierzchni
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(screenTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}