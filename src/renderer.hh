#ifndef DODGER_RENDERER_HH_
#define DODGER_RENDERER_HH_

#include <glm/glm.hpp>
#include "game.hh"
#include "sky.hh"
#include "spirit.hh"
#include "terrain.hh"

// An interface for renderers.
class Renderer {
  public:
    // Constructs a renderer with the specified game.
    explicit Renderer(const Game&);
    virtual ~Renderer();

    // Clears everything.
    virtual void clear() const = 0;
    // Prepares for shadow rendering.
    virtual void prepareShadowRendering() const = 0;
    // Prepares for rendering.
    virtual void prepareRendering() const = 0;
    // Renders everything.
    virtual void render();
    // Renders a terrain node with the specified model transformations for
    // shadowing.
    virtual void renderShadow(const Terrain&, glm::mat4 model_trans) = 0;
    // Renders a spirit node with the specified model transformations for
    // shadowing.
    virtual void renderShadow(const Spirit&, glm::mat4 model_trans) = 0;
    // Renders a terrain node with the specified model transformations.
    virtual void render(const Terrain&, glm::mat4 model_trans) const = 0;
    // Renders a sky node with the specified model transformations.
    virtual void render(const Sky&, glm::mat4 model_trans) const = 0;
    // Renders a spirit node with the specified model transformations.
    virtual void render(const Spirit&, glm::mat4 model_trans) = 0;
    // Callback that resizes the viewport as the window resizes.
    virtual void resize(int width, int height) = 0;
    // Toggles wireframe mode. The default is off.
    virtual void toggleWireframe();
    // Toggles backface culling. The default is on.
    virtual void toggleBackfaceCulling();


  protected:
    const Game& game_;

    bool drawing_wireframe_;
    bool culling_backface_;
};

#endif // DODGER_RENDERER_HH_
