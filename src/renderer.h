#ifndef DODGER_RENDERER_H_
#define DODGER_RENDERER_H_

#include <GL/glew.h>

class Renderer {
  public:
    Renderer(int width, int height);

    void render() const;
    void resize(int width, int height) const;

  private:
    void initGlew() const;
    GLuint loadVertexShader(const char* filename) const;
    GLuint loadFragmentShader(const char* filename) const;
    GLuint loadShader(GLenum shader_type, const char* filename) const;
    GLuint linkProgram(GLuint vertex_shader, GLuint fragment_shader) const;
    void _checkGlError(const char* filename, int lineno) const;

    GLuint vao_;
    GLuint vbo_;
    GLuint program_;
};

#endif // DODGER_RENDERER_H_
