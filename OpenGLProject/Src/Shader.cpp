#include <Shader.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader::Shader(const char* shaderSrc, ShaderType type) noexcept : m_shaderID{ 0u } {
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

Shader::~Shader() noexcept {
	glDeleteShader(m_shaderID);
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

void ShaderProgram::LinkProgram() const noexcept {
	glLinkProgram(m_programID);
}

void ShaderProgram::BindShaderProgram() noexcept {
	glUseProgram(m_programID);
}
