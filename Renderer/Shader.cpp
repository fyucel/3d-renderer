#include "Utilities.h"
#include "Shader.h"

Shader::Shader(int openGLVersionMajor, int openGLVersionMinor,
	const std::string& vertexShaderPath,
	const std::string& fragmentShaderPath)
{
	std::string vertexShaderSource = ParseShaderFromFile(
		vertexShaderPath, openGLVersionMajor, openGLVersionMinor);
	std::string fragmentShaderSource = ParseShaderFromFile(
		fragmentShaderPath, openGLVersionMajor, openGLVersionMinor);

	unsigned int vertexShader = CompileShaderSource(
		vertexShaderSource, GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShaderSource(
		fragmentShaderSource, GL_FRAGMENT_SHADER);

	AttackAndLinkShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	GL(glDeleteProgram(id));
}

void Shader::Bind() const
{
	GL(glUseProgram(id));
}

void Shader::Unbind() const
{
	GL(glUseProgram(0));
}

void Shader::Uniform1i(const std::string& name, int value)
{
	GL(glUniform1i(UniformLocation(name), value));
}

void Shader::UniformMat4fv(const std::string& name, const glm::mat4& matrix)
{
	GL(glUniformMatrix4fv(UniformLocation(name), 1,
		GL_FALSE, glm::value_ptr(matrix)));
}

void Shader::UniformVec3fv(const std::string& name, const glm::vec3& vector)
{
	GL(glUniform3fv(UniformLocation(name), 1, glm::value_ptr(vector)));
}

std::string Shader::ParseShaderFromFile(const std::string& path,
	int openGLVersionMajor, int openGLVersionMinor)
{
	std::ifstream input(path);

	if (!input)
		throw std::string("Failed to open shader file: " + path);

	std::stringstream source;
	std::string line;

	// Replace first line of the shader input with the set OpenGL version
	getline(input, line);
	source << "#version " << openGLVersionMajor << openGLVersionMinor << "0\n";

	while (getline(input, line))
		source << line << "\n";

	input.close();
	return source.str();
}

unsigned int Shader::CompileShaderSource(const std::string& source,
	unsigned int shaderType)
{
	int compiledShaderID = glCreateShader(shaderType);
	const char* sourcePointer = source.c_str();
	GL(glShaderSource(compiledShaderID, 1, &sourcePointer, nullptr));
	GL(glCompileShader(compiledShaderID));

	int result;
	GL(glGetShaderiv(compiledShaderID, GL_COMPILE_STATUS, &result));

	// Shader did not compile successfully
	if (result == GL_FALSE)
	{
		int errorMessageLength;
		GL(glGetShaderiv(compiledShaderID, GL_INFO_LOG_LENGTH,
			&errorMessageLength));
		char* errorMessage = new char[errorMessageLength];
		GL(glGetShaderInfoLog(compiledShaderID, errorMessageLength,
			&errorMessageLength,
			errorMessage));

		std::cout << "Failed to compile shader " << errorMessage << std::endl;

		delete[] errorMessage;
		GL(glDeleteShader(compiledShaderID));
	}
	return compiledShaderID;
}

void Shader::AttackAndLinkShader(unsigned int vertexShader,
	unsigned int fragmentShader)
{
	id = glCreateProgram();

	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);

	glLinkProgram(id);
	glValidateProgram(id);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

int Shader::UniformLocation(const std::string& name)
{
	if (uniformLocationCache.find(name) != uniformLocationCache.end())
		return uniformLocationCache.at(name);

	GL(int location = glGetUniformLocation(id, name.c_str()));
	uniformLocationCache[name] = location;
	return location;
}
