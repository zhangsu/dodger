#ifndef DODGER_RENDERER_H_
#define DODGER_RENDERER_H_

#include <GL/glew.h>
#include "shader_program.hh"

class Renderer {
  public:
    Renderer(int width, int height);

    void render() const;
    void resize(int width, int height) const;

  private:
    void initGlew() const;

    GLuint vao_;
    GLuint vbo_;
    ShaderProgram program_;
};

#endif // DODGER_RENDERER_H_
