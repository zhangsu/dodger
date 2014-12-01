#include <glm/glm.hpp>
#include "error.hh"
#include "openal_renderer.hh"

using glm::vec4;

// Public methods.

OpenALRenderer::OpenALRenderer(const Game& game) : AudioRenderer(game) {
    checkSealError(seal_startup(nullptr));
    checkSealError(seal_init_buf(&particle_noise_buf));
    checkSealError(seal_load2buf(&particle_noise_buf,
                                 "data/audio/particles.ogg",
                                 SEAL_OV_FMT));
    for (unsigned i = 0; i < Spirit::MAX_COUNT; ++i) {
        checkSealError(seal_init_src(&particle_noises[i]));
        checkSealError(seal_set_src_buf(&particle_noises[i],
                                        &particle_noise_buf));
        checkSealError(seal_set_src_gain(&particle_noises[i], 1.0f));
        // The particle noise is looping.
        checkSealError(seal_set_src_looping(&particle_noises[i], true));
        // Listener and source positions are specified in camera space.
        checkSealError(seal_set_src_relative(&particle_noises[i], true));
        checkSealError(seal_set_listener_pos(0.0f, 0.0f, 0.0f));
        // Simulate sound attenuation.
        checkSealError(seal_set_listener_gain(0.5f));
    }
}

OpenALRenderer::~OpenALRenderer() {
    for (unsigned i = 0; i < Spirit::MAX_COUNT; ++i) {
        seal_destroy_src(&particle_noises[i]);
    }
    seal_destroy_buf(&particle_noise_buf);
    seal_cleanup();
}

void OpenALRenderer::render(const Spirit& spirit, glm::mat4 model_trans) {
    unsigned id = spirit.id();
    seal_src_state_t state;
    checkSealError(seal_get_src_state(&particle_noises[id], &state));
    if (state != SEAL_PLAYING)
        checkSealError(seal_play_src(&particle_noises[id]));
    // Change sound source location in camera space.
    vec4 pos = game_.viewTrans() * model_trans * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    seal_set_src_pos(&particle_noises[id], pos.x, pos.y, pos.z);
}
