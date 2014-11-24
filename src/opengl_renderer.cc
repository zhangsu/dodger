#include <stdexcept>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "glerror.hh"
#include "opengl_renderer.hh"

using std::runtime_error;
using std::string;
using glm::mat4;
using glm::vec3;

// Public methods.

OpenGLRenderer::OpenGLRenderer(int width, int height, const Game& game)
    : Renderer((
        initGlew(),
        // Initialize global OpenGL properties.
        glEnable(GL_MULTISAMPLE),
        checkGlError(),
        glEnable(GL_BLEND),
        checkGlError(),
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
        checkGlError(),

        game)),
      program_("src/shader.vert", "src/shader.frag"),
      terrain_renderer_(game_, program_),
      spirit_renderer_(game_, program_) {

    resize(width, height);
}

void OpenGLRenderer::clear() const {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::render(const Terrain& terrain,
                            mat4 model_trans) const {
    terrain_renderer_.render(
        terrain, proj_trans_ * game_.viewTrans() * model_trans);
}

void OpenGLRenderer::render(const Spirit& spirit, mat4 model_trans) const {
    spirit_renderer_.render(
        spirit, proj_trans_ * game_.viewTrans() * model_trans);
}

void OpenGLRenderer::resize(int width, int height) {
    if (height == 0)
        height = 1;

    float fov = glm::radians(60.0f);
    float aspect_ratio = (float) width / height;
    proj_trans_ = glm::perspective(fov, aspect_ratio, 0.001f, 100.0f);

    glViewport(0, 0, width, height);
    checkGlError();
}

// Private methods.

void OpenGLRenderer::initGlew() const {
    glewExperimental = true;
    GLenum err = glewInit();
    // Ignore any OpenGL error generated by GLEW.
    glGetError();
    if (err != GLEW_OK) {
        string message = "GLEW: ";
        throw runtime_error(message + (const char*) glewGetErrorString(err));
    }
}
