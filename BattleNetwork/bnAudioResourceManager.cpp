#include "bnAudioResourceManager.h"

AudioResourceManager& AudioResourceManager::GetInstance()
{
	static AudioResourceManager instance;
	return instance;
}

AudioResourceManager::AudioResourceManager()
{
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


AudioResourceManager::~AudioResourceManager()
{
	// Stop playing everything 
	stream.stop();

	for (int i = 0; i < NUM_OF_CHANNELS; i++) {
		channels[i].stop();
	}

	// Free memory
	delete[] channels;
	delete[] sources;
}

// TODO: Use logger to log these paths for debugging 
void AudioResourceManager::LoadAllSources()
{
	// APPEAR
	sources[AudioType::APPEAR].loadFromFile("resources/sfx/appear.ogg");

	// AREA_GRAB
	sources[AudioType::AREA_GRAB].loadFromFile("resources/sfx/area_grab.ogg");

	// AREA_GRAB_TOUCHDOW
	sources[AudioType::AREA_GRAB_TOUCHDOWN].loadFromFile("resources/sfx/area_grab_touchdown.ogg");

	// BUSTER_CHARGED
	sources[AudioType::BUSTER_CHARGED].loadFromFile("resources/sfx/buster_charged.ogg");

	// BUSTER_CHARGING
	sources[AudioType::BUSTER_CHARGING].loadFromFile("resources/sfx/buster_charging.ogg");

	// CANNON
	sources[AudioType::CANNON].loadFromFile("resources/sfx/cannon.ogg");

	// CHIP_CANCEL
	sources[AudioType::CHIP_CANCEL].loadFromFile("resources/sfx/chip_cancel.ogg");

	// CHIP_CHOOSE
	sources[AudioType::CHIP_CHOOSE].loadFromFile("resources/sfx/chip_choose.ogg");

	// CHIP_CONFIRM
	sources[AudioType::CHIP_CONFIRM].loadFromFile("resources/sfx/chip_confirm.ogg");

	// CHIP_DESC
	sources[AudioType::CHIP_DESC].loadFromFile("resources/sfx/chip_desc.ogg");

	// CHIP_DESC_CLOSE
	sources[AudioType::CHIP_DESC_CLOSE].loadFromFile("resources/sfx/chip_desc_close.ogg");

	// CHIP_SELECT
	sources[AudioType::CHIP_SELECT].loadFromFile("resources/sfx/chip_select.ogg");

	// CUSTOM_BAR_FULL
	sources[AudioType::CUSTOM_BAR_FULL].loadFromFile("resources/sfx/custom_bar_full.ogg");

	// DELETED
	sources[AudioType::DELETED].loadFromFile("resources/sfx/deleted.ogg");

	// EXPLODE
	sources[AudioType::EXPLODE].loadFromFile("resources/sfx/explode.ogg");

	// GUN
	sources[AudioType::GUN].loadFromFile("resources/sfx/gun.ogg");

	// HEALTH_ALERT
	sources[AudioType::HEALTH_ALERT].loadFromFile("resources/sfx/health_alert.ogg");

	// HURT
	sources[AudioType::HURT].loadFromFile("resources/sfx/hurt.ogg");

	// PANEL_CRACK
	sources[AudioType::PANEL_CRACK].loadFromFile("resources/sfx/panel_crack.ogg");

	// PANEL_RETURN
	sources[AudioType::PANEL_RETURN].loadFromFile("resources/sfx/panel_return.ogg");

	// PAUSE
	sources[AudioType::PAUSE].loadFromFile("resources/sfx/pause.ogg");

	// PRE_BATTLE
	sources[AudioType::PRE_BATTLE].loadFromFile("resources/sfx/pre_battle.ogg");

	// RECOVER
	sources[AudioType::RECOVER].loadFromFile("resources/sfx/recover.ogg");

	// SPREADER
	sources[AudioType::SPREADER].loadFromFile("resources/sfx/spreader.ogg");

	// SWORD_SWING
	sources[AudioType::SWORD_SWING].loadFromFile("resources/sfx/sword_swing.ogg");

	// TOSS_ITEM
	sources[AudioType::TOSS_ITEM].loadFromFile("resources/sfx/toss_item.ogg");

	// WAVE
	sources[AudioType::WAVE].loadFromFile("resources/sfx/wave.ogg");

}

int AudioResourceManager::Play(AudioType type, int priority)
{
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

		}
		if (channels[i].getStatus() != sf::SoundSource::Status::Playing) {
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

int AudioResourceManager::Stream(std::string path, bool loop)
{
	// stop previous stream if any 
	stream.stop();

	if (!stream.openFromFile(path))
		return -1; // error

	stream.play();
	stream.setLoop(loop);

	return 0;
}

void AudioResourceManager::SetStreamVolume(int volume) {
	stream.setVolume(volume);
}

void AudioResourceManager::SetChannelVolume(int volume) {
	for (int i = 0; i < NUM_OF_CHANNELS; i++) {
		channels[i].setVolume(volume);
	}
}
