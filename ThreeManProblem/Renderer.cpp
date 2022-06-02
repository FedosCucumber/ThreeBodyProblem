#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR) {

    }
}

void GLCheckError()
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error] (" << error << ")\n";
    }
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL error] (" << error << ") " <<
            function << " " << file << ": " << line << std::endl;
        return false;
    }
}
