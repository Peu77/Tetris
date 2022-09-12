//
// Created by peu77 on 7/18/22.
//

#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include "glm/glm.hpp"


class Shader {
private:
    unsigned int programId;
    unsigned int vertexShaderId;
    unsigned int fragmentShaderId;
    std::unordered_map<std::string, int> uniformLocationCache;

    int getUniformLocation(std::string &name);

public:
    explicit Shader(std::string shaderName);

    ~Shader();

    void uniform1f(std::string name, float value);

    void uniform3f(std::string name, float f1, float f2, float f3);

    void uniform1i(std::string name, int value);

    void uniformVec3(std::string name, const glm::vec3 &value);

    void uniformMatrix4f(std::string name, glm::mat4 &value);

    static unsigned int createShader(const std::string &shaderCode, unsigned int type);

    static unsigned int createProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId);

    void use() const;
};
