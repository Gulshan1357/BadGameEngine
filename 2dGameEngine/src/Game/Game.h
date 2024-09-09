#pragma once
#include <memory>
#include "../AssetStore/AssetStore.h"


const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class SDL_Window;
class SDL_Renderer;
class Registry;
class AssetStore;

class Game
{
private:
	bool isRunning;
	int millisecsPreviousFrame;
	SDL_Window* window;
	SDL_Renderer* renderer;

	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetStore> assetStore;

public:
	Game();
	~Game();
	void Initialize();
	void Run();
	void Setup(); // Equivalaent to Unity's start function
	void LoadLevel(int level);
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	int windowWidth;
	int windowHeight;

};