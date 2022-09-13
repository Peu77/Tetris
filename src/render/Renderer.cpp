//
// Created by peu77 on 9/10/22.
//

#include <vector>
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "../game/Piece.h"

Renderer::Renderer(WindowInfo *windowInfo) {
    shader = new Shader("main/");

    instance = this;

    initQuad();
    initGrid(windowInfo);
}

void Renderer::initQuad() {
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
    };

    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    glGenVertexArrays(1, &quadData.vertexArray);
    glBindVertexArray(quadData.vertexArray);

    glGenBuffers(1, &quadData.vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quadData.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glGenBuffers(1, &quadData.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadData.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexArrayAttrib(quadData.vertexArray, 0);
    quadData.indicesCount = sizeof(indices) / sizeof(unsigned int);
}

void Renderer::initGrid(WindowInfo *windowInfo) {
    std::vector<float> vertices;
    // calculate edge vertices of grid

    int horizontalLines = windowInfo->height / ELEMENT_DIMENSION;
    int verticalLines = windowInfo->width / ELEMENT_DIMENSION;

    for (int i = 0; i < verticalLines; i++) {
        vertices.push_back(i * ELEMENT_DIMENSION);
        vertices.push_back(0);
        vertices.push_back(0);

        vertices.push_back(i * ELEMENT_DIMENSION);
        vertices.push_back(windowInfo->height);
        vertices.push_back(0);
    }

    for (int i = 0; i < horizontalLines; i++) {
        vertices.push_back(0);
        vertices.push_back(i * ELEMENT_DIMENSION);
        vertices.push_back(0);

        vertices.push_back(windowInfo->width);
        vertices.push_back(i * ELEMENT_DIMENSION);
        vertices.push_back(0);
    }


    // (0, 0) (1, 0) (2, 0) (3, 0)   (3, 1) (3, 2) (3, 3)    (2, 3) (1, 3) (0, 3)    (0, 2) (0, 1)



    std::vector<unsigned int> indices;
    for (int i = 0; i < (horizontalLines * 2) + (verticalLines * 2); i++) {
        indices.push_back(i);
    }


    glGenVertexArrays(1, &gridData.vertexArray);
    glBindVertexArray(gridData.vertexArray);

    glGenBuffers(1, &gridData.vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, gridData.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &gridData.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridData.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexArrayAttrib(gridData.vertexArray, 0);
    gridData.indicesCount = indices.size();
}

void Renderer::updateProjection(WindowInfo *windowInfo) {
    projection = glm::ortho(0.0f, (float) windowInfo->width, (float) windowInfo->height, 0.0f, -1.0f, 1.0f);
}

void Renderer::drawGrid() {
    shader->use();
    shader->uniformVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->uniformMatrix4f("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    shader->uniformMatrix4f("model", model);
    glBindVertexArray(gridData.vertexArray);

    glDrawElements(GL_LINES, gridData.indicesCount, GL_UNSIGNED_INT, 0);
}

void Renderer::renderQuad(float x, float y, float width, float height, glm::vec3 color) {
    shader->use();
    shader->uniformVec3("color", color);
    shader->uniformMatrix4f("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(x, y, 0.0f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));

    shader->uniformMatrix4f("model", model);

    glBindVertexArray(quadData.vertexArray);
    glDrawElements(GL_TRIANGLES, quadData.indicesCount, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
}

Renderer *Renderer::getInstance() {
    return instance;
}