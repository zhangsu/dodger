#ifndef DODGER_GAME_H_
#define DODGER_GAME_H_

#include <glm/glm.hpp>
#include "scene_node.hh"

class Game {
  public:
    Game();
    void move(float x, float y, float z);
    void move(glm::vec3 translation);
    glm::mat4 viewTrans() const;
    // Gets the root node of the scene.
    SceneNode* scene_root() const;

  private:
    glm::vec3 reference_pos() const;

    glm::vec3 pos_;
    glm::vec3 eye_pos_;
    glm::vec3 eye_dir_;

    SceneNode* const player_;
    SceneNode* const scene_root_;
};

#endif // DODGER_GAME_H_
