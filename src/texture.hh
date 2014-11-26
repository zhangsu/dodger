#ifndef OPENGL_TEXTURE_H_
#define OPENGL_TEXTURE_H_

#include <string>
#include <GL/glew.h>

// An abstraction of OpenGL texture object.
class Texture {
  public:
    // Constructs a texture from an image.
    Texture(std::string filename);

    // Binds to this texture.
    void bind() const;
    // Activates the specified texture unit and binds to this texture.
    void activateAndBind(GLenum texture_unit) const;

  private:
    GLuint id_;
};

#endif // OPENGL_TEXTURE_H_