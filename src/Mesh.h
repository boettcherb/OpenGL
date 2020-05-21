#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <vector>

class Mesh {
	unsigned int m_vertexArrayID;
	unsigned int m_vertexBufferID;

public:
	Mesh(const void* data, unsigned int size, const std::vector<unsigned int>& layout);
	~Mesh();

	void bind() const;
	void unbind() const;
};

#endif