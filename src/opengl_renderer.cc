#include <stdexcept>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "error.hh"
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
        glEnable(GL_CULL_FACE),
        checkGlError(),
        glEnable(GL_DEPTH_TEST),
        checkGlError(),

        game)),
      shadow_map_texture_index_(8),
      shadow_proj_trans_(glm::ortho<float>(-150, 150, -130, 130, -1000, 10000)),
      terrain_renderer_(game_, shadow_map_texture_index_, drawing_shadow_),
      sky_renderer_(game_),
      spirit_renderer_(game_),
      shadow_frame_buffer_(0),
      shadow_map_(8192, 8192, false, true) {

    resize(width, height);

    // Initialize objects used by shadow mapping.
    glGenFramebuffers(1, &shadow_frame_buffer_);
    checkGlError();
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame_buffer_);
    checkGlError();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                         shadow_map_.id(), 0);
    checkGlError();
    glDrawBuffer(GL_NONE);
    checkGlError();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw runtime_error("Framebuffer object not complete");

    shadow_map_.activateAndBind(GL_TEXTURE0 + shadow_map_texture_index_);
    checkGlError();
}

void OpenGLRenderer::clear() const {
    glClearColor(0, 0, 0, 1);
    checkGlError();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    checkGlError();
}

void OpenGLRenderer::prepareShadowRendering() const {
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame_buffer_);
    checkGlError();
    glViewport(0, 0, shadow_map_.width(), shadow_map_.height());
    checkGlError();
}

void OpenGLRenderer::prepareRendering() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGlError();
    glViewport(0, 0, width_, height_);
    checkGlError();
}

void OpenGLRenderer::renderShadow(const Terrain& spirit, mat4 model_trans) {
    if (drawing_terrain_shadow_) {
        terrain_renderer_.renderShadow(
            spirit, game_.sunViewTrans() * model_trans, shadow_proj_trans_);
    }
}

void OpenGLRenderer::renderShadow(const Spirit& spirit, mat4 model_trans) {
    spirit_renderer_.renderShadow(
        spirit, game_.sunViewTrans() * model_trans, shadow_proj_trans_);
}

void OpenGLRenderer::render(const Terrain& terrain, mat4 model_trans) const {
    mat4 shadow_mvp = mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    ) * shadow_proj_trans_ * game_.sunViewTrans() * model_trans;
    terrain_renderer_.render(
        terrain, game_.viewTrans() * model_trans, proj_trans_, shadow_mvp);
}

void OpenGLRenderer::render(const Sky& sky, mat4 model_trans) const {
    sky_renderer_.render(
        sky, game_.viewTrans() * model_trans, proj_trans_);
}

void OpenGLRenderer::render(const Spirit& spirit, mat4 model_trans) {
    spirit_renderer_.render(
        spirit, game_.viewTrans() * model_trans, proj_trans_);
}

void OpenGLRenderer::resize(int width, int height) {
    if (height == 0)
        height = 1;

    width_ = width;
    height_ = height;

    float fov = glm::radians(60.0f);
    float aspect_ratio = (float) width / height;
    proj_trans_ = glm::perspective(fov, aspect_ratio, 0.001f, 10000.0f);
}

void OpenGLRenderer::toggleWireframe() {
    Renderer::toggleWireframe();
    if (drawing_wireframe_)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    checkGlError();
}

void OpenGLRenderer::toggleBackfaceCulling() {
    Renderer::toggleBackfaceCulling();
    if (culling_backface_) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
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
