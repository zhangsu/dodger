#include "renderer.hh"

// Public methods.

Renderer::Renderer(const Game& game)
    : game_(game),
      drawing_wireframe_(false),
      culling_backface_(true) {}

void Renderer::render() {
    clear();
    game_.scene().render(*this);
}

void Renderer::toggleWireframe() {
    drawing_wireframe_ ^= true;
}

void Renderer::toggleBackfaceCulling() {
    culling_backface_ ^= true;
}
