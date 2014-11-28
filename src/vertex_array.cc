#include "glerror.hh"
#include "vertex_array.hh"

using std::string;
using std::vector;

VertexArray::VertexArray(const ShaderProgram& program)
    : count_(0),
      program_(program) {
    glGenVertexArrays(1, &id_);
    checkGlError();
}

void VertexArray::bind() const {
    glBindVertexArray(id_);
    checkGlError();
}

void VertexArray::addAttribute(vector<GLfloat> data, string attr,
                               size_t tuple_len) {
    bind();

    GLuint buf;
    glGenBuffers(1, &buf);
    checkGlError();
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    checkGlError();
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof (GLfloat),
                 &data[0], GL_STATIC_DRAW);
    checkGlError();

    GLuint index = program_.attribLocation(attr);
    glEnableVertexAttribArray(index);
    checkGlError();
    glVertexAttribPointer(index, tuple_len, GL_FLOAT, GL_FALSE, 0, 0);
    checkGlError();
}

void VertexArray::set_count(size_t count) {
    count_ = count;
}

size_t VertexArray::count() const {
    return count_;
}
