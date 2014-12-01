#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <GL/glew.h>
#include "error.hh"

using std::endl;
using std::ostringstream;
using std::runtime_error;
using std::string;

void _checkGlError(const char* filename, int lineno) {
#ifndef NDEBUG
    ostringstream output;
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        output << endl;
        switch (err) {
        case GL_INVALID_ENUM:
            output << "An unacceptable value is specified for an enumerated "
                   << "argument";
            break;
        case GL_INVALID_VALUE:
            output << "A numeric argument is out of range";
            break;
        case GL_INVALID_OPERATION:
            output << "The specified operation is not allowed in the current "
                   << "state";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            output << "The framebuffer object is not complete";
            break;
        case GL_OUT_OF_MEMORY:
            output << "There is not enough memory left to execute the command";
            break;
        case GL_STACK_UNDERFLOW:
            output << "An attempt has been made to perform an operation that "
                   << "would cause an internal stack to underflow";
            break;
        case GL_STACK_OVERFLOW:
            output << "An attempt has been made to perform an operation that "
                   << "would cause an internal stack to overflow.";
            break;
        }
    }
    string err_str = output.str();
    if (!err_str.empty()) {
        ostringstream message;
        message << "OpenGL (" << filename << ":" << lineno << "): " << err_str;
        throw runtime_error(message.str());
    }
#else
    // If not debugging, turn error checking off for performance.
    (void) filename;
    (void) lineno;
#endif // NDEBUG
}

void _checkSealError(seal_err_t err, const char* filename, int lineno) {
#ifndef NDEBUG
    ostringstream output;
    const char* err_str = seal_get_err_msg(err);
    if (err_str != nullptr) {
        ostringstream message;
        message << "SEAL (" << filename << ":" << lineno << "): " << err_str;
        throw runtime_error(message.str());
    }
#else
    // If not debugging, turn error checking off for performance.
    (void) err;
    (void) filename;
    (void) lineno;
#endif // NDEBUG
}
