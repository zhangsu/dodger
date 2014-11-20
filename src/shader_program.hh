#ifndef DODGER_SHADER_PROGRAM_H_
#define DODGER_SHADER_PROGRAM_H_

#include <GL/glew.h>

class ShaderProgram {
  public:
    void loadVertexShader(const char* filename);
    void loadFragmentShader(const char* filename);
    void link();
    void use() const;

  private:
    GLuint loadShader(GLenum shader_type, const char* filename) const;

    GLuint program_;
    GLuint vertex_shader_;
    GLuint fragment_shader_;
};

#endif // DODGER_SHADER_PROGRAM_H_
