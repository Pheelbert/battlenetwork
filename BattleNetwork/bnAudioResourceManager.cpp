#include "bnAudioResourceManager.h"
#include "bnLogger.h"

AudioResourceManager& AudioResourceManager::GetInstance() {
  static AudioResourceManager instance;
  return instance;
}

AudioResourceManager::AudioResourceManager() {
  channels = new sf::Sound[NUM_OF_CHANNELS];

  for (int i = 0; i < NUM_OF_CHANNELS; i++) {
    channels[i] = sf::Sound();
  }

  sources = new sf::SoundBuffer[AudioType::AUDIO_TYPE_SIZE];

  for (int i = 0; i < AUDIO_TYPE_SIZE; i++) {
    sources[i] = sf::SoundBuffer();
  }

  channelVolume = streamVolume = 100; //SFML default
}


AudioResourceManager::~AudioResourceManager() {
  // Stop playing everything 
  stream.stop();

  for (int i = 0; i < NUM_OF_CHANNELS; i++) {
    channels[i].stop();
  }

  // Free memory
  delete[] channels;
  delete[] sources;
}

void AudioResourceManager::LoadAllSources() {
  LoadSource(AudioType::APPEAR, "resources/sfx/appear.ogg");
  LoadSource(AudioType::AREA_GRAB, "resources/sfx/area_grab.ogg");
  LoadSource(AudioType::AREA_GRAB_TOUCHDOWN, "resources/sfx/area_grab_touchdown.ogg");
  LoadSource(AudioType::BUSTER_CHARGED, "resources/sfx/buster_charged.ogg");
  LoadSource(AudioType::BUSTER_CHARGING, "resources/sfx/buster_charging.ogg");
  LoadSource(AudioType::CANNON, "resources/sfx/cannon.ogg");
  LoadSource(AudioType::CHIP_CANCEL, "resources/sfx/chip_cancel.ogg");
  LoadSource(AudioType::CHIP_CHOOSE, "resources/sfx/chip_choose.ogg");
  LoadSource(AudioType::CHIP_CONFIRM, "resources/sfx/chip_confirm.ogg");
  LoadSource(AudioType::CHIP_DESC, "resources/sfx/chip_desc.ogg");
  LoadSource(AudioType::CHIP_DESC_CLOSE, "resources/sfx/chip_desc_close.ogg");
  LoadSource(AudioType::CHIP_SELECT, "resources/sfx/chip_select.ogg");
  LoadSource(AudioType::CUSTOM_BAR_FULL, "resources/sfx/custom_bar_full.ogg");
  LoadSource(AudioType::DELETED, "resources/sfx/deleted.ogg");
  LoadSource(AudioType::EXPLODE, "resources/sfx/explode.ogg");
  LoadSource(AudioType::GUN, "resources/sfx/gun.ogg");
  LoadSource(AudioType::HEALTH_ALERT, "resources/sfx/health_alert.ogg");
  LoadSource(AudioType::HURT, "resources/sfx/hurt.ogg");
  LoadSource(AudioType::PANEL_CRACK, "resources/sfx/panel_crack.ogg");
  LoadSource(AudioType::PANEL_RETURN, "resources/sfx/panel_return.ogg");
  LoadSource(AudioType::PAUSE, "resources/sfx/pause.ogg");
  LoadSource(AudioType::PRE_BATTLE, "resources/sfx/pre_battle.ogg");
  LoadSource(AudioType::RECOVER, "resources/sfx/recover.ogg");
  LoadSource(AudioType::SPREADER, "resources/sfx/spreader.ogg");
  LoadSource(AudioType::SWORD_SWING, "resources/sfx/sword_swing.ogg");
  LoadSource(AudioType::TOSS_ITEM, "resources/sfx/toss_item.ogg");
  LoadSource(AudioType::WAVE, "resources/sfx/wave.ogg");
}

void AudioResourceManager::LoadSource(AudioType type, const std::string& path) {
  if (!sources[type].loadFromFile(path)) {
    Logger::Failf("Failed loading audio: %s\n", path.c_str());
  } else {
    Logger::Logf("Loaded audio: %s\n", path.c_str());
  }
}

int AudioResourceManager::Play(AudioType type, int priority) {
  if (type == AudioType::AUDIO_TYPE_SIZE) {
    return -1;
  }

  // NOTE: As of now all sources have the same priority: LOW 
  //       This means a source can only be played one at a time
  // NOTE: Future priorities are LOWEST (can be canceled for something else),
  //                             LOW    (one at a time),
  //                             HIGH    (cancels lower rankings),
  // Find a free channel 
  for (int i = 0; i < NUM_OF_CHANNELS; i++) {
    if (priority == 0) {
      if (channels[i].getStatus() == sf::SoundSource::Status::Playing) {
        if (channels[i].getBuffer() == &(const sf::SoundBuffer)sources[type]) {
          // Lowest priority sounds only play once 
          return -1;
        }
      }

    } else if (channels[i].getStatus() != sf::SoundSource::Status::Playing) {
      // Check if this is the same type
      if (channels[i].getBuffer() != &(const sf::SoundBuffer)sources[type]) {
        channels[i].setBuffer(sources[type]);
        channels[i].play();
        return 0;
      }
    }
  }

  // No free channel? Skip playing this sound.
  return -1;
}

int AudioResourceManager::Stream(std::string path, bool loop) {
  // stop previous stream if any 
  stream.stop();

  if (!stream.openFromFile(path))
    return -1; // error

  stream.play();
  stream.setLoop(loop);

  return 0;
}

void AudioResourceManager::StopStream() {
  stream.stop();
}

void AudioResourceManager::SetStreamVolume(float volume) {
  stream.setVolume(volume);
}

void AudioResourceManager::SetChannelVolume(float volume) {
  for (int i = 0; i < NUM_OF_CHANNELS; i++) {
    channels[i].setVolume(volume);
  }
}
