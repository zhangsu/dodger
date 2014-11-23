#ifndef DODGER_IMAGE_HH_
#define DODGER_IMAGE_HH_

#include <string>

// An image, consisting of a rectangle of floating-point elements.
//
// This class makes it easy to read PNG files and the like from files. Note
// that colours in the range [0.0, 1.0] are mapped to the integer range
// [0, 255] when writing and reading PNG files.
class Image {
  public:
    // Constructs an empty image.
    Image();
    // Constructs an image with the given width, height and depth.
    Image(int width, int height, int depth);
    // Constructs an image as a copy of another.
    explicit Image(const Image& other);

    ~Image();

    // Copies the data from one image to another.
    Image& operator=(const Image& other);

    // Gets the width of the image.
    int width() const;
    // Gets the height of the image.
    int height() const;
    // Gets the depth (doubles per pixel) of the image.
    int elements() const;

    // Retrieve a particular component from the image.
    double operator()(int x, int y, int i) const;
    double& operator()(int x, int y, int i);

    // Loads a PNG file into this image.
    bool loadPng(const std::string& filename);
    // Saves this image into the given PNG file.
    bool savePng(const std::string& filename);

    // Retrieves the raw pixel data.
    const double* data() const;
    double* data();

  private:
    int width_, height_;
    int elements_;
    double* data_;
};

#endif // DODGER_IMAGE_HH_
