#include "material.hh"
#include "renderer.hh"
#include "spirit.hh"

using std::string;
using glm::mat4;
using glm::vec3;

// Public methods.

Spirit::Spirit(const Terrain* terrain)
    : Primitive(Material(vec3(0, 0, 0), vec3(0.5, 0.5, 0.5), 0.5)),
      terrain_(terrain) {}

#include <cstdio>
void Spirit::translate(float x, float z) {
    SceneNode::translate(x, 0, z);
    vec3 position = this->position(terrain_);
    trans_[3][1] = terrain_->height(position.x, position.z) + scaling().y * 1.3;
}

void Spirit::render(const Renderer& renderer, mat4 trans) const {
    mat4 stack = trans * this->trans();
    renderer.render(*this, stack);
    renderChildren(renderer, stack);
}
