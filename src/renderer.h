#ifndef DODGER_RENDERER_H_
#define DODGER_RENDERER_H_

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "shader_program.hh"

class Renderer {
  public:
    Renderer(int width, int height);

    void render() const;
    void resize(int width, int height);

  private:
    void initGlew() const;
    glm::mat4 mvp() const;

    GLuint vao_;
    GLuint vbo_;
    ShaderProgram program_;

    glm::mat4 perspective_, view_, model_;
};

#endif // DODGER_RENDERER_H_
