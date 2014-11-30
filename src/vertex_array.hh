#ifndef OPENGL_VERTEX_ARRAY_HH_
#define OPENGL_VERTEX_ARRAY_HH_

#include <unordered_map>
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
    void addAttribute(const GLfloat* data, size_t data_len, std::string attr,
                      size_t tuple_len);
    // Adds a buffer to this vertex array with content from `data` that have a
    // tuple size of `tuple_len` and associates it with the specified vertex
    // attribute of the shader.
    void addAttribute(std::vector<GLfloat> data, std::string attr,
                      size_t tuple_len);
    // Adds a empty stream buffer to this vertex array for data that have a
    // tuple size of `tuple_len` and associates it with the specified vertex
    // attribute of the shader. `len` is the length of the buffer.
    void addStreamAttribute(size_t len, std::string attr, size_t tuple_len);
    // Sets the vertex attribute divisor for instanced rendering.
    void setAttributeDivisor(std::string attr, size_t divisor);
    // Streams data into the buffer associated with the specified attribute.
    void stream(std::string attr, const GLfloat* data, size_t len);
    // Sets the number of vertices for this vertex array.
    void set_count(size_t count);
    // Gets the number of vertices for this vertex array.
    size_t count() const;

  private:
    struct Buffer {
        GLuint id;
        size_t size;
    };

    void initBufferAndAttribute(std::string attr, size_t tuple_len);

    GLuint id_;
    size_t count_;
    std::unordered_map<std::string, Buffer> buffers_;

    const ShaderProgram& program_;
};

#endif // OPENGL_VERTEX_ARRAY_HH_
