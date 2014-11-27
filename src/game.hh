#ifndef DODGER_GAME_HH_
#define DODGER_GAME_HH_

#include <glm/glm.hpp>
#include "scene_node.hh"

class Game {
  public:
    Game();
    void move(float x, float y, float z);
    void move(glm::vec3 translation);
    void turn(float angle);
    // Returns the viewing transformation matrix.
    glm::mat4 viewTrans() const;
    // Gets the root node of the scene.
    SceneNode* scene_root() const;

  private:
    SceneNode* const camera_;
    SceneNode* const player_;
    SceneNode* const scene_root_;
};

#endif // DODGER_GAME_HH_
