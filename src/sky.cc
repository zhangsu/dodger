#include "renderer.hh"
#include "sky.hh"

using glm::mat4;
using glm::vec3;
using glm::vec4;

// Public methods.

Sky::Sky() {}

void Sky::render(Renderer& renderer, mat4 trans) const {
    mat4 stack = trans * this->trans();
    renderer.render(*this, stack);
    renderChildren(renderer, stack);
}
