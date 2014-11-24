#include "renderer.hh"
#include "spirit.hh"

using std::string;
using glm::mat4;

// Public methods.

Spirit::Spirit() {}

void Spirit::render(const Renderer& renderer, mat4 transformation) const {
    mat4 stack = transformation * transformation_;
    renderer.render(*this, stack);
    for (auto child : children_)
        child->render(renderer, stack);
}
