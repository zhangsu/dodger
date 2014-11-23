#ifndef DODGER_OPENGL_RENDERER_H_
#define DODGER_OPENGL_RENDERER_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "game.hh"
#include "renderer.hh"
#include "shader_program.hh"
#include "opengl_terrain_renderer.hh"

// The main OpenGL renderer that renders everything.
class OpenGLRenderer : public Renderer {
  public:
    // Constructs a renderer with the specified framebuffer size and game.
    OpenGLRenderer(int width, int height, const Game&);

    // Clears everything.
    void clear() const;
    using Renderer::render;
    // Renders a terrain node with the specified model transformations.
    virtual void render(const Terrain&, glm::mat4 transformations) const;
    // Callback that resizes the viewport as the window resizes.
    void resize(int width, int height);

  private:
    // Initialize GLEW.
    void initGlew() const;

    const ShaderProgram program_;
    const OpenGLTerrainRenderer terrain_renderer_;

    glm::mat4 projection_;
};

#endif // DODGER_OPENGL_RENDERER_H_
