#ifndef MODEL_HPP_
#define MODEL_HPP_
#include <cstdint>
#include <DirectXColors.h>
#include <Shader.hpp>

class Model {
public:
	Model() noexcept;

	void SetIndexCount(std::int32_t indexCount) noexcept;
	void SetBufferIndices(ShaderProgram* shaderProgram) noexcept;
	void SetModelColour(const DirectX::XMVECTORF32& colour) noexcept;
	void SetModelOffset(const DirectX::XMFLOAT3& offset) noexcept;
	void SetModelScale(float scale) noexcept;

	void Draw() noexcept;

private:
	std::int32_t m_indexCount;
	DirectX::XMFLOAT4 m_modelColour;
	DirectX::XMFLOAT3 m_modelOffset;
	DirectX::XMFLOAT4X4 m_modelMatrix;
	std::int32_t m_colourIndex;
	std::int32_t m_offsetIndex;
	std::int32_t m_matrixIndex;
};
#endif
