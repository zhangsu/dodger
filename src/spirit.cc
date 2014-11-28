#include "material.hh"
#include "renderer.hh"
#include "spirit.hh"

using std::string;
using glm::mat4;
using glm::vec3;

// Public methods.

Spirit::Spirit()
    : Primitive(Material(vec3(0, 0, 0), vec3(0.5, 0.5, 0.5), 0.5)) {}

void Spirit::render(const Renderer& renderer, mat4 trans) const {
    mat4 stack = trans * this->trans();
    renderer.render(*this, stack);
    renderChildren(renderer, stack);
}
