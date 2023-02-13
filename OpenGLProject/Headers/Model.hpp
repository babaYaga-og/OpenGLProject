#ifndef MODEL_HPP_
#define MODEL_HPP_
#include <cstdint>
#include <DirectXColors.h>
#include <Shader.hpp>

class Model {
public:
	Model() noexcept;
	virtual ~Model() = default;

	void SetIndexCount(std::int32_t indexCount) noexcept;
	void SetBufferIndices(ShaderProgram* shaderProgram) noexcept;
	void SetModelColour(const DirectX::XMVECTORF32& colour) noexcept;
	void SetModelOffset(const DirectX::XMFLOAT3& offset) noexcept;
	void SetModelScale(float scale) noexcept;

	void Draw() noexcept;

	virtual void PhysicsUpdate() noexcept;

protected:
	std::int32_t m_indexCount;
	DirectX::XMFLOAT4 m_modelColour;
	DirectX::XMFLOAT3 m_modelOffset;
	DirectX::XMFLOAT4X4 m_modelMatrix;
	std::int32_t m_colourIndex;
	std::int32_t m_offsetIndex;
	std::int32_t m_matrixIndex;
};

class OrbitingModel : public Model {
public:
	OrbitingModel() noexcept;

	void PhysicsUpdate() noexcept final;
	void MeasureRadius() noexcept;
	void SetSpeedModifier(float speed) noexcept;

protected:
	[[nodiscard]]
	virtual DirectX::XMFLOAT3 GetAngularOffset() noexcept = 0;

protected:
	float m_angle;
	float m_radius;
	float m_modelDirectionX;
	float m_modelDirectionY;
	float m_speedModifier;
};

class OrbitModelClock : public OrbitingModel {
private:
	[[nodiscard]]
	DirectX::XMFLOAT3 GetAngularOffset() noexcept final;
};

class OrbitModelAntiClock : public OrbitingModel {
private:
	[[nodiscard]]
	DirectX::XMFLOAT3 GetAngularOffset() noexcept final;
};
#endif
