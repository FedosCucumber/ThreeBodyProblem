#pragma once
#include "Renderer.h"
#include <GLFW/glfw3.h>

class ShaderStorageBuffer
{
public:
	ShaderStorageBuffer(const void* data, unsigned int size);
	~ShaderStorageBuffer();

	void Bind();
	void Unbind();

private:
	unsigned int m_RendererID;
};

