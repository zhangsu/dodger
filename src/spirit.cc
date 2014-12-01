#include <cstdlib>
#include "audio_renderer.hh"
#include "gameover.hh"
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
      terrain_(terrain) {}

void Spirit::translate(float x, float z) {
    SceneNode::translate(x, 0, z);

    vec3 position = this->position(terrain_);
    vec3 up(0.0f, 1.0f, 0.0f);
    vec3 normal = terrain_->transformedNormal(position.x, position.z);

    if (glm::dot(up, normal) / glm::length(up) / glm::length(normal) < 0.7f
        || colliding()) {
        // Current terrain is too steep or colliding with others, fallback.
        SceneNode::translate(-x, 0, -z);
    } else {
        updateAltitude();
    }
}

void Spirit::walk() {
    bool turning = (rand() % 3 == 0);
    if (turning) {
        bool turningLeft = (rand() % 2 == 0);
        float angle = ((float) rand()) / RAND_MAX;
        if (turningLeft)
            rotate(angle, 0, 1, 0);
        else
            rotate(-angle, 0, 1, 0);
    }
    bool moving = (rand() % 3 != 0);
    if (moving) {
        translate(0, -0.4f);
    }
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

void Spirit::renderAudio(AudioRenderer& renderer, mat4 trans) const {
    mat4 stack = trans * this->trans();
    renderer.render(*this, stack);
    renderChildrenAudio(renderer, trans * this->trans());
}

bool Spirit::collideWith(const SceneNode* node) const {
    const Spirit* spirit;
    if (node == this) {
        return false;
    } else if ((spirit = dynamic_cast<const Spirit*>(node)) != nullptr) {

        bool colliding = glm::length(this->position() - node->position()) < 2;
        if (colliding) {
            if (spirit->is_player() || this->is_player())
                throw Gameover();
            return true;
        } else {
            return false;
        }
    }
    return false;
}

unsigned Spirit::id() const {
    return id_;
}

unsigned Spirit::is_player() const {
    return is_player_;
}

// Private methods.

void Spirit::updateAltitude() {
    vec3 position = this->position(terrain_);
    // Move spirit on terrain.
    trans_[3][1] = terrain_->height(position.x, position.z) + scaling().y * 1.3;
}
