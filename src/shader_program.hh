#ifndef DODGER_SHADER_PROGRAM_H_
#define DODGER_SHADER_PROGRAM_H_

#include <unordered_map>
#include <GL/glew.h>

class ShaderProgram {
  public:
    void loadVertexShader(const char* filename);
    void loadFragmentShader(const char* filename);
    void link();
    void use() const;
    void uniformMat4(const char* name, const GLfloat* value) const;
    void enableVertexAttribArray(const char* name) const;
    void vertexAttribPointer(const char* name,
                             GLint size,
                             GLenum type,
                             GLboolean normalized,
                             GLsizei stride,
                             const GLvoid* pointer) const;
    GLuint uniformLocation(const char* name) const;
    GLuint attribLocation(const char* name) const;

  private:
    GLuint loadShader(GLenum shader_type, const char* filename) const;

    GLuint program_;
    GLuint vertex_shader_;
    GLuint fragment_shader_;
    // Mutable because this is only used for lazy initialization.
    mutable std::unordered_map<std::string, GLuint> uniforms_;
    mutable std::unordered_map<std::string, GLuint> attributes_;
};

#endif // DODGER_SHADER_PROGRAM_H_
