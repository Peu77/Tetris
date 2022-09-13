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

std::function<void()> createPiece = nullptr;

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

    createPiece = [&]() {
        currentPiece = new Piece(windowInfo, createPiece);
        pieces.push_back(currentPiece);
    };

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        /*
        if (key == GLFW_KEY_H && action == GLFW_PRESS) {
            if (createPiece == nullptr) {
                std::cout << "createPiece is null" << std::endl;
                return;

                createPiece();
            }
        }
         */

    });

    int tick = 0;
    createPiece();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // if press key down
        if (tick % 5 == 0) {
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                currentPiece->move({0, 1}, pieces, windowInfo);
            }

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                currentPiece->move({-1, 0}, pieces, windowInfo);
            }

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                currentPiece->move({1, 0}, pieces, windowInfo);
            }

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                currentPiece->rotate(pieces, windowInfo);
            }
        }

        renderer->updateProjection(&windowInfo);
        bool canUpdate = (tick++ % 50 == 0);

        for (auto &piece: pieces) {
            piece->render(renderer);
            if (canUpdate)
                piece->update(pieces, windowInfo);
        }

        renderer->drawGrid();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
