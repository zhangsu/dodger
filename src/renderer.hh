#ifndef DODGER_RENDERER_H_
#define DODGER_RENDERER_H_

#include <glm/glm.hpp>
#include "game.hh"
#include "spirit.hh"
#include "terrain.hh"

// An interface for renderers.
class Renderer {
  public:
    // Constructs a renderer with the specified game.
    Renderer(const Game&);

    // Clears everything.
    virtual void clear() const = 0;
    // Renders everything.
    virtual void render() const;
    // Renders a terrain node with the specified model transformations.
    virtual void render(const Terrain&, glm::mat4 transformations) const = 0;
    // Renders a spirit node with the specified model transformations.
    virtual void render(const Spirit&, glm::mat4 transformations) const = 0;
    // Callback that resizes the viewport as the window resizes.
    virtual void resize(int width, int height) = 0;
    // Toggles wireframe mode. The default is off.
    virtual void toggleWireframe();


  protected:
    const Game& game_;

    bool draw_wireframe_;
};

#endif // DODGER_RENDERER_H_
