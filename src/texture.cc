#include <vector>
#include "glerror.hh"
#include "image.hh"
#include "texture.hh"

using std::string;
using std::vector;

Texture::Texture(string filename) {
    glGenTextures(1, &id_);
    checkGlError();

    bind();

    Image texture_map;
    texture_map.loadPng(filename);

    // Convert double data to float data.
    size_t len = texture_map.width() * texture_map.height()
                 * texture_map.elements();
    vector<float> data(len);
    for (size_t i = 0; i < len; ++i)
        data[i] = texture_map.data()[i];

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_map.width(),
                 texture_map.height(), 0, GL_RGB, GL_FLOAT, &data[0]);
    checkGlError();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    checkGlError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    checkGlError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkGlError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    checkGlError();

    GLfloat aniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
    checkGlError();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);
    checkGlError();

    checkGlError();
    glGenerateMipmap(GL_TEXTURE_2D);
    checkGlError();
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id_);
    checkGlError();
}

void Texture::activateAndBind(GLenum texture_unit) const {
    glActiveTexture(texture_unit);
    checkGlError();

    bind();
}
