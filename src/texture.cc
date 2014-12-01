#include <vector>
#include "error.hh"
#include "image.hh"
#include "texture.hh"

using std::string;
using std::vector;

// Public methods.

Texture::Texture(string filename, bool clamp, bool nearest) {
    init(clamp, nearest);

    Image map;
    map.loadPng(filename);

    width_ = map.width();
    height_ = map.height();
    // Convert double data to float data and flip the image from top left
    // origin to bottom left origin.
    size_t len = width_ * height_ * map.elements();
    vector<float> data(len);
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            int data_index = y * width_ + x;
            int map_index = (height_ - y - 1) * width_ + x;
            data[data_index * 3] = map.data()[map_index * 3];
            data[data_index * 3 + 1] = map.data()[map_index * 3 + 1];
            data[data_index * 3 + 2] = map.data()[map_index * 3 + 2];
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_,
                 height_, 0, GL_RGB, GL_FLOAT, &data[0]);
    checkGlError();

    glGenerateMipmap(GL_TEXTURE_2D);
    checkGlError();
}

Texture::Texture(int width, int height, bool clamp, bool nearest) {
    init(clamp, nearest);

    width_ = width;
    height_ = height;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    checkGlError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    checkGlError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
                    GL_COMPARE_R_TO_TEXTURE);
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

GLuint Texture::id() const {
    return id_;
}

int Texture::width() const {
    return width_;
}

int Texture::height() const {
    return height_;
}

// Private methods.

void Texture::init(bool clamp, bool nearest) {
    glGenTextures(1, &id_);
    checkGlError();

    bind();

    if (clamp) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        checkGlError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        checkGlError();
    }
    if (nearest) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        checkGlError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        checkGlError();
    } else {
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
    }
}
