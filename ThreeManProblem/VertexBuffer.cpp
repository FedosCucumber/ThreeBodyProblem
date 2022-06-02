#include "Renderer.h"
#include "VertexBuffer.h"



VertexBuffer::VertexBuffer(const void* data, unsigned int size) 
{ 
    GLcall(glGenBuffers(1, &m_RendererID));
    GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLcall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));

    Unbind();
}

VertexBuffer::~VertexBuffer()
{
    GLcall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Modify(const void* data, unsigned int size)
{
    GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::Bind() const
{
    GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const 
{
    GLcall(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
