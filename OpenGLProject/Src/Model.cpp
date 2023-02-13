#include <Model.hpp>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Model::Model() noexcept
	: m_indexCount{ 0 }, m_modelColour{ 0.5f, 0.5f, 0.5f, 1.f }, m_modelOffset{ 0.f, 0.f, 0.f },
	m_modelMatrix{}, m_colourIndex{ 0 }, m_offsetIndex{ 0 }, m_matrixIndex{ 0 } {
	DirectX::XMStoreFloat4x4(&m_modelMatrix, DirectX::XMMatrixIdentity());
}

void Model::SetIndexCount(std::int32_t indexCount) noexcept {
	m_indexCount = indexCount;
}

void Model::SetBufferIndices(ShaderProgram* shaderProgram) noexcept {
	m_colourIndex = shaderProgram->GetUniformLocation("modelColour");
	m_offsetIndex = shaderProgram->GetUniformLocation("modelOffset");
	m_matrixIndex = shaderProgram->GetUniformLocation("modelMatrix");
}

void Model::SetModelColour(const DirectX::XMVECTORF32& colour) noexcept {
	m_modelColour.x = colour.f[0];
	m_modelColour.y = colour.f[1];
	m_modelColour.z = colour.f[2];
	m_modelColour.w = colour.f[3];
}

void Model::SetModelOffset(const DirectX::XMFLOAT3& offset) noexcept {
	m_modelOffset = offset;
}

void Model::SetModelScale(float scale) noexcept {
	DirectX::XMStoreFloat4x4(&m_modelMatrix, DirectX::XMMatrixScaling(scale, scale, scale));
}

void Model::Draw() noexcept {
	glUniform4f(
		m_colourIndex, m_modelColour.x, m_modelColour.y, m_modelColour.z, m_modelColour.w
	);
	glUniform3f(m_offsetIndex, m_modelOffset.x, m_modelOffset.y, m_modelOffset.z);
	glUniformMatrix4fv(m_matrixIndex, 1u, GL_FALSE, m_modelMatrix.m[0]);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

void Model::PhysicsUpdate() noexcept {}

// Orbital Model
OrbitingModel::OrbitingModel() noexcept
	: m_angle{ 0.f }, m_radius{ 0.f }, m_modelDirectionX{ 0.f }, m_modelDirectionY{ 0.f },
	m_speedModifier{ 0.05f } {}

void OrbitingModel::PhysicsUpdate() noexcept {
	const DirectX::XMFLOAT3 offset = GetAngularOffset();
	m_modelOffset = offset;

	m_angle += m_speedModifier;
	if (m_angle > DirectX::XM_2PI)
		m_angle = 0.f;
}

void OrbitingModel::SetSpeedModifier(float speed) noexcept {
	m_speedModifier = speed;
}

void OrbitingModel::MeasureRadius() noexcept {
	const DirectX::XMFLOAT3& modelLocation = m_modelOffset;
	m_radius = std::sqrt(
		std::pow(modelLocation.x, 2.f) + std::pow(modelLocation.y, 2.f) +
		std::pow(modelLocation.z, 2.f)
	);

	// Direction of X and Y decides the orbit direction
	m_modelDirectionX = modelLocation.x / m_radius;
	m_modelDirectionY = modelLocation.y / m_radius;
}

// Orbit ClockWise
DirectX::XMFLOAT3 OrbitModelClock::GetAngularOffset() noexcept {
	return {
		m_modelDirectionX * std::sin(m_angle) * m_radius,
		m_modelDirectionY * std::sin(m_angle) * m_radius,
		std::cos(m_angle) * m_radius
	};
}

// Orbit Anti-Clockwise
DirectX::XMFLOAT3 OrbitModelAntiClock::GetAngularOffset() noexcept {
	return {
		m_modelDirectionX * std::cos(m_angle) * m_radius,
		m_modelDirectionY * std::cos(m_angle) * m_radius,
		std::sin(m_angle) * m_radius
	};
}
