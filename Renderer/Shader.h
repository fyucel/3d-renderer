#pragma once

class Shader
{
public:
	Shader(int openGLVersionMajor, int openGLVersionMinor,
		const std::string& vertexShaderPath,
		const std::string& fragmentShaderPath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void Uniform1i(const std::string& name, int value);
	void UniformMat4fv(const std::string& name, const glm::mat4& matrix);
	void UniformVec3fv(const std::string& name, const glm::vec3& vector);

private:
	unsigned int id;

	std::unordered_map<std::string, int> uniformLocationCache;

	std::string ParseShaderFromFile(const std::string& path,
		int openGLVersionMajor, int openGLVersionMinor);

	unsigned int CompileShaderSource(const std::string& source,
		unsigned int shaderType);

	void AttackAndLinkShader(unsigned int vertexShader,
		unsigned int fragmentShader);

	int UniformLocation(const std::string& name);
};
