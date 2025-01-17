#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include "constants.h"
#include "Texture2D.h"
#include "Commons.h"
#include "GameScreenManager.h"



using namespace std;



//Globals
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
GameScreenManager* game_screen_manager;
Uint32 g_old_time;
Mix_Music* g_music = nullptr;
Mix_Chunk* g_sfx = nullptr;
//Texture2D* g_texture = nullptr;

//Function prototypes
bool InitSDL();
void CloseSDL();
bool Update();
void Render();
void LoadMusic(string path);




int main(int argc, char* args[])
{
	//check if SDL was setup correctly
	if (InitSDL())
	{
		game_screen_manager = new GameScreenManager(g_renderer, SCREEN_LEVEL1);
		//set the time
		g_old_time = SDL_GetTicks();



		//flag to check if we wish to quit
		bool quit = false;

		//game loop
		while (!quit)
		{
			Render();
			quit = Update();
		}




	}
	CloseSDL();
	return 0;
}

bool InitSDL()
{
	//initialise the mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cout << "Mixer could not init, error: " << Mix_GetError();
		return false;
	}

	LoadMusic("Music/Mario.mp3");
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(g_music, -1);
	}



	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL did not initialise.  Error: " << SDL_GetError();
		return false;
	}
	else
	{
		//setup passed so create window
		g_window = SDL_CreateWindow("Games Engine Creation",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
		//did the window get created?
		if (g_window == nullptr)
		{
			//window failed
			cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}

	}

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

	if (g_renderer != nullptr)
	{
		//init PNG loading
		int imageFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imageFlags) & imageFlags))
		{
			cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
			return false;
		}
	}
	else
	{
		cout << "Renderer could not initialise. Error: " << SDL_GetError();
		return false;
	}

	//Load the background texture
	//g_texture = new Texture2D(g_renderer);

	//if (!g_texture->LoadFromFile("Images/maxresdefault.jpg"))
	//{
	//	return false;
	//}
	


}

void CloseSDL() 
{
	//clear up music
	Mix_FreeMusic(g_music);
	g_music = nullptr;


	//release the window
	SDL_DestroyWindow(g_window); 
	g_window = nullptr;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();

	//release the texture
	//delete g_texture;
	//g_texture = nullptr;

	//release the renderer
	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;

	//destroy the game screen manager
	delete game_screen_manager;
	game_screen_manager = nullptr;




}

bool Update()
{
	Uint32 new_time = SDL_GetTicks();

	//Event handler
	SDL_Event e;

	//get events
	SDL_PollEvent(&e);

	//handle the events
	switch (e.type)
	{
		//click the 'X' to quit
	case SDL_QUIT:
		return true;
		break;
	}

	//cout << game_screen_manager->Quitting() << endl;
	if (game_screen_manager->Quitting())
	{
		cout << "Now quitting" << endl;
		return true;
	}


	game_screen_manager->Update((float)(new_time - g_old_time) / 1000.0f, e);
	g_old_time = new_time;
	return false;
}

void Render()
{
	//Clear the screen
	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(g_renderer);

	//g_texture->Render(Vector2D(), SDL_FLIP_NONE);
	game_screen_manager->Render();

	//update the screen
	SDL_RenderPresent(g_renderer);

}


void LoadMusic(string path)
{
	g_music = Mix_LoadMUS(path.c_str());
	if (g_music == nullptr)
	{
		cout << "Failed to load music. Error: " << Mix_GetError() << endl;
	}
}

