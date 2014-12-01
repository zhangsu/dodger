#include "audio_renderer.hh"

// Public methods.

AudioRenderer::AudioRenderer(const Game& game) : game_(game) {}

AudioRenderer::~AudioRenderer() {}

void AudioRenderer::render() {
    game_.scene().renderAudio(*this);
}

void AudioRenderer::render(const SceneNode&, glm::mat4) {}
