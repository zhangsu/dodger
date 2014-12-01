#include "audio_renderer.hh"

// Public methods.

AudioRenderer::AudioRenderer(const Game& game) : game_(game), on_(true) {}

AudioRenderer::~AudioRenderer() {}

void AudioRenderer::render() {
    if (on_)
        game_.scene().renderAudio(*this);
}

void AudioRenderer::render(const SceneNode&, glm::mat4) {}

void AudioRenderer::toggle() {
    on_ ^= true;
}
