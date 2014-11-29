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

    // Moves the player.
    void move(float x, float y, float z);
    void move(glm::vec3 translation);
    // Turns the player left or right on the xz terrain plane.
    void turn(float angle);
    // Zooms in (negative distance) or out (positive distance) the camera.
    void zoom(float distance);
    // Rotates the camera only.
    void rotateCamera(float dx, float dy);
    // Rotates the player together with camera.
    void rotatePlayer(float dx, float dy);
    // Reset the camera's x angle, making it look at the player.
    void lookAtPlayer();
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

    // Camera's distance to the player.
    float camera_distance_;
    glm::vec2 camera_angles_;
    glm::vec3 god_camera_translation_;
    bool god_mode_;
};

#endif // DODGER_GAME_HH_
