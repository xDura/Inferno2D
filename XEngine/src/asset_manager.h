#pragma once
#include "shader.h"
#include "texture.h"
#include "sprite_sheet.h"
#include "pool.h"
#include <unordered_map>
#include <string>
#include "External/tinyxml2.h"
#include "tilemap.h"
#include "utils.h"
#include "level.h"

#define DEFAULT_ASSET_POOL_SIZE 100

class AssetManager
{
public:
	static std::unordered_map<std::string, Shader*> shaders;
	static std::unordered_map<std::string, Texture*> textures;
	static std::unordered_map<std::string, SpriteSheet*> spriteSheets;
	static std::unordered_map<std::string, Tilemap*> tilemaps;
	static std::unordered_map<std::string, Level*> levels;

	static Texture* GetTexture(const char* path)
	{
		std::string pathString = path;
		std::unordered_map<std::string, Texture*>::const_iterator findResult = textures.find(pathString);
		Texture* tex = NULL;
		if (findResult == textures.end())
			tex = LoadTexture(path);
		else
			tex = findResult->second;
		ASSERT(tex != NULL);
		return tex;
	}

	static Shader* GetShader(const char* vsPath, const char* psPath)
	{
		std::string keyString = vsPath;
		keyString += psPath;
		std::unordered_map<std::string, Shader*>::const_iterator findResult = shaders.find(keyString);
		Shader* shader = NULL;
		if (findResult == shaders.end())
			shader = LoadShader(vsPath, psPath);
		else
			shader = findResult->second;
		ASSERT(shader != NULL);
		return shader;
	}

	static SpriteSheet* GetSpriteSheet(const char* path)
	{
		std::string pathString = path;
		std::unordered_map<std::string, SpriteSheet*>::const_iterator findResult = spriteSheets.find(pathString);
		SpriteSheet* spriteSheet = NULL;
		if (findResult == spriteSheets.end())
			spriteSheet = LoadSpriteSheet(path);
		else
			spriteSheet = findResult->second;
		ASSERT(spriteSheet != NULL);
		return spriteSheet;
	}

	static Tilemap* GetTilemap(const char* path) 
	{
		std::string pathString = path;
		std::unordered_map<std::string, Tilemap*>::const_iterator findResult = tilemaps.find(pathString);
		Tilemap* tilemap = NULL;
		if (findResult == tilemaps.end())
			tilemap = LoadTilemap(path);
		else
			tilemap = findResult->second;
		ASSERT(tilemap != NULL);
		return tilemap;
	}

	static Level* GetLevel(const char* path)
	{
		std::string pathString = path;
		std::unordered_map<std::string, Level*>::const_iterator findResult = levels.find(pathString);
		Level* level = NULL;
		if (findResult == levels.end())
			level = LoadLevel(path);
		else
			level = findResult->second;
		ASSERT(level != NULL);
		return level;
	}


	static void Init()
	{
		shader_pool.prewarm(DEFAULT_ASSET_POOL_SIZE);
		texture_pool.prewarm(DEFAULT_ASSET_POOL_SIZE);
		spriteSheet_pool.prewarm(DEFAULT_ASSET_POOL_SIZE);
		tilemap_pool.prewarm(DEFAULT_ASSET_POOL_SIZE);
		level_pool.prewarm(DEFAULT_ASSET_POOL_SIZE);
	}

	static void ReloadAll()
	{
	}

	static void ReloadTilemaps() 
	{
		std::unordered_map<std::string, Tilemap*>::iterator it = tilemaps.begin();
		while (it != tilemaps.end())
		{
			it->second->ReloadXML();
			it++;
		}
	}

	static void Destroy()
	{
		shaders.clear();
		textures.clear();
		spriteSheets.clear();
		tilemaps.clear();
		levels.clear();

		shader_pool.release();
		texture_pool.release();
		spriteSheet_pool.release();
		tilemap_pool.release();
		level_pool.release();
	}

private:
	static Pool<Shader> shader_pool;
	static Pool<Texture> texture_pool;
	static Pool<SpriteSheet> spriteSheet_pool;
	static Pool<Tilemap> tilemap_pool;
	static Pool<Level> level_pool;

	static Texture* LoadTexture(const char* path)
	{
		std::string pathString = path;
		ASSERT(textures.find(pathString) == textures.end());
		Texture* newTex = texture_pool.spawn();
		newTex->Load(path);
		textures.emplace(pathString, newTex);
		return newTex;
	}
	static Shader* LoadShader(const char* vsPath, const char* psPath)
	{
		std::string keyString = vsPath;
		keyString += psPath;
		ASSERT(shaders.find(keyString) == shaders.end());
		Shader* newShader = shader_pool.spawn();
		newShader->Load(vsPath, psPath);
		shaders.emplace(keyString, newShader);
		return newShader;
	}

	static SpriteSheet* LoadSpriteSheet(const char* path)
	{
		std::string keyString = path;
		ASSERT(spriteSheets.find(path) == spriteSheets.end());
		SpriteSheet* spriteSheet = spriteSheet_pool.spawn();
		spriteSheet->Delete();
		spriteSheet->LoadXML(path);
		spriteSheets.emplace(keyString, spriteSheet);
		return spriteSheet;
	}

	static Tilemap* LoadTilemap(const char* path)
	{
		std::string keyString = path;
		ASSERT(spriteSheets.find(path) == spriteSheets.end());
		Tilemap* tilemap = tilemap_pool.spawn();
		tilemap->Delete();
		tilemap->LoadXML(path);
		tilemaps.emplace(keyString, tilemap);
		return tilemap;
	}

	static Level* LoadLevel(const char* path)
	{
		std::string keyString = path;
		ASSERT(spriteSheets.find(path) == spriteSheets.end());
		Level* level = level_pool.spawn();
		level->Delete();
		level->LoadXML(path);
		levels.emplace(keyString, level);
		return level;
	}
};