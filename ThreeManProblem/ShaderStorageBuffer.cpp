#include "ShaderStorageBuffer.h"

ShaderStorageBuffer::ShaderStorageBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_RendererID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    Unbind();
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void ShaderStorageBuffer::Bind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_RendererID);
}

void ShaderStorageBuffer::Unbind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
