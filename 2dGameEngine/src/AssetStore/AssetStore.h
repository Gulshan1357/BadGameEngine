#pragma once

#include<map>
#include<string>
#include<SDL.h>

class AssetStore
{
private:
	std::map<std::string, SDL_Texture*> textures;
	//TODO: create a map for fonts
	// TOFO: create a map for audio

public:
	AssetStore();
	~AssetStore();

	void ClearAssets();
	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
	SDL_Texture* GetTexture(const std::string& assetId);

};