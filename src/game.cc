#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game.hh"
#include "spirit.hh"
#include "terrain.hh"

using glm::vec3;

// Public methods.

Game::Game()
    : camera_(new SceneNode()),
      player_(new SceneNode()),
      scene_root_(new SceneNode()) {

    scene_root_->addChild(new Terrain("data/images/heightmap.png"));
    scene_root_->addChild(player_);
    player_->addChild(new Spirit());
    player_->addChild(camera_);
    camera_->translate(vec3(0, 0, 10));
}

void Game::move(float x, float y, float z) {
    move(vec3(x, y, z));
}

void Game::move(vec3 translation) {
    player_->translate(translation);
}

glm::mat4 Game::viewTrans() const {
    return glm::inverse(camera_->cumulativeTransformation());
}

SceneNode* Game::scene_root() const {
    return scene_root_;
}
