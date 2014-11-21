#ifndef DODGER_RENDERER_H_
#define DODGER_RENDERER_H_

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "game.hh"
#include "shader_program.hh"

class Renderer {
  public:
    Renderer(int width, int height, const Game&);

    void render();
    void resize(int width, int height);

  private:
    void initGlew() const;
    glm::mat4 mvp() const;

    const Game& game_;

    GLuint vao_;
    GLuint vbo_;
    ShaderProgram program_;

    glm::mat4 perspective_, view_, model_;
};

#endif // DODGER_RENDERER_H_
