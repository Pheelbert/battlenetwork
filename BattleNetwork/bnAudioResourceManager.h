#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include "bnAudioType.h"
#include <atomic>

// For more authentic retro experience, decrease available channels.
#define NUM_OF_CHANNELS 10

class AudioResourceManager {
public:
  static AudioResourceManager& GetInstance();

  void LoadAllSources(std::atomic<int> &status);
  void LoadSource(AudioType type, const std::string& path);
  int Play(AudioType type, int priority = 1);
  int Stream(std::string path, bool loop = false);
  void StopStream();
  void SetStreamVolume(float volume);
  void SetChannelVolume(float volume);

  AudioResourceManager();
  ~AudioResourceManager();

private:
  sf::Sound* channels;
  sf::SoundBuffer* sources;
  sf::Music stream;
  int channelVolume;
  int streamVolume;
};

