#include "Mesh.h"
#include "ShaderProgram.h"

#include <glad/glad.h>

#include <vector>
#include <numeric>

Mesh::Submesh::Submesh(unsigned int vao, unsigned int ibo, unsigned int count, const ShaderProgram* shader)
    : m_vertexArrayID{ vao }, m_indexBufferID{ ibo }, m_indexBufferCount{ count }, m_shader{ shader } {}

Mesh::Mesh(const void* data, unsigned int size, const std::vector<unsigned int>& layout) 
    : m_vbData{ data }, m_vbSize{ size }, m_vbLayout{ layout } {
    glGenBuffers(1, &m_vbID);
}

Mesh::~Mesh() {
    // delete submeshes
    for (const Submesh& mesh : m_meshes) {
        glDeleteVertexArrays(1, &mesh.m_vertexArrayID);
        glDeleteBuffers(1, &mesh.m_indexBufferID);
    }

    // delete vertex buffer
    glDeleteBuffers(1, &m_vbID);
}

void Mesh::addSubmesh(const void* ibData, unsigned int count, const ShaderProgram* shader) {
    // create and bind vertex array
    unsigned int vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // bind and set up vertex buffer
    setVertexBuffer();

    // create and bind index buffer
    unsigned int indexBufferID = setIndexBuffer(ibData, count);

    // unbind everything (vertex array first)
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    m_meshes.emplace_back(vertexArrayID, indexBufferID, count, shader);
}

void Mesh::render() const {
    for (const Submesh& mesh : m_meshes) {
        glBindVertexArray(mesh.m_vertexArrayID);
        mesh.m_shader->bind();
        glDrawElements(GL_TRIANGLES, mesh.m_indexBufferCount, GL_UNSIGNED_INT, nullptr);
    }
}

void Mesh::setVertexBuffer() const {
    // bind vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbID);

    // pass in vertex data to OpenGL
    glBufferData(GL_ARRAY_BUFFER, m_vbSize, m_vbData, GL_STATIC_DRAW);

    // calculate the number of bytes of each vertex
    // std::accumulate sums up the values in the vertex buffer layout
    int stride = std::accumulate(m_vbLayout.begin(), m_vbLayout.end(), 0) * sizeof(float);

    // tell openGL the layout of our vertex data.
    unsigned long long offset = 0;
    for (unsigned int i = 0; i < m_vbLayout.size(); ++i) {
        // offset is the number of bytes from the start of the data, but OpenGL
        // reads this information in as a const void pointer
        const void* offsetPtr = reinterpret_cast<const void*>(offset);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, m_vbLayout[i], GL_FLOAT, false, stride, offsetPtr);
        offset += m_vbLayout[i] * sizeof(float);
    }
}

unsigned int Mesh::setIndexBuffer(const void* data, unsigned int count) const {
    unsigned int indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    return indexBufferID;
}