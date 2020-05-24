#ifndef SHADER_PROGRAM_H_INCLUDED
#define SHADER_PROGRAM_H_INCLUDED

#include "Texture.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <unordered_map>

class ShaderProgram {

	struct Shader {
		unsigned int m_type;
		unsigned int m_id;
		std::string m_source;
		Shader(unsigned int type, unsigned int id, const std::string& source);
	};

	std::vector<Shader> m_shaders;
	unsigned int m_shaderProgramID;
	std::unordered_map<std::string, int> m_uniformLocationCache;

public:
	ShaderProgram(const std::vector<std::string>& filePaths);
	~ShaderProgram();

	void bind() const;
	void unbind() const;
	void addTexture(const Texture* texture, const std::string& name);
	void addUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void addUniform1i(const std::string& name, int value);
	void addUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	void compileAndLink() const;
	std::pair<unsigned int, std::string> parseShader(const std::string& filePath) const;
	unsigned int getShaderType(const std::string& line) const;
	int getUniformLocation(const std::string& name);
};

#endif