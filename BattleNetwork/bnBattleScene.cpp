#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Clock;
using sf::Event;
using sf::Font;

#include "bnBattleScene.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnMemory.h"
#include "bnMettaur.h"
#include "bnProgsMan.h"
#include "bnBackgroundUI.h"
#include "bnPlayerHealthUI.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnControllableComponent.h"
#include "bnEngine.h"

int main()
{
    return BattleScene::Run();
}

int BattleScene::Run()
{
    Engine::GetInstance().Initialize();
    TextureResourceManager::GetInstance().LoadAllTextures();
	AudioResourceManager::GetInstance().LoadAllSources();
	AudioResourceManager::GetInstance().SetStreamVolume(15); // 15% 

    Field* field(new Field(6, 3));
    //TODO: just testing states here, remove later
    field->GetAt(3, 1)->SetState(TileState::CRACKED);
	field->GetAt(1, 1)->SetState(TileState::NORMAL);
	field->GetAt(1, 2)->SetState(TileState::NORMAL);
	field->GetAt(1, 3)->SetState(TileState::NORMAL);
    field->GetAt(6, 1)->SetState(TileState::NORMAL);
    field->GetAt(6, 2)->SetState(TileState::NORMAL);
    field->GetAt(6, 3)->SetState(TileState::NORMAL);

    //TODO: More dynamic way of handling entities
    //(for now there's only 1 battle and you start straight in it)
    Entity* player(new Player());
    field->AddEntity(player, 2, 2);
	ProgsMan* boss = new ProgsMan();
	boss->SetTarget(player);
	field->AddEntity(boss, 5, 2);

    /*Entity* mob(new Mettaur());
    field->AddEntity(mob, 6, 2);
    Entity* mob2(new Mettaur());
    field->AddEntity(mob2, 4, 2);
	Entity* mob3(new Mettaur());
	field->AddEntity(mob3, 6, 1);*/

    BackgroundUI background = BackgroundUI();

	sf::Font* font = TextureResourceManager::GetInstance().LoadFontFromFile("resources/fonts/dr_cain_terminal.ttf");
	sf::Text* pauseLabel = new sf::Text("paused", *font);
	pauseLabel->setOrigin(pauseLabel->getLocalBounds().width/2, pauseLabel->getLocalBounds().height / 2);
	pauseLabel->setPosition((sf::Vector2f)((sf::Vector2i)Engine::GetInstance().GetWindow()->getSize()/2));

	// Stream battle music 
	AudioResourceManager::GetInstance().Stream("resources/loops/loop_boss_battle.ogg", true);

    Clock clock;
    float elapsed = 0.0f;
	bool isPaused = false; 
	bool isInChipSelect = false;

	// Special: Load shaders if supported 
	double shaderCooldown = 2000;
	sf::Shader shader;

	// load only the vertex shader
	if (!shader.loadFromFile("resources/shaders/pixel_blur.frag.txt", sf::Shader::Fragment)) {
		// TODO: log error...
	}
	else {
		shader.setParameter("texture", sf::Shader::CurrentTexture);
		shader.setParameter("pixel_threshold", 0); // start
	}


	while (Engine::GetInstance().Running())
	{
		clock.restart();

		// TODO: Do not update when paused or in chip select
		ControllableComponent::GetInstance().update();

		if (!isPaused) {
			field->Update(elapsed);
		}

		Engine::GetInstance().Clear();

		background.Draw();

		Tile* tile = nullptr;
		while (field->GetNextTile(tile))
		{
			Engine::GetInstance().LayUnder(tile);
		}

		for (int d = 1; d <= field->GetHeight(); d++)
		{
			Entity* entity = nullptr;
			while (field->GetNextEntity(entity, d))
			{
				if (!entity->IsDeleted())
				{
					Engine::GetInstance().Push(entity);
					Engine::GetInstance().Lay(entity->GetMiscComponents());
				}
			}
		}

		Engine::GetInstance().DrawUnderlay();
		Engine::GetInstance().DrawLayers();
		Engine::GetInstance().DrawOverlay();

		if (isPaused) {
			Engine::GetInstance().Draw(pauseLabel);
		}

		// Write contents to screen (always last step)
		Engine::GetInstance().Display();

		// Scene keyboard controls
		if (ControllableComponent::GetInstance().has(PRESSED_PAUSE)) {
			isPaused = !isPaused;
		}

		elapsed = static_cast<float>(clock.getElapsedTime().asMilliseconds());

		shaderCooldown -= elapsed;

		if (shaderCooldown < 0) {
			shaderCooldown = 0;
		}

		shader.setParameter("pixel_threshold", shaderCooldown);
    }

	delete pauseLabel;
	delete font;
	
    return EXIT_SUCCESS;
}