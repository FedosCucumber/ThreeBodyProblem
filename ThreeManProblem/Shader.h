#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>





struct ShaderProgramSrc
{
	std::string VertexSrc;
	std::string FragmentSrc;
	
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map <std::string, unsigned int> m_LocationCache;
public:
	Shader(const std::string& filepath = "Basic.shader");
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int v0);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:

	ShaderProgramSrc ParseShader(const std::string& filepath);

	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);

};