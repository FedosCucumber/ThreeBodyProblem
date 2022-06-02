#include "Renderer.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :
    m_Count(count)
{
    GLcall(glGenBuffers(1, &m_RendererID));
    GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));

    Unbind();
}

IndexBuffer::~IndexBuffer()
{
    if (m_RendererID)
    {
        GLcall(glDeleteBuffers(1, &m_RendererID));
    }
}

void IndexBuffer::Bind() const
{
    GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

}

unsigned int IndexBuffer::GetCount() const { return m_Count; };
