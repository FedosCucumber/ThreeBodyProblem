#include "Shader.h"

#include "Renderer.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

Shader::Shader(const std::string& filepath) :
    m_FilePath(filepath),
    m_RendererID(0)
{
    ShaderProgramSrc src = ParseShader(filepath);
    m_RendererID = CreateShader(src.VertexSrc, src.FragmentSrc);
    Unbind();
}

Shader::~Shader()
{
    GLcall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLcall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLcall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
    GLcall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLcall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLcall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{   
    auto iter = m_LocationCache.insert({ name, glGetUniformLocation(m_RendererID, name.c_str()) }).first;
    return   iter->second;
}



ShaderProgramSrc Shader::ParseShader(const std::string& filepath) {
    std::ifstream inp(filepath);
    std::string str;

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;

    std::stringstream ss[2];
    while (getline(inp, str)) {
        if (str.find("#shader") != std::string::npos)
        {
            if (str.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (str.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << str << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GLcall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLcall(glShaderSource(id, 1, &src, nullptr));
    GLcall(glCompileShader(id));

    int res;
    GLcall(glGetShaderiv(id, GL_COMPILE_STATUS, &res));
    if (res == GL_FALSE)
    {
        int len;
        GLcall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
        char* message = (char*)alloca(len * sizeof(char));
        GLcall(glGetShaderInfoLog(id, len, &len, message));

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << "\n";

        GLcall(glDeleteShader(id));
        return 0;
    }

    return id;

}
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLcall(glAttachShader(program, vs));
    GLcall(glAttachShader(program, fs));
    GLcall(glLinkProgram(program));
    GLcall(glValidateProgram(program));

    GLcall(glDeleteShader(vs));
    GLcall(glDeleteShader(fs));

    return program;
}