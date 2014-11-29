#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game.hh"
#include "sky.hh"

using std::vector;
using glm::mat4;
using glm::vec3;

// Public methods.

Game::Game()
    : ambient_(0.3f),
      terrain_(new Terrain("data/images/heightmap.png")),
      camera_(new SceneNode()),
      player_(new Spirit(terrain_)),
      camera_distance_(10),
      god_mode_(false) {
    // Set up sky.
    Sky* sky = new Sky();
    scene_.attach(sky);
    sky->translate(-2500, -2500, -2500);
    sky->scale(5000, 5000, 5000);

    // Set up terrain.
    scene_.attach(terrain_);
    terrain_->scale(0.5, 20, 0.5);
    // Center the terrain in world space.
    terrain_->translate(-terrain_->width() / 2.0, 0, -terrain_->height() / 2.0);

    // Set up player.
    scene_.attach(player_);
    // Update player's y position immediately.
    player_->translate(0, 0);
    addLight(player_, new Light(vec3(0.05, 0.1, 1.0), vec3(0.1, 0.01, 0.01)));

    // Set up camera.
    player_->attach(camera_);
    updateCameraTrans();

    // Add a global sunlight.
    Light* sun = new Light(vec3(0.1, 0.1, 0.1), vec3(0.1, 0, 0));
    sun->translate(0, 100, 0);
    addLight(&scene_, sun);
}

void Game::move(float x, float y, float z) {
    move(vec3(x, y, z));
}

void Game::move(vec3 translation) {
    if (god_mode_) {
        camera_->translate(5.0f * translation);
    } else {
        player_->translate(translation.x, translation.z);
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

void Game::rotateCamera(float dx, float dy) {
    float angle = glm::radians(120.0f);
    if (god_mode_) {
        camera_->rotate(-dx * angle, 0, 1, 0);
        camera_->rotate(-dy * angle, 1, 0, 0);
    } else {
        camera_angles_.x += dx * angle;
        camera_angles_.y += -dy * angle;
        updateCameraTrans();
    }
}

void Game::rotatePlayer(float dx, float dy) {
    float camera_angle = glm::radians(60.0f);
    if (god_mode_) {
        camera_->rotate(-dx * camera_angle, 0, 1, 0);
        camera_->rotate(-dy * camera_angle, 1, 0, 0);
    } else {
        player_->rotate(-dx * glm::radians(120.0f), 0, 1, 0);
        // Camera y position can be rotated at the same time, but not x.
        camera_angles_.y += -dy * camera_angle;
        updateCameraTrans();
    }
}

void Game::lookAtPlayer() {
    if (god_mode_)
        return;
    camera_angles_.x = 0;
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
    camera_angles_.y = std::min(std::max(camera_angles_.y, -0.5f), 0.1f);
    camera_->resetTrans();
    // Rotate around camera's, also the player's origin.
    camera_->rotate(camera_angles_.x, 0, 1, 0);
    camera_->rotate(camera_angles_.y, 1, 0, 0);
    camera_->translate(0, 0, camera_distance_);
}
