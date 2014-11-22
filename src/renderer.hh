#ifndef DODGER_RENDERER_H_
#define DODGER_RENDERER_H_

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "game.hh"
#include "shader_program.hh"

// The main renderer that renders everything.
class Renderer {
  public:
    // Constructs a renderer with the specified framebuffer size.
    Renderer(int width, int height, const Game&);

    // Renders everything.
    void render() const;
    // Callback that resizes the viewport as the window resizes.
    void resize(int width, int height);

  private:
    void initGlew() const;

    const Game& game_;

    GLuint vao_;
    GLuint vbo_;
    ShaderProgram program_;


    glm::mat4 perspective_;
};

#endif // DODGER_RENDERER_H_
