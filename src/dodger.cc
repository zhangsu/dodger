#include <exception>
#include <iostream>
// GLFW documentation asks to include GLEW headers before GLFW.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.hh"

using std::cout;
using std::cerr;
using std::endl;
using std::exception;

namespace {

// This global pointer are used by lambda functions who need to access the
// renderer but cannot capture any variable due to the fact that they must
// be used as function pointers. Only lambda functions that do not capture
// anything can be used as function pointers.
Renderer* prenderer;

// Handles all the keyboard input.
void handleKeyEvent(GLFWwindow* window, Game& game) {
    bool lctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        game.move(0, 0, -0.1);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        game.move(0, 0, 0.1);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        game.move(-0.1, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        game.move(0.1, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        game.move(0, lctrl ? -0.1 : 0.1, 0);
}

}

int main(void) {
    glfwSetErrorCallback([](int error, const char* description) {
        (void) error;
        cerr << "GLFW: " << description << endl;
    });

    if (!glfwInit())
        return -1;

    // Create window and OpenGL context.
    int width = 800, height = 600;
    glfwWindowHint(GLFW_VISIBLE, false);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "Dodger", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -2;
    }

    // Center the window on the primary monitor.
    const GLFWvidmode* vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(window,
                     (vid_mode->width - width) / 2,
                     (vid_mode->height - height) / 2);
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);

    int exit_code = 0;
    // Frame buffer size can be different from window size.
    glfwGetFramebufferSize(window, &width, &height);
    try {
        Game game;
        Renderer renderer(width, height, game);
        prenderer = &renderer;

        glfwSetWindowSizeCallback(
            window,
            [](GLFWwindow* window, int width, int height) {
                glfwGetFramebufferSize(window, &width, &height);
                prenderer->resize(width, height);
            }
        );

        // Force vertical sync.
        glfwSwapInterval(1);

#ifndef NDEBUG
        int frame_count = 0;
        double last_frame_time = glfwGetTime();
#endif
        while (!glfwWindowShouldClose(window)) {
            renderer.render();
            glfwSwapBuffers(window);
            glfwPollEvents();
            handleKeyEvent(window, game);

#ifndef NDEBUG
            // Output FPS count to standard output.
            frame_count++;
            double current_frame_time = glfwGetTime();
            double time_delta = current_frame_time - last_frame_time;
            if (time_delta >= 1.0) {
                cout << "FPS: " << frame_count / time_delta << endl;
                frame_count = 0;
                last_frame_time = current_frame_time;
            }
#endif
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
        exit_code = -3;
    }

    glfwTerminate();
    return exit_code;
}
