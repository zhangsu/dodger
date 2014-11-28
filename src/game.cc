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
      god_mode_(false) {
    Terrain* terrain = new Terrain("data/images/heightmap.png");
    scene_root_.attach(terrain);
    scene_root_.attach(player_);
    terrain->scale(0.5, 20, 0.5);
    // Center the terrain in world space.
    terrain->translate(-terrain->width() / 2.0, 0, -terrain->height() / 2.0);

    player_->translate(0, 5, 0);
    player_->attach(new Spirit());
    player_->attach(camera_);
    camera_->translate(0, 0, 10);

    Light* sun = new Light(vec3(0.2, 0.2, 0.2), vec3(0.1, 0, 0));
    sun->translate(0, 100, 0);
    addLight(&scene_root_, sun);

    addLight(player_, new Light(vec3(0.05, 0.1, 1.0), vec3(0.1, 0.01, 0.01)));
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

void Game::toggleGodMode() {
    god_mode_ ^= true;
    if (god_mode_) {
        mat4 world_trans = camera_->worldTrans();
        player_->detach(camera_);
        camera_->set_trans(world_trans);
    } else {
        camera_->resetTrans();
        player_->attach(camera_);
        camera_->translate(0, 0, 10);
    }
}

mat4 Game::viewTrans() const {
    return glm::inverse(camera_->worldTrans());
}

const SceneNode& Game::scene_root() const {
    return scene_root_;
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
