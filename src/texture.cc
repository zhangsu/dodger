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

    Image map;
    map.loadPng(filename);

    // Convert double data to float data and flip the image from top left
    // origin to bottom left origin.
    size_t len = map.width() * map.height() * map.elements();
    vector<float> data(len);
    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
            int data_index = y * map.width() + x;
            int map_index = (map.height() - y - 1) * map.width() + x;
            data[data_index * 3] = map.data()[map_index * 3];
            data[data_index * 3 + 1] = map.data()[map_index * 3 + 1];
            data[data_index * 3 + 2] = map.data()[map_index * 3 + 2];
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, map.width(),
                 map.height(), 0, GL_RGB, GL_FLOAT, &data[0]);
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
