#ifndef DODGER_GAME_HH_
#define DODGER_GAME_HH_

#include <vector>
#include <glm/glm.hpp>
#include "light.hh"
#include "scene_node.hh"
#include "spirit.hh"
#include "terrain.hh"

class Game {
  public:
    Game();

    void move(float x, float y, float z);
    void move(glm::vec3 translation);
    void turn(float angle);
    // Zoom in (negative distance) or out (positive distance) the camera.
    void zoom(float distance);
    // Toggles god mode.
    void toggleGodMode();

    // Returns the viewing transformation matrix.
    glm::mat4 viewTrans() const;
    // Gets the root node of the scene.
    const SceneNode& scene() const;
    // Gets the camera node.
    const SceneNode* camera() const;
    // Gets the list of lights.
    const std::vector<const Light*> lights() const;
    // Gets the ambient light coefficient.
    float ambient() const;

  private:
    void addLight(SceneNode* parent, Light*);
    // Updates the camera transformation in parent's frame.
    void updateCameraTrans();

    const float ambient_;

    Terrain* const terrain_;
    SceneNode* const camera_;
    Spirit* const player_;

    SceneNode scene_;
    std::vector<const Light*> lights_;

    float camera_distance_;
    bool god_mode_;
};

#endif // DODGER_GAME_HH_
