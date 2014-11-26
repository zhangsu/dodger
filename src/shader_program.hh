#ifndef DODGER_SHADER_PROGRAM_H_
#define DODGER_SHADER_PROGRAM_H_

#include <string>
#include <unordered_map>
#include <GL/glew.h>

// Program that encapsulates shader-related operations.
class ShaderProgram {
  public:
    // Constructs a shader program with the specified vertex and fragment
    // shader.
    ShaderProgram(std::string vert_filename, std::string frag_filename);
    // Loads a vertex shader into this program.
    void loadVertexShader(std::string filename);
    // Loads a fragment shader into this program.
    void loadFragmentShader(std::string filename);
    // Links the loaded shaders to this program.
    void link();
    // Uses the current program.
    void use() const;
    // Sets the specified int type uniform value.
    void uniform1i(std::string name, GLint value) const;
    // Sets the specified mat4 type uniform value.
    void uniformMat4(std::string name, const GLfloat* value) const;
    // Enable the specified vertex attribute array.
    void enableVertexAttribArray(std::string name) const;
    // Sets the vertex attributes.
    void vertexAttribPointer(std::string name,
                             GLint size,
                             GLenum type,
                             GLboolean normalized,
                             GLsizei stride,
                             const GLvoid* pointer) const;
    // Gets the location of the specified uniform value.
    GLuint uniformLocation(std::string name) const;
    // Gets the location of the specified attribute.
    GLuint attribLocation(std::string name) const;

  private:
    GLuint loadShader(GLenum shader_type, std::string filename) const;

    GLuint program_;
    GLuint vertex_shader_;
    GLuint fragment_shader_;
    // Mutable because this is only used for lazy initialization.
    mutable std::unordered_map<std::string, GLuint> uniforms_;
    mutable std::unordered_map<std::string, GLuint> attributes_;
};

#endif // DODGER_SHADER_PROGRAM_H_
