//
// Created by peu77 on 9/10/22.
//

#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "../render/Renderer.h"

#define ELEMENT_DIMENSION 100
#define PIECE_LIST std::vector<Piece*> &pieces

struct PieceData {
    glm::vec3 positions[4];
    glm::vec3 origin;
    glm::vec3 color;
};

class Piece {
private:
    PieceData pieceData;
    int pieceIndex;
    bool stuck;

    void setPosition(glm::vec2 direction);

    bool canMove(glm::vec2 direction, PIECE_LIST, WindowInfo &windowInfo, bool isGravity);

    bool canRotate(glm::vec3 newPositions[4], PIECE_LIST, WindowInfo &windowInfo);

public:
    Piece(WindowInfo &windowInfo);

    void move(glm::vec2 direction, PIECE_LIST, WindowInfo &windowInfo, bool isGravity = false);

    void render(Renderer *renderer);

    void rotate(PIECE_LIST, WindowInfo &windowInfo);

    void update(PIECE_LIST, WindowInfo &windowInfo);
};
