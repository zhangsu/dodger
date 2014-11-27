#ifndef OPENGL_VERTEX_ARRAY_HH_
#define OPENGL_VERTEX_ARRAY_HH_

#include <vector>
#include <GL/glew.h>
#include "shader_program.hh"

// An abstraction of OpenGL Vertex Array Object and related operations.
class VertexArray {
  public:
    // Constructs a vertex array object with an associated shader program.
    explicit VertexArray(const ShaderProgram&);

    // Binds to this vertex array.
    void bind() const;
    // Adds a buffer to this vertex array with content from `data` that have a
    // tuple size of `tuple_len` and associates it with the specified vertex
    // attribute of the shader.
    void addAttribute(std::vector<GLfloat> data, std::string attr,
                      size_t tuple_len) const;

  private:
    GLuint id_;
    const ShaderProgram& program_;
};

#endif // OPENGL_VERTEX_ARRAY_HH_
