#ifndef DODGER_SHADER_PROGRAM_H_
#define DODGER_SHADER_PROGRAM_H_

#include <unordered_map>
#include <GL/glew.h>

// Program that encapsulates shader-related operations.
class ShaderProgram {
  public:
    // Loads a vertex shader into this program.
    void loadVertexShader(const char* filename);
    // Loads a fragment shader into this program.
    void loadFragmentShader(const char* filename);
    // Links the loaded shaders to this program.
    void link();
    // Uses the current program.
    void use() const;
    // Sets the specified mat4 type uniform value.
    void uniformMat4(const char* name, const GLfloat* value) const;
    // Enable the specified vertex attribute array.
    void enableVertexAttribArray(const char* name) const;
    // Sets the vertex attributes.
    void vertexAttribPointer(const char* name,
                             GLint size,
                             GLenum type,
                             GLboolean normalized,
                             GLsizei stride,
                             const GLvoid* pointer) const;
    // Gets the location of the specified uniform value.
    GLuint uniformLocation(const char* name) const;
    // Gets the location of the specified attribute.
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
