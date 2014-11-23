#include "renderer.hh"

// Public methods.

Renderer::Renderer(const Game& game) : game_(game) {}

void Renderer::render() const {
    clear();
    game_.scene_root()->render(*this);
}
