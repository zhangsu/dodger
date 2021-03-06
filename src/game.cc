#include <algorithm>
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
      sun_position_(2500.0f, 3000.0f, 0.0f),
      terrain_(new Terrain("data/images/heightmap.png")),
      camera_(new SceneNode()),
      player_(new Spirit(terrain_, true)),
      camera_distance_(10),
      god_mode_(false),
      moving_speed_(0.1f),
      spirit_view_index(0) {
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
    addLight(player_, new Light(vec3(0.05, 0.1, 1.0), vec3(0.1, 0.01, 0.01)));

    // Set up enemies.
    for (int i = 0; i < 8; ++i) {
        Spirit* enemy = new Spirit(terrain_);
        enemies_.push_back(enemy);
        scene_.attach(enemy);
        addLight(enemy, new Light(vec3(1.0, 0.1, 0.05), vec3(0.1, 0.01, 0.01)));
    }

    // Set up camera.
    player_->attach(camera_);
    updateCameraTrans();

    // Add a global sunlight.
    Light* sun = new Light(vec3(0.1, 0.1, 0.1), vec3(0.1, 0, 0), true);
    sun->translate(sun_position_);
    addLight(&scene_, sun);

    reset();
}

void Game::moveForward() {
    move(0, 0, -moving_speed_);
}

void Game::moveBackward() {
    move(0, 0, moving_speed_);
}

void Game::moveLeft() {
    move(-moving_speed_, 0, 0);
}

void Game::moveRight() {
    move(moving_speed_, 0, 0);
}

void Game::moveUp() {
    move(0, moving_speed_, 0);
}

void Game::moveDown() {
    move(0, -moving_speed_, 0);
}
void Game::turnLeft() {
    turn(-0.025);
}

void Game::turnRight() {
    turn(0.025);
}

void Game::accelerate() {
    moving_speed_ += 0.01f;
    if (moving_speed_ > 1.0f)
        moving_speed_ = 1.0f;
}

void Game::decelerate() {
    moving_speed_ -= 0.01f;
    if (moving_speed_ < 0.1f)
        moving_speed_ = 0.1f;
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
        camera_angles_.x += -dx * angle;
        camera_angles_.y += -dy * angle;
        updateCameraTrans();
    }
}

void Game::rotatePlayer(float dx, float dy) {
    float camera_angle = glm::radians(60.0f);
    if (god_mode_) {
        camera_->rotate(-dx * camera_angle, 0, 1, 0);
        camera_->rotate(-dy * camera_angle, 1, 0, 0);
    } else if (spirit_view_index == 0) {
        player_->rotate(-dx * glm::radians(120.0f), 0, 1, 0);
        // Camera y position can be rotated at the same time, but not x.
        camera_angles_.y += -dy * camera_angle;
        updateCameraTrans();
    }
}

void Game::lookAtSpirit() {
    if (god_mode_)
        return;
    camera_angles_.x = 0;
    updateCameraTrans();
}

void Game::switchSpiritView() {
    spirit_view_index = (spirit_view_index + 1) % (enemies_.size() + 1);
    camera_->parent()->detach(camera_);
    if (spirit_view_index == 0) {
        player_->attach(camera_);
    } else {
        enemies_[spirit_view_index - 1]->attach(camera_);
    }
}

void Game::tick() {
    for (auto enemy : enemies_) {
        enemy->walk();
    }
}

void Game::reset() {
    player_->resetTrans();
    player_->translate(0, 10);
    for (unsigned i = 0; i < enemies_.size(); ++i) {
        enemies_[i]->resetTrans();
        enemies_[i]->translate(i / 3 * 5, - (float)(i % 3) * 5);
    }
}

void Game::toggleGodMode() {
    god_mode_ ^= true;
    if (god_mode_) {
        mat4 world_trans = camera_->worldTrans();
        camera_->parent()->detach(camera_);
        camera_->set_trans(world_trans);
    } else {
        player_->attach(camera_);
        updateCameraTrans();
    }
}

mat4 Game::viewTrans() const {
    return glm::inverse(camera_->worldTrans());
}

mat4 Game::sunViewTrans() const {
    return glm::lookAt(sun_position_, vec3(0.0f, 0.0f, 0.0f),
                       vec3(0.0f, 1.0f, 0.0f));
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

size_t Game::spirit_count() const {
    // Number of enemies plus the player.
    return enemies_.size() + 1;
}

// Private methods.

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

void Game::addLight(SceneNode* parent, Light* light) {
    lights_.push_back(light);
    parent->attach(light);
}

void Game::updateCameraTrans() {
    camera_angles_.y = std::min(std::max(camera_angles_.y, -1.0f), 0.0f);
    camera_->resetTrans();
    // Rotate around camera's, also the player's origin.
    camera_->rotate(camera_angles_.x, 0, 1, 0);
    camera_->rotate(camera_angles_.y, 1, 0, 0);
    camera_->translate(0, 0, camera_distance_);
}
