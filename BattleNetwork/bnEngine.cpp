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
    window->draw(_drawable, state);
}

void Engine::Draw(Drawable* _drawable)
{
    if (_drawable) window->draw(*_drawable, state);
}

void Engine::Draw(vector<LayeredDrawable*> _drawable)
{
    auto it = _drawable.begin();
    for(it; it != _drawable.end(); ++it)
    {
		/*
		NOTE: Could add support for multiple layers:
		sf::RenderTexture image1;
		sf::RenderTexture image2;

		sf::RenderTexture* front = &image1;
		sf::RenderTexture* back = &image2;

		// draw the initial scene into "back"
		...

		for (std::vector<sf::Shader>::iterator it = shaders.begin(); it != shaders.end(); ++it)
		{
		// draw "back" into "front"
		front->clear();
		front->draw(sf::Sprite(back->getTexture()), &*it);
		front->display();

		// swap front and back buffers
		std::swap(back, front)
		}

		*/

		// For now, support at most one shader.
		// Grab the shader and image, apply to a new render target, pass this render target into Draw()

		LayeredDrawable* context = *it;
		sf::Shader* shader = context->GetShader();

		if (shader != nullptr) {
			sf::RenderTexture* postFX = new sf::RenderTexture();
			const sf::Texture* original = context->getTexture();
			postFX->create(original->getSize().x,  original->getSize().y);
			postFX->draw(sf::Sprite(*context->getTexture()), shader); // bake
			(*it)->setTexture(postFX->getTexture());
			Draw(*it);
			(*it)->setTexture(*original);
			delete postFX;
		}
		else {
			Draw(context);
		}
    }
}

void Engine::Draw(vector<Drawable*> _drawable)
{
	auto it = _drawable.begin();
	for (it; it != _drawable.end(); ++it)
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

void Engine::SetShader(sf::Shader* shader) {
	state.shader = shader;
}