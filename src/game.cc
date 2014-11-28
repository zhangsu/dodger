#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game.hh"
#include "spirit.hh"
#include "terrain.hh"

using std::vector;
using glm::mat4;
using glm::vec3;

// Public methods.

Game::Game()
    : ambient_(0.3f),
      camera_(new SceneNode()),
      player_(new SceneNode()),
      camera_distance_(10),
      god_mode_(false) {
    // Set up terrain.
    Terrain* terrain = new Terrain("data/images/heightmap.png");
    scene_.attach(terrain);
    terrain->scale(0.5, 20, 0.5);
    // Center the terrain in world space.
    terrain->translate(-terrain->width() / 2.0, 0, -terrain->height() / 2.0);

    // Set up player.
    scene_.attach(player_);
    player_->translate(0, 5, 0);
    player_->attach(new Spirit());
    addLight(player_, new Light(vec3(0.05, 0.1, 1.0), vec3(0.1, 0.01, 0.01)));

    // Set up camera.
    player_->attach(camera_);
    updateCameraTrans();

    // Add a global sunlight.
    Light* sun = new Light(vec3(0.2, 0.2, 0.2), vec3(0.1, 0, 0));
    sun->translate(0, 100, 0);
    addLight(&scene_, sun);
}

void Game::move(float x, float y, float z) {
    move(vec3(x, y, z));
}

void Game::move(vec3 translation) {
    if (god_mode_) {
        camera_->translate(translation);
    } else {
        player_->translate(translation);
    }
}

void Game::turn(float angle) {
    if (god_mode_) {
        camera_->rotate(angle, 0, 1, 0);
    } else {
        player_->rotate(angle, 0, 1, 0);
    }
}

void Game::zoom(float distance) {
    camera_distance_ += distance;
    camera_distance_ = std::min(20.0f, std::max(-1.0f, camera_distance_));
    if (!god_mode_)
        updateCameraTrans();
}

void Game::toggleGodMode() {
    god_mode_ ^= true;
    if (god_mode_) {
        mat4 world_trans = camera_->worldTrans();
        player_->detach(camera_);
        camera_->set_trans(world_trans);
    } else {
        player_->attach(camera_);
        updateCameraTrans();
    }
}

mat4 Game::viewTrans() const {
    return glm::inverse(camera_->worldTrans());
}

const SceneNode& Game::scene() const {
    return scene_;
}

const SceneNode* Game::camera() const {
    return camera_;
}

const vector<const Light*> Game::lights() const {
    return lights_;
}

float Game::ambient() const {
    return ambient_;
}

// Private methods.

void Game::addLight(SceneNode* parent, Light* light) {
    lights_.push_back(light);
    parent->attach(light);
}

void Game::updateCameraTrans() {
    camera_->resetTrans();
    camera_->rotate(-camera_distance_ * 0.01, 1, 0, 0);
    camera_->translate(0, 0, camera_distance_);
}
