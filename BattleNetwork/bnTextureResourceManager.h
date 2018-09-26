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
#include <atomic>

class TextureResourceManager {
public:
  static TextureResourceManager& GetInstance();
  void LoadAllTextures(std::atomic<int> &status);
  Texture* LoadTextureFromFile(string _path);
  Texture* GetTexture(TextureType _ttype);
  sf::IntRect GetCardRectFromID(unsigned ID);
  sf::IntRect GetIconRectFromID(unsigned ID);
  Font* LoadFontFromFile(string _path);

private:
  TextureResourceManager(void);
  ~TextureResourceManager(void);
  vector<string> paths;
  map<TextureType, Texture*> textures;
};

#define TEXTURES TextureResourceManager::GetInstance()
#define LOAD_TEXTURE(x) *TEXTURES.GetTexture(TextureType::x)