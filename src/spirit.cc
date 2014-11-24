#include "renderer.hh"
#include "spirit.hh"

using std::string;
using glm::mat4;

// Public methods.

Spirit::Spirit() {}

void Spirit::render(const Renderer& renderer, mat4 transformations) const {
    mat4 stack = transformations * transformations_;
    renderer.render(*this, stack);
    for (auto child : children_)
        child->render(renderer, stack);
}
