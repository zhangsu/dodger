#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game.hh"
#include "terrain.hh"

using glm::vec3;

// Public methods.

Game::Game()
    : pos_(vec3(0, 0, 8)),
      eye_pos_(vec3(0, 0, 10)),
      eye_dir_(vec3(0.0f, 0.0f, -1.0f)),
      scene_root_(new Terrain("data/images/heightmap.png")) {}

void Game::move(float x, float y, float z) {
    move(vec3(x, y, z));
}

void Game::move(vec3 translation) {
    pos_ += translation;
    eye_pos_ += translation;
}

glm::mat4 Game::view() const {
    return glm::lookAt(eye_pos_, reference_pos(), vec3(0, 1, 0));
}

SceneNode* Game::scene_root() const {
    return scene_root_;
}

// Private methods.

vec3 Game::reference_pos() const {
    return eye_pos_ + eye_dir_;
}
