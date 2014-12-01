#include "renderer.hh"
#include "spirit.hh"

using std::string;
using glm::mat4;
using glm::vec3;

namespace {

int id_count = 0;

}

// Public methods.

Spirit::Spirit(const Terrain* terrain, bool is_player)
    : id_(id_count++),
      is_player_(is_player),
      terrain_(terrain) {
    translate(0, 0);
}

void Spirit::translate(float x, float z) {
    SceneNode::translate(x, 0, z);
    vec3 position = this->position(terrain_);
    // Move spirit on terrain.
    trans_[3][1] = terrain_->height(position.x, position.z) + scaling().y * 1.3;
}

void Spirit::renderShadow(Renderer& renderer, mat4 trans) const {
    mat4 stack = trans * this->trans();
    renderer.renderShadow(*this, stack);
    renderChildrenShadow(renderer, stack);
}

void Spirit::render(Renderer& renderer, mat4 trans) const {
    mat4 stack = trans * this->trans();
    renderer.render(*this, stack);
    renderChildren(renderer, stack);
}

unsigned Spirit::id() const {
    return id_;
}

unsigned Spirit::is_player() const {
    return is_player_;
}
