#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <GL/glew.h>
#include "glerror.hh"
#include "shader_program.hh"

using std::ifstream;
using std::ostringstream;
using std::runtime_error;
using std::string;
using std::vector;

// Public methods.

ShaderProgram::ShaderProgram(string vert_filename, string frag_filename) {
    loadVertexShader(vert_filename);
    loadFragmentShader(frag_filename);
    link();
}

void ShaderProgram::loadVertexShader(string filename) {
    vertex_shader_ = loadShader(GL_VERTEX_SHADER, filename);
}

void ShaderProgram::loadFragmentShader(string filename) {
    fragment_shader_ = loadShader(GL_FRAGMENT_SHADER, filename);
}

void ShaderProgram::link() {
    uniforms_.clear();
    attributes_.clear();
    GLuint program = glCreateProgram();
    checkGlError();
    glAttachShader(program, vertex_shader_);
    checkGlError();
    glAttachShader(program, fragment_shader_);
    checkGlError();
    glLinkProgram(program);
    checkGlError();

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    checkGlError();
    if (!linked) {
        GLint log_len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
        checkGlError();
        if (log_len > 0) {
            vector<char> log(log_len);
            glGetProgramInfoLog(program, log_len, nullptr, &log[0]);
            checkGlError();
            throw runtime_error(string("Program: ") + &log[0]);
        } else {
            throw runtime_error(string("Program: unknown error"));
        }
    }

    program_ = program;

    glDeleteShader(vertex_shader_);
    checkGlError();
    glDeleteShader(fragment_shader_);
    checkGlError();
}

void ShaderProgram::use() const {
    glUseProgram(program_);
    checkGlError();
}

void ShaderProgram::uniform1i(string name, GLint value) const {
    glUniform1i(uniformLocation(name), value);
    checkGlError();
}

void ShaderProgram::uniformMat4(string name, const GLfloat* value) const {
    glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, value);
    checkGlError();
}

void ShaderProgram::enableVertexAttribArray(string name) const {
    glEnableVertexAttribArray(attribLocation(name));
    checkGlError();
}

void ShaderProgram::vertexAttribPointer(
    string name,
    GLint size,
    GLenum type,
    GLboolean normalized,
    GLsizei stride,
    const GLvoid* pointer
) const {
    GLuint index = attribLocation(name);
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    checkGlError();
}

GLuint ShaderProgram::uniformLocation(string name) const {
    auto uniform = uniforms_.find(name);
    if (uniform == uniforms_.end()) {
        GLint location = glGetUniformLocation(program_, name.c_str());
        if (location == -1)
            throw runtime_error(string("Uniform ") + name + " does not exist");
        checkGlError();
        uniforms_[name] = location;
        return location;
    }
    return uniform->second;
}

GLuint ShaderProgram::attribLocation(string name) const {
    auto attribute = attributes_.find(name);
    if (attribute == attributes_.end()) {
        GLint location = glGetAttribLocation(program_, name.c_str());
        checkGlError();
        attributes_[name] = location;
        return location;
    }
    return attribute->second;
}

// Private methods.

GLuint ShaderProgram::loadShader(GLenum shader_type, string filename) const {
    GLuint shader = glCreateShader(shader_type);
    checkGlError();

    ifstream t(filename);
    if (t.fail())
        throw runtime_error(string("Cannot open ") + filename);
    ostringstream buffer;
    buffer << t.rdbuf();
    string source = buffer.str();
    const GLchar * source_cstr = source.c_str();
    const GLint size = source.size();

    glShaderSource(shader, 1, &source_cstr, &size);
    checkGlError();

    glCompileShader(shader);
    checkGlError();

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    checkGlError();
    if (!compiled) {
        GLint log_len = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
        checkGlError();

        ostringstream message;
        message << filename << " ";

        if (log_len > 0) {
            vector<char> log(log_len);
            glGetShaderInfoLog(shader, log_len, nullptr, &log[0]);
            checkGlError();
            message << &log[0];
        } else {
            message << "unknown error";
        }
        throw runtime_error(message.str());
    }

    return shader;
}
