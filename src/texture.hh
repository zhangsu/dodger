#ifndef OPENGL_TEXTURE_HH_
#define OPENGL_TEXTURE_HH_

#include <string>
#include <GL/glew.h>

// An abstraction of OpenGL texture object.
class Texture {
  public:
    // Constructs a texture from an image.
    Texture(std::string filename, bool clamp = false, bool nearest = false);
    // Constructs an empty texture with the specified size for depth values.
    Texture(int width, int height, bool clamp = false,
            bool nearest = false);

    // Binds to this texture.
    void bind() const;
    // Activates the specified texture unit and binds to this texture.
    void activateAndBind(GLenum texture_unit) const;
    // Returns the OpenGL ID of the texture.
    GLuint id() const;
    // Returns the width of the texture.
    int width() const;
    // Returns the height of the texture.
    int height() const;

  private:
    void init(bool clamp, bool nearest);

    GLuint id_;
    int width_;
    int height_;
};

#endif // OPENGL_TEXTURE_HH_
