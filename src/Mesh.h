#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "ShaderProgram.h"

#include <vector>

class Mesh {

	struct Submesh {
		unsigned int m_vertexArrayID;
		unsigned int m_indexBufferID;
		unsigned int m_indexBufferCount;
		const ShaderProgram* m_shader;
		Submesh(unsigned int vao, unsigned int ibo, unsigned int count, const ShaderProgram* shader);
	};

	unsigned int m_vbID;
	const void* m_vbData;
	unsigned int m_vbSize;
	std::vector<unsigned int> m_vbLayout;
	std::vector<Submesh> m_meshes;

public:
	Mesh(const void* data, unsigned int size, const std::vector<unsigned int>& layout);
	~Mesh();

	void addSubmesh(const void* ibData, unsigned int count, const ShaderProgram* shader);
	void render() const;

private:
	void setVertexBuffer() const;
	unsigned int setIndexBuffer(const void* data, unsigned int count) const;
};

#endif