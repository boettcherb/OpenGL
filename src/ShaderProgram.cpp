#include "ShaderProgram.h"
#include "Texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

ShaderProgram::Shader::Shader(unsigned int type, unsigned int id, const std::string& source)
    : m_type{ type }, m_id{ id }, m_source{ source } {}

ShaderProgram::ShaderProgram(const std::vector<std::string>& filePaths) {
	m_shaderProgramID = glCreateProgram();
    for (const std::string& filePath : filePaths) {
        // first = type, second = source
        std::pair<unsigned int, std::string> shaderData = parseShader(filePath);
        unsigned int id = glCreateShader(shaderData.first);
	    m_shaders.emplace_back(shaderData.first, id, shaderData.second);
    }
    compileAndLink();

    // the individual shaders are not needed after they have been linked into one program
    for (const auto& shader : m_shaders) {
        glDeleteShader(shader.m_id);
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_shaderProgramID);
}

void ShaderProgram::compileAndLink() const {
    int success;
    for (const auto& shader : m_shaders) {
        const char* src = shader.m_source.c_str();
        glShaderSource(shader.m_id, 1, &src, nullptr);
        glCompileShader(shader.m_id);

        // make sure the shader compiled successfully
        char infoLog[512] = { 0 };
        glGetShaderiv(shader.m_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader.m_id, 512, nullptr, infoLog);
            std::cerr << "Shader Compilation Failed\n" << infoLog << '\n';
        }
        glAttachShader(m_shaderProgramID, shader.m_id);
    }

    glLinkProgram(m_shaderProgramID);

    // make sure the shader program linked successfully
    glValidateProgram(m_shaderProgramID);
    glGetProgramiv(m_shaderProgramID, GL_VALIDATE_STATUS, &success);
    if (!success) {
        std::cerr << "Shader Program Linking Failed\n";
    }
}

std::pair<unsigned int, std::string> ShaderProgram::parseShader(const std::string& filePath) const {
    std::ifstream stream(filePath);
    std::string shader, line;
    getline(stream, line);
    unsigned int type = getShaderType(line);
    while (getline(stream, line)) {
        shader.append(line + '\n');
    }
    return { type, shader };
}

unsigned int ShaderProgram::getShaderType(const std::string& line) const {
    // At the beginning of the each shader file, place a comment with the shader type
    // Ex: // VERTEX
    const int headerTypeOffset = 3; // The 3 chars before the shader type
    if (line.length() >= headerTypeOffset) {
        if (line.substr(headerTypeOffset) == "VERTEX") {
            return GL_VERTEX_SHADER;
        } else if (line.substr(headerTypeOffset) == "FRAGMENT") {
            return GL_FRAGMENT_SHADER;
        }
    }
    std::cerr << "Invalid Shader Header. Place a comment at the top of your shader files "
                 "with the shader type.\nEx: // VERTEX\n";
    return 0;
}

void ShaderProgram::bind() const {
    glUseProgram(m_shaderProgramID);
}

void ShaderProgram::unbind() const {
    glUseProgram(0);
}

void ShaderProgram::addTexture(const Texture* texture, const std::string& name) {
    bind();
    texture->bind();
    addUniform1i(name, texture->getSlot());
}

void ShaderProgram::addUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    bind();
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void ShaderProgram::addUniform1i(const std::string& name, int value) {
    bind();
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::addUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    bind();
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

int ShaderProgram::getUniformLocation(const std::string& name) {
    auto cachedLocation = m_uniformLocationCache.find(name);
    if (cachedLocation != m_uniformLocationCache.end()) {
        return cachedLocation->second;
    }
    int location = glGetUniformLocation(m_shaderProgramID, name.c_str());
    if (location == -1) {
        std::cerr << "The Uniform " + name + " does not exist!\n";
    }
    m_uniformLocationCache[name] = location;
    return location;
}