#include "bnEngine.h"
#include <time.h>       /* time */

Engine& Engine::GetInstance() {
  static Engine instance;
  return instance;
}

void Engine::Initialize() {
  camera = sf::View(sf::Vector2f(240, 160), sf::Vector2f(480, 320));
  original = camera; // never changes 
  window = new RenderWindow(VideoMode(camera.getSize().x, camera.getSize().y), "Battle Network : Prototype");
  window->setFramerateLimit(60);
  postprocessing.create(camera.getSize().x, camera.getSize().y); // Same as display

  // See the random generator with current time
  srand(time(0));
}

void Engine::Draw(Drawable& _drawable, bool applyShaders) {
  if (applyShaders) {
    postprocessing.draw(_drawable, state);
  } else {
    postprocessing.draw(_drawable);
  }
}

void Engine::Draw(Drawable* _drawable, bool applyShaders) {
  if (!_drawable) {
    return;
  }

  if (applyShaders) {
    postprocessing.draw(*_drawable, state);
  } else {
    postprocessing.draw(*_drawable);
  }
}

void Engine::Draw(LayeredDrawable* _drawable) {
  // For now, support at most one shader.
  // Grab the shader and image, apply to a new render target, pass this render target into Draw()

  LayeredDrawable* context = _drawable;
  sf::Shader* shader = context->GetShader();

  sf::Vector2f originalPos = context->getPosition();
  context->move(GetViewOffset());

  if (shader != nullptr) {
    const sf::Texture* original = context->getTexture();
    postprocessing.draw(*context, shader); // bake
  } else {
    Draw(context, true);
  }

  context->setPosition(originalPos);
}
void Engine::Draw(vector<LayeredDrawable*> _drawable) {
  auto it = _drawable.begin();
  for (it; it != _drawable.end(); ++it) {
    /*
    NOTE: Could add support for multiple shaders:
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
      sf::Vector2f originalPos = context->getPosition();
      context->move(GetViewOffset());
      postprocessing.draw(*context, shader); // bake
      context->setPosition(originalPos);
    } else {
      sf::Vector2f originalPos = context->getPosition();
      context->move(GetViewOffset());
      Draw(context, true);
      context->setPosition(originalPos);
    }
  }
}

void Engine::Draw(vector<Drawable*> _drawable, bool applyShaders) {
  auto it = _drawable.begin();
  for (it; it != _drawable.end(); ++it) {
    Draw(*it, applyShaders);
  }
}

void Engine::Display() {
  // flip and ready buffer
  postprocessing.display();
  // Capture buffer in a drawable context
  sf::Sprite postFX(postprocessing.getTexture());
  // drawbuffer on top of the scene
  window->draw(postFX);
  // show final result
  window->display();
  // Prepare buffer for next cycle
  postprocessing.clear(sf::Color::Transparent);
}

bool Engine::Running() {
  return window->isOpen();
}

void Engine::Clear() {
  underlay.Clear();
  layers.Clear();
  overlay.Clear();
  window->clear();
}

RenderWindow* Engine::GetWindow() const {
  return window;
}

Engine::Engine(void)
  : layers(Layers()),
  overlay(Overlay()),
  underlay(Underlay()) {
}

Engine::~Engine(void) {
  delete window;
}

const sf::Vector2f Engine::GetViewOffset() {
  return GetDefaultView().getCenter() - camera.getCenter();
}

void Engine::Push(LayeredDrawable* _drawable) {
  if (_drawable) {
    layers.Insert(_drawable);
  }
}

void Engine::Lay(LayeredDrawable* _drawable) {
  if (_drawable) {
    overlay.Push(_drawable);
  }
}

void Engine::Lay(vector<sf::Drawable*> _drawable) {
  auto it = _drawable.begin();
  for (it; it != _drawable.end(); ++it) {
    if (*it) {
      overlay.Push(*it);
    }
  }
}

void Engine::LayUnder(sf::Drawable* _drawable) {
  if (_drawable) {
    underlay.Push(_drawable);
  }
}

void Engine::DrawLayers() {
  for (int i = layers.min; i <= layers.max; i++) {
    Draw(layers.At(i));
  }
}

void Engine::DrawOverlay() {
  Draw(overlay, false);
}

void Engine::DrawUnderlay() {
  Draw(underlay);
}

void Engine::SetShader(sf::Shader* shader) {

  if (shader == nullptr) {
    state = sf::RenderStates::Default;
  } else {
    state.shader = shader;
  }
}

void Engine::RevokeShader() {
  SetShader(nullptr);
}

const sf::View Engine::GetDefaultView() {
  return original;
}

void Engine::SetView(sf::View camera) {
  this->camera = camera;
}
