#pragma once
#include "asset_manager.h"
#include "debug.h"

std::unordered_map<std::string, Shader*> AssetManager::shaders;
std::unordered_map<std::string, Texture*> AssetManager::textures;
std::unordered_map<std::string, SpriteSheet*> AssetManager::spriteSheets;

Pool<Texture> AssetManager::texture_pool;
Pool<Shader> AssetManager::shader_pool;
Pool<SpriteSheet> AssetManager::spriteSheet_pool;