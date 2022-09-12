//
// Created by peu77 on 9/10/22.
//

#include "Piece.h"
#include "glm/ext/matrix_transform.hpp"

std::vector<PieceData> defaultPieces = {
        // I
        {
                glm::vec3(0, 0, 0),
                glm::vec3(1, 0, 0),
                glm::vec3(2, 0, 0),
                glm::vec3(3, 0, 0),
                {1.5, -0.5, 0},
                {0.1411, 1.0,     1.0}
        },

        // J
        {
                glm::vec3(0, 0, 0),
                glm::vec3(0, 1, 0),
                glm::vec3(1, 1, 0),
                glm::vec3(2, 1, 0),
                {1,   1,    0},
                {0.3529, 0.098,   0.901}
        },

        // L
        {
                glm::vec3(2, 0, 0),
                glm::vec3(0, 1, 0),
                glm::vec3(1, 1, 0),
                glm::vec3(2, 1, 0),
                {1,   1,    0},
                {0.866,  0.396,   0.125}
        },

        // O
        {
                glm::vec3(0, 0, 0),
                glm::vec3(1, 0, 0),
                glm::vec3(0, 1, 0),
                glm::vec3(1, 1, 0),
                {0.5, 0.5,  0.0},
                {0.898,  0.949,   0.02745}
        },
        // S
        {
                glm::vec3(1, 0, 0),
                glm::vec3(2, 0, 0),
                glm::vec3(0, 1, 0),
                glm::vec3(1, 1, 0),
                {1,   1,    0},
                {0.225,  0.89411, 0.08627}
        },
        // T
        {
                glm::vec3(1, 0, 0),
                glm::vec3(0, 1, 0),
                glm::vec3(1, 1, 0),
                glm::vec3(2, 1, 0),
                {1,   1,    0},
                {0.745,  0.0784,  0.9019}
        },

        // Z
        {
                glm::vec3(0, 0, 0),
                glm::vec3(1, 0, 0),
                glm::vec3(1, 1, 0),
                glm::vec3(2, 1, 0),
                {1,   1,    0},
                {0.807,  0.133,   0.133}
        }

};

Piece::Piece(WindowInfo &windowInfo) : stuck(false) {
    pieceIndex = rand() % defaultPieces.size();
    this->pieceData = defaultPieces[pieceIndex];
    float playAreaWidth = (float) windowInfo.width / ELEMENT_DIMENSION;
    this->setPosition({playAreaWidth / 2, 0});
}

void Piece::setPosition(glm::vec2 direction) {
    for (auto &position: pieceData.positions) {
        position.x += direction.x;
        position.y += direction.y;
    }
    pieceData.origin.x += direction.x;
    pieceData.origin.y += direction.y;
}

void Piece::move(glm::vec2 direction, PIECE_LIST, WindowInfo &windowInfo, bool isGravity) {
    if (!canMove(direction, pieces, windowInfo, isGravity))
        return;

    setPosition(direction);
}

bool Piece::canMove(glm::vec2 direction, PIECE_LIST, WindowInfo &windowInfo, bool isGravity) {
    if (stuck) return false;

    const float playFieldHeight = (float) windowInfo.height / ELEMENT_DIMENSION;
    for (auto &position: pieceData.positions) {
        if (position.y + direction.y > playFieldHeight - 1) {
            stuck = true;
            return false;
        }
    }
    for (auto &piece: pieces) {
        for (auto &position: piece->pieceData.positions) {
            for (auto &position2: pieceData.positions) {
                if (piece == this)
                    continue;

                if (position.y == position2.y + direction.y && position.x == position2.x + direction.x) {
                    if (isGravity) {
                        stuck = true;
                    }
                    return false;
                }
            }
        }
    }
    return true;
}

bool Piece::canRotate(glm::vec3 newPositions[4], PIECE_LIST, WindowInfo &windowInfo) {
    const float playFieldWidth = (float) windowInfo.width / ELEMENT_DIMENSION;
    const float playFieldHeight = (float) windowInfo.height / ELEMENT_DIMENSION;

    for (int i = 0; i < 4; i++) {
        auto position = newPositions[i];
        if (position.x < 0 || position.x > playFieldWidth - 1) {
            return false;
        }
        if (position.y > playFieldHeight - 1) {
            stuck = true;
            return false;
        }
    }
    for (auto &piece: pieces) {
        for (auto &position: piece->pieceData.positions) {
            for (int i = 0; i < 4; i++) {
                auto position2 = newPositions[i];
                if (piece != this)
                    if (position.x == position2.x && position.y == position2.y) {
                        return false;
                    }
            }
        }
    }
    return true;
}

void Piece::render(Renderer *renderer) {
    const float halfElementDimension = ELEMENT_DIMENSION / 2.0f;
    const glm::vec3 color = pieceData.color;
    for (auto &block: this->pieceData.positions) {
        renderer->renderQuad(block.x * ELEMENT_DIMENSION + halfElementDimension,
                             block.y * ELEMENT_DIMENSION + halfElementDimension,
                             ELEMENT_DIMENSION,
                             ELEMENT_DIMENSION, color);
    }


    glm::vec3 origin = this->pieceData.origin;
    renderer->renderQuad(origin.x * ELEMENT_DIMENSION + halfElementDimension,
                         origin.y * ELEMENT_DIMENSION + halfElementDimension,
                         20,
                         20, glm::vec3(0, 0, 1));
}

void Piece::rotate(PIECE_LIST, WindowInfo &windowInfo) {
    glm::vec3 center = pieceData.origin;
    glm::vec3 newPositions[4];

    int index = 0;
    for (auto &block: this->pieceData.positions) {
        glm::vec3 newBlock = block - center;
        newPositions[index] = glm::vec3(-newBlock.y, newBlock.x, 0) + center;
        index++;
    }

    if (canRotate(newPositions, pieces, windowInfo)) {
        for (int i = 0; i < 4; i++) {
            this->pieceData.positions[i] = newPositions[i];
        }
    } else {
        std::cout << "Can't rotate" << std::endl;
    }
}


void Piece::update(PIECE_LIST, WindowInfo &windowInfo) {
    const glm::vec3 down = glm::vec3(0, 1, 0);
    move(glm::vec2(0, 1), pieces, windowInfo, true);
}

