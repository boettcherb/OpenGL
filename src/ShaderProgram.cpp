#include "ShaderProgram.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
}

ShaderProgram::~ShaderProgram() {
    for (const auto& shader : m_shaders) {
        glDeleteShader(shader.m_id);
    }
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
            std::cout << "Shader Compilation Failed\n" << infoLog << '\n';
        }
        glAttachShader(m_shaderProgramID, shader.m_id);
    }

    glLinkProgram(m_shaderProgramID);

    // make sure the shader program linked successfully
    glValidateProgram(m_shaderProgramID);
    glGetProgramiv(m_shaderProgramID, GL_VALIDATE_STATUS, &success);
    if (!success) {
        std::cout << "Shader Program Linking Failed\n";
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
    std::cout << "Invalid Shader Header. Place a comment at the top of your shader files "
                 "with the shader type.\nEx: // VERTEX\n";
    return 0;
}

void ShaderProgram::bind() const {
    glUseProgram(m_shaderProgramID);
}

void ShaderProgram::unbind() const {
    glUseProgram(0);
}