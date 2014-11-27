#ifndef DODGER_SHADER_PROGRAM_HH_
#define DODGER_SHADER_PROGRAM_HH_

#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/glm.hpp>

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
    // Sets the specified float type uniform value.
    void uniform1f(std::string name, GLfloat value) const;
    // Sets the specified 3-tuple float type uniform value.
    void uniform3f(std::string name, GLfloat v0, GLfloat v1, GLfloat v2) const;
    // Sets the specified vec3 type uniform value.
    void uniformVec3(std::string name, const glm::vec3& value) const;
    // Sets the specified mat4 type uniform value.
    void uniformMat4(std::string name, const glm::mat4& value) const;
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

#endif // DODGER_SHADER_PROGRAM_HH_
