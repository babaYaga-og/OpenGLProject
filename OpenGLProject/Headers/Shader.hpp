#ifndef SHADER_HPP_
#define SHADER_HPP_
#include <cstdint>
#include <string>
#include <vector>

enum class ShaderType {
	Fragment,
	Vertex
};

class Shader {
public:
	Shader() noexcept;
	~Shader() noexcept;

	void CreateShader(const char* shaderSrc, ShaderType type) noexcept;
	void CreateShader(const std::wstring& fileName, ShaderType type);

	[[nodiscard]]
	std::uint32_t GetShaderID() const noexcept;

private:
	[[nodiscard]]
	std::vector<char> LoadText(const std::wstring& fileName);

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
