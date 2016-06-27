/* main.cpp
 * the "core" of the game engine.
 *
 * All parts which deals with operative system (opening a window,
 * capturing mouses, keyboards), events loop, timers...
 *
 */

#include <iostream>

/* we use SDL but note the rest of the code is SDL free!!!
 * E.g. it should be easy to change this with, e.g. freeGlut, glfw, etc
 */

#include <SDL.h>
#include "custom_classes.h"
#include "aimind.h"

using namespace std;
const int FPS = 30;

SDL_Window *win = NULL;
SDL_GLContext glcontext;

int N_PLAYERS = 1; // 0, 1 or 2

bool quitGame = false;

AiMind aiP0;
AiMind aiP1;

void rendering();
void initRendering();
void preloadAllAssets();

#define SDL_TIMEREVENT SDL_USEREVENT


unsigned int pushTimerEvent(unsigned int /*time*/ , void* /*data*/ ){
	SDL_Event e;
	e.type = SDL_TIMEREVENT;
	SDL_PushEvent(&e);
	return 1000/FPS;
}

void callbackKeyboard(SDL_Event &e , bool isDown ){
	int key = e.key.keysym.sym;
	switch (key) {
	case SDLK_ESCAPE :
		quitGame = true;
		break;
	case SDLK_r:
		if (!isDown) scene.initAsNewGame();
		break;
	}
	scene.ships[0].controller.soakKey( key, isDown );
	scene.ships[1].controller.soakKey( key, isDown );
}

// questo viene invocato FPS volte al secondo:
void callbackTimer(SDL_Event& ){

	SDL_GL_MakeCurrent( win, glcontext );

	aiP0.rethink( scene.ships[0].controller );
	aiP1.rethink( scene.ships[1].controller );

	scene.doPhysStep();

	rendering();

	SDL_GL_SwapWindow( win );

	/* // count frames:
	static int nframe = 0;
	std::cout << "Frame "<< (nframe++) <<"\n";
	*/
}

void processEvent( SDL_Event &e) {
	switch (e.type) {
	case SDL_QUIT: quitGame = true; break;

	case SDL_KEYDOWN: callbackKeyboard( e , true ); break;
	case SDL_KEYUP  : callbackKeyboard( e , false); break;

	case SDL_TIMEREVENT: callbackTimer( e ); break;
	}
}

void ShipController::useArrows(){
	key[ LEFT ] = SDLK_LEFT;
	key[ RIGHT ] = SDLK_RIGHT;
	key[ GO ] = SDLK_UP;
	key[ FIRE ] = SDLK_RCTRL;
}

void ShipController::useWASD(){
	key[ LEFT ] = SDLK_a;
	key[ RIGHT ] = SDLK_d;
	key[ GO ] = SDLK_w;
	key[ FIRE ] = SDLK_LSHIFT;
}

int main(int , char **)
{

	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	win = SDL_CreateWindow(
		"Kamikaze!!!",
		100, 100, 500, 500,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);

	if (win == NULL){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	glcontext = SDL_GL_CreateContext(win);
	initRendering();

	scene.initAsNewGame();
	preloadAllAssets();

	SDL_AddTimer( 1000/FPS, pushTimerEvent, NULL );

	if (N_PLAYERS>0) scene.ships[0].controller.useArrows();
	else {
		aiP0.me = &(scene.ships[0]);
		aiP0.target = &(scene.ships[1]);
		aiP0.setHumanLike();
	}

	if (N_PLAYERS>1) scene.ships[1].controller.useWASD();
	else {
		aiP1.me = &(scene.ships[1]);
		aiP1.target = &(scene.ships[0]);
		aiP1.setTerminator();
		//aiP1.setHumanLike();
	}

	/* ciclo degli eventi */
	while (!quitGame) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		processEvent(e);
	}
	SDL_Quit();

	return 0;
}
