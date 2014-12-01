#include "glerror.hh"
#include "vertex_array.hh"

using std::string;
using std::vector;

// Public methods.

VertexArray::VertexArray() : count_(0) {
    glGenVertexArrays(1, &id_);
    checkGlError();
}

VertexArray::VertexArray(const ShaderProgram& program)
    : VertexArray() {
    program_ = &program;
}

void VertexArray::set_program(const ShaderProgram& program) {
    program_ = &program;
}

void VertexArray::bind() const {
    glBindVertexArray(id_);
    checkGlError();
}

void VertexArray::addAttribute(const GLfloat* data, size_t data_len,
                               string attr, size_t tuple_len) {
    initBufferAndAttribute(attr, tuple_len);
    glBufferData(GL_ARRAY_BUFFER, data_len, data, GL_STATIC_DRAW);
    checkGlError();
    buffers_[attr].size = data_len;
}

void VertexArray::addAttribute(vector<GLfloat> data, string attr,
                               size_t tuple_len) {
    addAttribute(&data[0], data.size() * sizeof (GLfloat), attr, tuple_len);
}

void VertexArray::addStreamAttribute(size_t len, string attr,
                                     size_t tuple_len) {
    initBufferAndAttribute(attr, tuple_len);
    glBufferData(GL_ARRAY_BUFFER, len, nullptr, GL_STREAM_DRAW);
    checkGlError();
    buffers_[attr].size = len;
}

void VertexArray::setAttributeDivisor(std::string attr, size_t divisor) {
    GLuint index = program_->attribLocation(attr);
    glVertexAttribDivisor(index, divisor);
    checkGlError();
}

void VertexArray::stream(std::string attr, const GLfloat* data, size_t len) {

    const Buffer& buffer = buffers_[attr];
    glBindBuffer(GL_ARRAY_BUFFER, buffer.id);
    checkGlError();
    // Buffer orphaning.
    glBufferData(GL_ARRAY_BUFFER, buffer.size, NULL, GL_STREAM_DRAW);
    checkGlError();
    glBufferSubData(GL_ARRAY_BUFFER, 0, len, data);
    checkGlError();
}

void VertexArray::set_count(size_t count) {
    count_ = count;
}

size_t VertexArray::count() const {
    return count_;
}

// Private methods.

void VertexArray::initBufferAndAttribute(string attr, size_t tuple_len) {
    bind();

    GLuint buf;
    glGenBuffers(1, &buf);
    checkGlError();
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    checkGlError();

    GLuint index = program_->attribLocation(attr);
    glEnableVertexAttribArray(index);
    checkGlError();
    glVertexAttribPointer(index, tuple_len, GL_FLOAT, GL_FALSE, 0, 0);
    checkGlError();

    buffers_[attr].id = buf;
}
