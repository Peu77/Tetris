//
// Created by peu77 on 9/10/22.
//

#pragma once

#include "shader/Shader.h"
#include "glad/glad.h"
#include "WindowInfo.h"

struct RenderData {
    unsigned int vertexArray{}, vertexBuffer{}, indexBuffer{};
    int indicesCount{};
};

class Renderer;

static Renderer *instance;

class Renderer {
private:
    RenderData quadData{};
    RenderData gridData{};
    glm::mat4 projection{};
    Shader *shader;

    void initQuad();

    void initGrid(WindowInfo *windowInfo);

public:
    Renderer(WindowInfo *windowInfo);

    void updateProjection(WindowInfo *windowInfo);

    void drawGrid();

    void renderQuad(float x, float y, float width, float height, glm::vec3 color);

    static Renderer *getInstance();
};
