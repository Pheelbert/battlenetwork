#pragma once
#include "bnTextureType.h"
#include "bnMemory.h"
#include "bnLogger.h"

#include <SFML/Graphics.hpp>
using sf::Texture;
using sf::Font;
using std::string;
#include <map>
using std::map;
using std::pair;
#include <vector>
using std::vector;
#include <iostream>
using std::cerr;
using std::endl;

class TextureResourceManager
{
public:
	static TextureResourceManager& GetInstance();
	void LoadAllTextures();
	Texture* LoadTextureFromFile(string _path);
	Texture* GetTexture(TextureType _ttype);
	Font* LoadFontFromFile(string _path);
    string LoadDataFromFile(string _path);

private:
	TextureResourceManager(void);
	~TextureResourceManager(void);
	vector<string> paths;
	map<TextureType, Texture*> textures;
};