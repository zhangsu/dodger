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

    // Move functions.
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    // Turns the player left on the xz terrain plane.
    void turnLeft();
    // Turns the player right on the xz terrain plane.
    void turnRight();
    // Accelerates the moving speed.
    void accelerate();
    // Decaccelerates the moving speed.
    void decelerate();
    // Zooms in (negative distance) or out (positive distance) the camera.
    void zoom(float distance);
    // Rotates the camera only.
    void rotateCamera(float dx, float dy);
    // Rotates the player together with camera.
    void rotatePlayer(float dx, float dy);
    // Reset the camera's x angle, making it look at the spirit.
    void lookAtSpirit();
    // Switches the view to a different spirit.
    void switchSpiritView();
    // Ticks the game.
    void tick();
    // Resets the game.
    void reset();
    // Toggles god mode.
    void toggleGodMode();

    // Returns the viewing transformation matrix.
    glm::mat4 viewTrans() const;
    // Returns the viewing transformation matrix from the Sun's perspective.
    glm::mat4 sunViewTrans() const;
    // Gets the root node of the scene.
    const SceneNode& scene() const;
    // Gets the camera node.
    const SceneNode* camera() const;
    // Gets the list of lights.
    const std::vector<const Light*> lights() const;
    // Gets the ambient light coefficient.
    float ambient() const;
    // Gets the number of spirits currently in game.
    size_t spirit_count() const;

  private:
    // Moves the player.
    void move(float x, float y, float z);
    void move(glm::vec3 translation);
    // Turns the player left or right on the xz terrain plane.
    void turn(float angle);
    void addLight(SceneNode* parent, Light*);
    // Updates the camera transformation in parent's frame.
    void updateCameraTrans();

    const float ambient_;
    const glm::vec3 sun_position_;

    Terrain* const terrain_;
    SceneNode* const camera_;
    Spirit* const player_;

    SceneNode scene_;
    std::vector<const Light*> lights_;
    std::vector<Spirit*> enemies_;

    // Camera's distance to the player.
    float camera_distance_;
    glm::vec2 camera_angles_;
    bool god_mode_;
    float moving_speed_;
    int spirit_view_index;
};

#endif // DODGER_GAME_HH_
