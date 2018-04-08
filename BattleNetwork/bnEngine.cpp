#include "bnEngine.h"

Engine& Engine::GetInstance()
{
    static Engine instance;
    return instance;
}

void Engine::Initialize()
{
    window = new RenderWindow(VideoMode(480, 320), "Battle Network : Prototype");
    window->setFramerateLimit(60);
}

void Engine::Draw(Drawable& _drawable)
{
    window->draw(_drawable);
}

void Engine::Draw(Drawable* _drawable)
{
    if (_drawable) window->draw(*_drawable);
}

void Engine::Draw(vector<Drawable*> _drawable)
{
    auto it = _drawable.begin();
    for(it; it != _drawable.end(); ++it)
    {
        Draw(*it);
    }
}

void Engine::Display()
{
    window->display();
}

bool Engine::Running()
{
    return window->isOpen();
}

void Engine::Clear()
{
    underlay.Clear();
    layers.Clear();
    overlay.Clear();
    window->clear();
}

RenderWindow* Engine::GetWindow() const
{
    return window;
}

Engine::Engine(void)
    : layers(Layers()),
      overlay(Overlay()),
      underlay(Underlay())
{
}

Engine::~Engine(void)
{
    delete window;
}

void Engine::Push(LayeredDrawable* _drawable)
{
    if (_drawable)
    {
        layers.Insert(_drawable);
    }
}

void Engine::Lay(Drawable* _drawable)
{
    if (_drawable)
    {
        overlay.Push(_drawable);
    }
}

void Engine::Lay(vector<Drawable*> _drawable)
{
    auto it = _drawable.begin();
    for(it; it != _drawable.end(); ++it)
    {
        if (*it)
        {
            overlay.Push(*it);
        }
    }
}

void Engine::LayUnder(Drawable* _drawable)
{
    if (_drawable)
    {
        underlay.Push(_drawable);
    }
}

void Engine::DrawLayers()
{
    for (int i = layers.min; i <= layers.max; i++)
    {
        Draw(layers.At(i));
    }
}

void Engine::DrawOverlay()
{
    Draw(overlay);
}

void Engine::DrawUnderlay()
{
    Draw(underlay);
}