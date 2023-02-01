#ifndef SHADER_HPP_
#define SHADER_HPP_
#include <cstdint>

enum class ShaderType {
	Fragment,
	Vertex
};

class Shader {
public:
	Shader(const char* shaderSrc, ShaderType type) noexcept;
	~Shader() noexcept;

	[[nodiscard]]
	std::uint32_t GetShaderID() const noexcept;

private:
	std::uint32_t m_shaderID;
};

class ShaderProgram {
public:
	ShaderProgram() noexcept;
	~ShaderProgram() noexcept;

	[[nodiscard]]
	ShaderProgram& AttachShader(std::uint32_t shaderID) noexcept;

	void LinkProgram() const noexcept;
	void BindShaderProgram() noexcept;

private:
	std::uint32_t m_programID;
};
#endif
