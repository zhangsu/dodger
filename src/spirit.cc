#include "renderer.hh"
#include "spirit.hh"

using std::string;
using glm::mat4;

// Public methods.

Spirit::Spirit() {}

void Spirit::render(const Renderer& renderer, mat4 trans) const {
    mat4 stack = trans * transformation();
    renderer.render(*this, stack);
    renderChildren(renderer, stack);
}
