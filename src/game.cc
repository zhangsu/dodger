#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game.hh"
#include "spirit.hh"
#include "terrain.hh"

using glm::vec3;

// Public methods.

Game::Game()
    : camera_(new SceneNode()),
      player_(new SceneNode()) {
    Terrain* terrain = new Terrain("data/images/heightmap.png");
    scene_root_.addChild(terrain);
    scene_root_.addChild(player_);
    terrain->scale(vec3(0.5, 20, 0.5));
    // Center the terrain in world space.
    terrain->translate(vec3(-terrain->width() / 2.0, 0,
                            -terrain->height() / 2.0));

    player_->translate(vec3(0, 5, 0));
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

void Game::turn(float angle) {
    player_->rotate(angle, vec3(0, 1, 0));
}

glm::mat4 Game::viewTrans() const {
    return glm::inverse(camera_->cumulativeTransformation());
}

const SceneNode& Game::scene_root() const {
    return scene_root_;
}
