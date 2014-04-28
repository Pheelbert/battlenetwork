#include <SFML\Window.hpp>
using sf::Event;
using sf::Keyboard;
#include "bnEngine.h"
#include "bnControllableComponent.h"
#include "bnDirection.h"

ControllableComponent& ControllableComponent::GetInstance()
{
    static ControllableComponent instance;
    return instance;
}

ControllableComponent::ControllableComponent()
    : events(vector<ControllableEvent>())
{
}


ControllableComponent::~ControllableComponent()
{
}

void ControllableComponent::update()
{
    this->events.clear();
    Event event;
    while (Engine::GetInstance().GetWindow()->pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            Engine::GetInstance().GetWindow()->close();
        }

        if (Event::KeyPressed == event.type)
        {
            if (Keyboard::Up == event.key.code)
            {
                events.push_back(PRESSED_UP);
            }
            else if (Keyboard::Left == event.key.code)
            {
                events.push_back(PRESSED_LEFT);
            }
            else if (Keyboard::Down == event.key.code)
            {
                events.push_back(PRESSED_DOWN);
            }
            else if (Keyboard::Right == event.key.code)
            {
                events.push_back(PRESSED_RIGHT);
            }
            else if (Keyboard::Space == event.key.code)
            {
                events.push_back(PRESSED_SPACE);
            }
        }

        if (Event::KeyReleased == event.type)
        {
            if (Keyboard::Up == event.key.code)
            {
                events.push_back(RELEASED_UP);
            }
            else if (Keyboard::Left == event.key.code)
            {
                events.push_back(RELEASED_LEFT);
            }
            else if (Keyboard::Down == event.key.code)
            {
                events.push_back(RELEASED_DOWN);
            }
            else if (Keyboard::Right == event.key.code)
            {
                events.push_back(RELEASED_RIGHT);
            }
            else if (Keyboard::Space == event.key.code)
            {
                events.push_back(RELEASED_SPACE);
            }
        }
    }   
}

bool ControllableComponent::has(ControllableEvent _event)
{
    return events.end() != find(events.begin(), events.end(), _event);
}

bool ControllableComponent::empty()
{
    return events.empty();
}