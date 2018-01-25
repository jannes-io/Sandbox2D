#pragma once

#include "../stdafx.h"
#include "PrecisionTimer.h"
#include "../EntryState.h"

// Static Variable Initialization
Sandbox2D* Sandbox2D::_sandbox2DPtr = nullptr;

// Sandbox2D constructor/deconstructor
Sandbox2D::Sandbox2D() {}
Sandbox2D::~Sandbox2D() {}

/*********************
* PRIVATE FUNCTIONS  *
*********************/
Sandbox2D* Sandbox2D::GetSingleton()
{
	return _sandbox2DPtr == nullptr ? _sandbox2DPtr = new Sandbox2D() : _sandbox2DPtr;
}

int Sandbox2D::run()
{
	// Initialise the game
	init();
	
	// Start entry state
	_states.push_back(new EntryState());
	_states.back()->stateStart();

	// Main game loop
	double previous = _gameTickTimerPtr->GetGameTime() - _maxFPS;
	double lag = 0.00000; // Left over time
	
	// Main loop _flag
	bool quit = false;

	// Event handler
	while (!quit)
	{
		const double current = _gameTickTimerPtr->GetGameTime();
		double elapsed = current - previous;
		// prevent jumps in time when game is sleeping
		if (elapsed > 0.25) elapsed = 0.25; 
		previous = current;
		lag += elapsed;
		while (lag >= _maxFPS)
		{
			_inputManager->setOldInputStates();
			// Handle all events in queue
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				// User requests quit
				if (event.type == SDL_QUIT) quit = true;
			}
			_inputManager->setCurrInputStates();

			// Execute the game tick
			_states.back()->stateTick(_maxFPS);

			// Execute the game paint
			_graphics->renderAll(_states.back());
			
			lag -= _maxFPS;
		}
	}
	return 0;
}

void Sandbox2D::init()
{
	// Initialise logging
	if (CreateLog()) LogInfo("Logging started!");

	// Initialise high precision timer
	_gameTickTimerPtr = new PrecisionTimer();
	_gameTickTimerPtr->Reset();

	// Initialse Cache
	_cache = new Cache();

	// Initialise InputManager and InputEvents
	_inputManager = new InputManager();

	// TODO: change to reading from ini file
	GameSettings gameSettings;
	gameSettings.windowSize = { 1280,720 };
	gameSettings.windowTitle = "S2D Test environment";

	// Initialise Graphics
	_graphics = new Graphics();
	_graphics->init(GetSingleton(), gameSettings, _cache);

	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return;
	}

	LogInfo("Sandbox2D initialised successfully!");
}

void Sandbox2D::destroy()
{
	LogInfo("Destroying Sandbox2D!");

	// Delete any left over gamestates
	while (_states.size() > 0)
		this->popState();

	// Engine cleanup
	delete _gameTickTimerPtr;
	_gameTickTimerPtr = nullptr;

	delete _inputManager;
	_inputManager = nullptr;

	_graphics->cleanup();
	delete _graphics;
	_graphics = nullptr;

	delete _cache;
	_cache = nullptr;

	// Quit SDL subsystems
	std::thread tMix(Mix_Quit);
	std::thread tTTF(TTF_Quit);
	std::thread tIMG(IMG_Quit);

	tMix.join();
	tTTF.join();
	tIMG.join();

	SDL_Quit();
	LogInfo("SDL cleanup finished!");

	// Delete engine singleton
	if (_sandbox2DPtr != nullptr)
	{
		delete _sandbox2DPtr;
		_sandbox2DPtr = nullptr;
	}
	LogInfo("Sandbox2D destroyed successfully!");
}

/*********************
 * PUBLIC FUNCTIONS  *
 *********************/

void Sandbox2D::changeState(GameState* state)
{
	popState();
	pushState(state);
}

void Sandbox2D::pushState(GameState* state)
{
	// Add state to gamestates
	_states.push_back(state);

	// User fegined start
	_states.back()->stateStart();
}

void Sandbox2D::popState()
{
	// User defined cleanup
	_states.back()->stateEnd();

	// Delete the gamestate
	delete _states.back();
	_states.pop_back();
}

Cache* Sandbox2D::getCache() const
{
	return _cache;
}

void Sandbox2D::bitmapToCache(std::string name, std::string path) const
{
	_cache->bmpCache.push(name, _cache->bmpCache.CreateCachableBitmap(path));
}

void Sandbox2D::fontToCache(std::string name, std::string path, int size) const
{
	_cache->fntCache.push(name, _cache->fntCache.CreateCachableFont(path, size));
}
