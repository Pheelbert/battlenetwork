#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Clock;
using sf::Event;

#include "bnGame.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnMemory.h"
#include "bnMettaur.h"
#include "bnBackgroundUI.h"
#include "bnPlayerHealthUI.h"
#include "bnResourceManager.h"
#include "bnRenderer.h"

int Game::Run()
{
	Renderer::GetInstance().Initialise();
	ResourceManager::GetInstance().LoadAllTextures();

	Field* field(new Field(6, 3));
	//TODO: just testing states here, remove later
	field->GetAt(1, 1)->SetState(TileState::CRACKED);
	field->GetAt(3, 1)->SetState(TileState::CRACKED);
	field->GetAt(3, 2)->SetState(TileState::CRACKED);
	field->GetAt(3, 3)->SetState(TileState::CRACKED);

	//TODO: More dynamic way of handling entities
	//(for now there's only 1 battle and you start straight in it)
	Entity* player(new Player());
	field->AddEntity(player, 2, 2);
	Entity* mob(new Mettaur());
	field->AddEntity(mob, 5, 2);

	BackgroundUI background = BackgroundUI();

	Clock clock;
	float elapsed = 0.0f;
	while (Renderer::GetInstance().Running())
	{
		clock.restart();
		field->Update(elapsed);

		Renderer::GetInstance().Clear();

		background.Draw();

        Tile* tile = nullptr;
		while (field->GetNextTile(tile))
		{
			Renderer::GetInstance().Pose(tile);
		}

		for (int d = 1; d <= field->GetHeight(); d++)
		{
			Entity* entity = nullptr;
			while (field->GetNextEntity(entity, d))
			{
				if (!entity->IsDeleted())
				{
					Renderer::GetInstance().Push(entity);
					Renderer::GetInstance().Lay(entity->GetMiscComponents());
				}
			}
		}

		Renderer::GetInstance().DrawUnderlay();
		Renderer::GetInstance().DrawLayers();
		Renderer::GetInstance().DrawOverlay();
		Renderer::GetInstance().Display();

		elapsed = static_cast<float>(clock.getElapsedTime().asMilliseconds());
	}

	return EXIT_SUCCESS;
}