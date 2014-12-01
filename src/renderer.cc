#include "renderer.hh"

// Public methods.

Renderer::Renderer(const Game& game)
    : game_(game),
      drawing_wireframe_(false),
      culling_backface_(true),
      drawing_shadow_(true),
      drawing_terrain_shadow_(false) {}

Renderer::~Renderer() {}

void Renderer::render() {
    if (drawing_shadow_) {
        prepareShadowRendering();
        clear();
        game_.scene().renderShadow(*this);
    }
    prepareRendering();
    clear();
    game_.scene().render(*this);
}

void Renderer::toggleWireframe() {
    drawing_wireframe_ ^= true;
}

void Renderer::toggleBackfaceCulling() {
    culling_backface_ ^= true;
}

void Renderer::toggleShadow() {
    drawing_shadow_ ^= true;
}

void Renderer::toggleTerrainShadow() {
    drawing_terrain_shadow_ ^= true;
}

