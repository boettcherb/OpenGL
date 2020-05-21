#include "Mesh.h"

#include <glad/glad.h>

#include <vector>
#include <numeric>

Mesh::Mesh(const void* data, unsigned int size, const std::vector<unsigned int>& layout) {
    // create/bind vertex array
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    // create/bind vertex buffer
    glGenBuffers(1, &m_vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    // calculate the number of bytes of each vertex
    // std::accumulate sums up the values in layout
    int stride = std::accumulate(layout.begin(), layout.end(), 0) * sizeof(float);

    // tell openGL the layout of our vertex data.
    // offset is the number of bytes from the start of the data, but OpenGL
    // reads this information in as a const void pointer
    unsigned long long offset = 0;
    for (unsigned int i = 0; i < layout.size(); ++i) {
        glEnableVertexAttribArray(i);
        const void* offsetPtr = reinterpret_cast<const void*>(offset);
        glVertexAttribPointer(i, layout[i], GL_FLOAT, false, stride, offsetPtr);
        offset += layout[i] * sizeof(float);
    }

    // unbind the vertex buffer (it is no longer needed)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() {
    // delete vertex array
    glDeleteVertexArrays(1, &m_vertexArrayID);

    // delete vertex buffer
    glDeleteBuffers(1, &m_vertexBufferID);
}

void Mesh::bind() const {
    glBindVertexArray(m_vertexArrayID);
}

void Mesh::unbind() const {
    glBindVertexArray(0);
}