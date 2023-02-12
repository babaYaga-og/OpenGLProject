#include <Shader.hpp>
#include <fstream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader::Shader() noexcept : m_shaderID{ 0u } {}

Shader::~Shader() noexcept {
	glDeleteShader(m_shaderID);
}

void Shader::CreateShader(const char* shaderSrc, ShaderType type) noexcept {
	switch (type) {
	case ShaderType::Fragment: {
		m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	}
	case ShaderType::Vertex: {
		m_shaderID = glCreateShader(GL_VERTEX_SHADER);
		break;
	}
	}

	glShaderSource(m_shaderID, 1u, &shaderSrc, nullptr);
	glCompileShader(m_shaderID);
}

void Shader::CreateShader(const std::wstring& fileName, ShaderType type) {
	std::vector<char> shaderCode = LoadText(fileName);
	CreateShader(std::data(shaderCode), type);
}

std::vector<char> Shader::LoadText(const std::wstring& fileName) {
	std::ifstream shader(fileName.c_str(), std::ios_base::ate);

	const size_t shaderSize = static_cast<size_t>(shader.tellg());
	shader.seekg(0u);

	std::vector<char> byteCode(shaderSize);
	shader.read(std::data(byteCode), static_cast<std::streamsize>(shaderSize));

	return byteCode;
}

std::uint32_t Shader::GetShaderID() const noexcept {
	return m_shaderID;
}

// Shader Program
ShaderProgram::ShaderProgram() noexcept : m_programID{ glCreateProgram() } {}

ShaderProgram::~ShaderProgram() noexcept {
	glDeleteProgram(m_programID);
}

ShaderProgram& ShaderProgram::AttachShader(std::uint32_t shaderID) noexcept {
	glAttachShader(m_programID, shaderID);

	return *this;
}

std::int32_t ShaderProgram::GetUniformLocation(const char* variableName) const noexcept {
	return glGetUniformLocation(m_programID, variableName);
}

void ShaderProgram::LinkProgram() const noexcept {
	glLinkProgram(m_programID);
}

void ShaderProgram::BindShaderProgram() noexcept {
	glUseProgram(m_programID);
}
