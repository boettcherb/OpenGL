#ifndef SHADER_PROGRAM_H_INCLUDED
#define SHADER_PROGRAM_H_INCLUDED

#include <string>
#include <vector>

class ShaderProgram {

	struct Shader {
		unsigned int m_type;
		unsigned int m_id;
		std::string m_source;
		Shader(unsigned int type, unsigned int id, const std::string& source);
	};

	std::vector<Shader> m_shaders;
	unsigned int m_shaderProgramID;

public:
	ShaderProgram(const std::vector<std::string>& filePaths);
	~ShaderProgram();

	void bind() const;
	void unbind() const;

private:
	void compileAndLink() const;
	std::pair<unsigned int, std::string> parseShader(const std::string& filePath) const;
	unsigned int getShaderType(const std::string& line) const;
};

#endif