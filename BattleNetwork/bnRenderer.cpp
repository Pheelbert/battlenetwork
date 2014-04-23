#include "bnRenderer.h"

Renderer& Renderer::GetInstance()
{
	static Renderer instance;
	return instance;
}

void Renderer::Initialise()
{
	window = new RenderWindow(VideoMode(480, 320), "Battle Network : Prototype");
	window->setFramerateLimit(60);
}

void Renderer::Draw(Drawable& _drawable)
{
	window->draw(_drawable);
}

void Renderer::Draw(Drawable* _drawable)
{
	if (_drawable) window->draw(*_drawable);
}

void Renderer::Draw(vector<Drawable*> _drawable)
{
	auto it = _drawable.begin();
	for(it; it != _drawable.end(); ++it)
	{
		Draw(*it);
	}
}

void Renderer::Display()
{
	window->display();
}

bool Renderer::Running()
{
	return window->isOpen();
}

void Renderer::Clear()
{
	underlay.Clear();
	layers.Clear();
	overlay.Clear();
	window->clear();
}

RenderWindow* Renderer::GetWindow() const
{
	return window;
}

Renderer::Renderer(void)
    : layers(Layers()),
      overlay(Overlay()),
      underlay(Underlay())
{
}

Renderer::~Renderer(void)
{
	delete window;
}

void Renderer::Push(LayeredDrawable* _drawable)
{
    if (_drawable)
    {
        layers.Insert(_drawable);
    }
}

void Renderer::DrawLayers()
{
	for (int i = layers.min; i <= layers.max; i++)
	{
		Draw(layers.At(i));
	}
}

void Renderer::Lay(Drawable* _drawable)
{
    if (_drawable)
    {
        overlay.Push(_drawable);
    }
}

void Renderer::Lay(vector<Drawable*> _drawable)
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

void Renderer::DrawOverlay()
{
	Draw(overlay);
}

void Renderer::Pose(Drawable* _drawable)
{
    if (_drawable)
    {
        underlay.Push(_drawable);
    }
}

void Renderer::DrawUnderlay()
{
	Draw(underlay);
}