#include "renderer.hh"

// Public methods.

Renderer::Renderer(const Game& game) : game_(game), draw_wireframe_(false) {}

void Renderer::render() const {
    clear();
    game_.scene_root().render(*this);
}

void Renderer::toggleWireframe() {
    draw_wireframe_ ^= true;
}
