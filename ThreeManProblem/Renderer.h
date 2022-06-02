#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(X) if (!(X)) __debugbreak();

#define GLcall(X) GLCheckError();\
    X;\
    //ASSERT(GLLogCall(#X, __FILE__, __LINE__))


void GLClearError();

void GLCheckError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer 
{
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear() const;
};