#include <exception>
#include <iostream>
// GLFW documentation asks to include GLEW headers before GLFW.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gameover.hh"
#include "openal_renderer.hh"
#include "opengl_renderer.hh"

using std::cout;
using std::cerr;
using std::endl;
using std::exception;

namespace {

// These global variables are global because they need to be used by lambda
// functions who need to access the them but cannot capture any variable due to
// the fact that they must be used as function pointers. Only lambda functions
// that do not capture anything can be used as function pointers.
Game game;
Renderer* prenderer;
bool left_mouse_button_down = false;
bool right_mouse_button_down = false;

struct { double x, y; } last_mouse_pos;

// Handles all the keyboard input state for each frame.
void handleKeyState(GLFWwindow* window, Game& game) {
    bool lctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        game.moveForward();
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        game.moveBackward();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        game.moveLeft();
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        game.moveRight();

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        game.turnLeft();
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        game.turnRight();

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (lctrl)
            game.moveDown();
        else
            game.moveUp();
    }

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        game.accelerate();
    else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        game.decelerate();
}

// Handles all the keyboard input event independent of frames.
void handleKeyEvent(GLFWwindow*, int key, int, int action, int) {
    if (action != GLFW_PRESS)
        return;

    switch (key) {
    case GLFW_KEY_1:
        prenderer->toggleWireframe();
        break;
    case GLFW_KEY_2:
        prenderer->toggleBackfaceCulling();
        break;
    case GLFW_KEY_G:
        game.toggleGodMode();
        break;
    }
}

// Handles mouse press and release event.
void handleMouseButtonEvent(GLFWwindow* window, int button, int action, int) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        left_mouse_button_down = (action == GLFW_PRESS);
        if (!left_mouse_button_down)
            game.lookAtPlayer();
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        right_mouse_button_down = (action == GLFW_PRESS);
        break;
    }
    if (left_mouse_button_down || right_mouse_button_down)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// Handles mouse move event.
void handleMouseMoveEvent(GLFWwindow* window, double x, double y) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    double dx = (x - last_mouse_pos.x) / width;
    double dy = (y - last_mouse_pos.y) / height;
    if (right_mouse_button_down)
        game.rotatePlayer(dx, dy);
    else if (left_mouse_button_down)
        game.rotateCamera(dx, dy);
    last_mouse_pos.x = x;
    last_mouse_pos.y = y;
}

// Handles scrolling device input event (e.g., mouse scroll).
void handleScrollEvent(GLFWwindow*, double, double yoffset) {
    game.zoom(-yoffset * 0.1);
}

}

int main() {
    glfwSetErrorCallback([](int, const char* description) {
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
        OpenGLRenderer renderer(width, height, game);
        prenderer = &renderer;
        OpenALRenderer audio_renderer(game);

        glfwSetWindowSizeCallback(
            window,
            [](GLFWwindow* window, int width, int height) {
                glfwGetFramebufferSize(window, &width, &height);
                prenderer->resize(width, height);
            }
        );
        glfwSetKeyCallback(window, handleKeyEvent);
        glfwSetMouseButtonCallback(window, handleMouseButtonEvent);
        glfwSetCursorPosCallback(window, handleMouseMoveEvent);
        glfwSetScrollCallback(window, handleScrollEvent);

        glfwGetCursorPos(window, &last_mouse_pos.x, &last_mouse_pos.y);

        // Force vertical sync.
        glfwSwapInterval(1);

#ifndef NDEBUG
        int frame_count = 0;
        double last_frame_time = glfwGetTime();
#endif
        while (!glfwWindowShouldClose(window)) {
            try {
                renderer.render();
                audio_renderer.render();
                glfwSwapBuffers(window);
                glfwPollEvents();
                handleKeyState(window, game);
                game.tick();
            } catch (const Gameover&) {
                game.reset();
            }

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
