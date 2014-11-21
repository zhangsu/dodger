#ifndef DODGER_GAME_H_
#define DODGER_GAME_H_

#include <glm/glm.hpp>

class Game {
  public:
    Game();
    void move(float x, float y, float z);
    void move(glm::vec3 translation);
    glm::vec3 pos() const;
    glm::vec3 eye_pos() const;
    glm::vec3 reference_pos() const;

  private:
    glm::vec3 pos_;
    glm::vec3 eye_pos_;
    glm::vec3 eye_dir_;
};

#endif // DODGER_GAME_H_
