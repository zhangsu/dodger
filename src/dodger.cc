#include <exception>
#include <iostream>
// GLFW documentation asks to include GLEW headers before GLFW.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"

using std::cerr;
using std::endl;
using std::exception;

namespace {

Renderer* prenderer;

}

int main(void) {
    glfwSetErrorCallback([](int error, const char* description) {
        (void) error;
        cerr << "GLFW: " << description << endl;
    });

    /* Initialize the library */
    if (!glfwInit())
        return -1;

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
        Renderer renderer(width, height);
        prenderer = &renderer;

        glfwSetWindowSizeCallback(
            window,
            [](GLFWwindow* window, int width, int height) {
                glfwGetFramebufferSize(window, &width, &height);
                prenderer->resize(width, height);
            }
        );

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            renderer.render();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
        exit_code = -3;
    }

    glfwTerminate();
    return exit_code;
}
