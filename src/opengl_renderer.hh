#ifndef DODGER_OPENGL_RENDERER_HH_
#define DODGER_OPENGL_RENDERER_HH_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "game.hh"
#include "renderer.hh"
#include "shader_program.hh"
#include "sky_renderer.hh"
#include "spirit_renderer.hh"
#include "terrain_renderer.hh"

// The main OpenGL renderer that renders everything.
class OpenGLRenderer : public Renderer {
  public:
    // Constructs a renderer with the specified framebuffer size and game.
    OpenGLRenderer(int width, int height, const Game&);

    // Clears everything.
    void clear() const;
    using Renderer::render;
    // Renders a terrain node with the specified model transformation.
    virtual void render(const Terrain&, glm::mat4 model_trans) const;
    // Renders a sky node with the specified model transformation.
    virtual void render(const Sky&, glm::mat4 model_trans) const;
    // Renders a spirit node with the specified model transformation.
    virtual void render(const Spirit&, glm::mat4 model_trans) const;
    // Resizes the viewport as the window resizes.
    void resize(int width, int height);
    // Toggles wireframe mode. The default is off.
    virtual void toggleWireframe();
    // Toggles backface culling. The default is on.
    virtual void toggleBackfaceCulling();

  private:
    // Initialize GLEW.
    void initGlew() const;

    const ShaderProgram program_;
    const TerrainRenderer terrain_renderer_;
    const SkyRenderer sky_renderer_;
    const SpiritRenderer spirit_renderer_;

    glm::mat4 proj_trans_;
};

#endif // DODGER_OPENGL_RENDERER_HH_
