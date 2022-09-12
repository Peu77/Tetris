//
// Created by peu77 on 7/18/22.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "glad/glad.h"
#include "Shader.h"

std::string readFile(std::string fileName) {
    std::ifstream stream(fileName);

    // check if file is open
    if (!stream.is_open()) {
        std::cout << "Failed to open file: " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::stringstream ss[1];
    while (getline(stream, line)) {
        ss[0] << line << '\n';
    }
    return ss[0].str();
}

Shader::Shader(std::string shaderName) {
    std::string vertexShaderPath = "res/shaders/" + shaderName + "vertex.glsl";
    std::string fragmentShaderPath = "res/shaders/" + shaderName + "fragment.glsl";
    std::string vertexShaderCode = readFile(vertexShaderPath);
    std::string fragmentShaderCode = readFile(fragmentShaderPath);
    vertexShaderId = createShader(vertexShaderCode, GL_VERTEX_SHADER);
    fragmentShaderId = createShader(fragmentShaderCode, GL_FRAGMENT_SHADER);
    programId = createProgram(vertexShaderId, fragmentShaderId);
}

Shader::~Shader() {
    glDeleteProgram(programId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

void Shader::use() const {
    glUseProgram(programId);
}


void Shader::uniform1f(std::string name, float value) {
    int location = getUniformLocation(name);
    glUniform1f(location, value);
}

void Shader::uniform3f(std::string name, float f1, float f2, float f3) {
    int location = getUniformLocation(name);
    glUniform3f(location, f1, f2, f3);
}

void Shader::uniform1i(std::string name, int value) {
    int location = getUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::uniformVec3(std::string name, const glm::vec3 &value) {
    int location = getUniformLocation(name);
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::uniformMatrix4f(std::string name, glm::mat4 &value) {
    int location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

unsigned int Shader::createShader(const std::string &shaderCode, unsigned int type) {
    // create shader
    unsigned int shaderId = glCreateShader(type);
    // set shader source
    const char *shaderCodePtr = shaderCode.c_str();
    glShaderSource(shaderId, 1, &shaderCodePtr, nullptr);
    // compile shader
    glCompileShader(shaderId);
    // check compile status
    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success) {
        int logLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
        char *log = new char[logLength];
        glGetShaderInfoLog(shaderId, logLength, nullptr, log);
        std::cout << "Shader compile error: " << log << std::endl;
        delete[] log;
    }
    return shaderId;
}

int Shader::getUniformLocation( std::string &name) {
    if (uniformLocationCache.contains(name)) {
        return uniformLocationCache.at(name);
    }
    int location = glGetUniformLocation(programId, name.c_str());
    if (location == -1) {
        std::cout << "Uniform " << name.c_str() << " not found" << std::endl;
    }

    uniformLocationCache[name] = location;

    return location;
}

unsigned int Shader::createProgram(unsigned int vertexShaderId, unsigned int fragmentShaderId) {
    // create program
    unsigned int programId = glCreateProgram();
    // attach shaders
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    // link program
    glLinkProgram(programId);
    // check link status
    int success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        int logLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        char *log = new char[logLength];
        glGetProgramInfoLog(programId, logLength, nullptr, log);
        std::cout << "Shader link error: " << log << std::endl;
        delete[] log;
    }
    return programId;
}
