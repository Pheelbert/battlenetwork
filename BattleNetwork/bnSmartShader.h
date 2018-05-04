#pragma once
#include <SFML/Graphics.hpp>
#include <map>

class SmartShader
{
  friend class Engine;
private:
  sf::Shader* ref;
  std::map<std::string, int>    iuniforms;
  std::map<std::string, float>  funiforms;
  std::map<std::string, double> duniforms;
  // TODO: add support for other unform types as needed

  void ApplyUniforms();
  void ResetUniforms();
  sf::Shader* Get();

public:
  SmartShader();
  SmartShader(SmartShader&);
  ~SmartShader();
  SmartShader(sf::Shader& rhs);
  SmartShader& operator=(sf::Shader& rhs);
  SmartShader& operator=(sf::Shader* rhs);
  void SetUniform(std::string uniform, float fvalue);
  void SetUniform(std::string uniform, int ivalue);
  void Reset();
};

