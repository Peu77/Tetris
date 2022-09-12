#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "filesystem"

#include "render/Renderer.h"
#include "render/WindowInfo.h"
#include "game/Piece.h"

Renderer *renderer;

WindowInfo windowInfo;

Piece *currentPiece;
std::vector<Piece *> pieces;

int main() {
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    windowInfo.width = 1400;
    windowInfo.height = 1200;
    windowInfo.title = "Tetris";
    windowInfo.window = window;
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(windowInfo.width, windowInfo.height, windowInfo.title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // on window resize
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    glfwMakeContextCurrent(window);
    // glad init
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    renderer = new Renderer(&windowInfo);

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (key == GLFW_KEY_H && action == GLFW_PRESS) {
            currentPiece = new Piece(windowInfo);
            pieces.push_back(currentPiece);
        }

        if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
            currentPiece->rotate(pieces, windowInfo);
        }


        if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            currentPiece->move({-1, 0}, pieces, windowInfo);
        }

        if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
            currentPiece->move({1, 0}, pieces, windowInfo);
        }
    });

    int tick = 0;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        renderer->updateProjection(&windowInfo);
        bool canUpdate = (tick++ % 50 == 0);

        for (auto &piece: pieces) {
            if (canUpdate)
                piece->update(pieces, windowInfo);
            piece->render(renderer);
        }

        renderer->drawGrid();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
