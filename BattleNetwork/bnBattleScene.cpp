#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Clock;
using sf::Event;

#include "bnBattleScene.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnMemory.h"
#include "bnMettaur.h"
#include "bnProgsMan.h"
#include "bnBackgroundUI.h"
#include "bnPlayerHealthUI.h"
#include "bnResourceManager.h"
#include "bnEngine.h"

int main()
{
    return BattleScene::Run();
}

int BattleScene::Run()
{
    Engine::GetInstance().Initialise();
    ResourceManager::GetInstance().LoadAllTextures();

    Field* field(new Field(6, 3));
    //TODO: just testing states here, remove later
    field->GetAt(3, 1)->SetState(TileState::CRACKED);
	field->GetAt(1, 1)->SetState(TileState::EMPTY);
	field->GetAt(1, 2)->SetState(TileState::EMPTY);
	field->GetAt(1, 3)->SetState(TileState::EMPTY);
    field->GetAt(6, 1)->SetState(TileState::CRACKED);
    field->GetAt(6, 2)->SetState(TileState::CRACKED);
    field->GetAt(6, 3)->SetState(TileState::CRACKED);

    //TODO: More dynamic way of handling entities
    //(for now there's only 1 battle and you start straight in it)
    Entity* player(new Player());
    field->AddEntity(player, 2, 2);
	Entity* mob(new ProgsMan());
	field->AddEntity(mob, 5, 2);

    /*Entity* mob(new Mettaur());
    field->AddEntity(mob, 6, 2);
    Entity* mob2(new Mettaur());
    field->AddEntity(mob2, 4, 2);
	Entity* mob3(new Mettaur());
	field->AddEntity(mob3, 6, 1);*/

    BackgroundUI background = BackgroundUI();

    Clock clock;
    float elapsed = 0.0f;
    while (Engine::GetInstance().Running())
    {
        clock.restart();
        field->Update(elapsed);

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
        Engine::GetInstance().Display();

        elapsed = static_cast<float>(clock.getElapsedTime().asMilliseconds());
    }

    return EXIT_SUCCESS;
}