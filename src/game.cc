#include <glm/glm.hpp>
#include "game.hh"

using glm::vec3;

Game::Game()
    : pos_(vec3(0, 0, 10)),
      eye_pos_(vec3(0, 0, 8)),
      eye_dir_(vec3(0.0f, 0.0f, -1.0f)) {}

void Game::move(float x, float y, float z) {
    move(vec3(x, y, z));
}

void Game::move(vec3 translation) {
    pos_ += translation;
    eye_pos_ += translation;
}

vec3 Game::pos() const {
    return pos_;
}

vec3 Game::eye_pos() const {
    return eye_pos_;
}

vec3 Game::reference_pos() const {
    return eye_pos_ + eye_dir_;
}
