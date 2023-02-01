#include <VertexArray.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

VertexArray::VertexArray() noexcept : m_vertexArrayID{ 0u } {
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
}

VertexArray::~VertexArray() noexcept {
	glDeleteVertexArrays(1, &m_vertexArrayID);

	for (auto& bufferID : m_bufferIDs)
		glDeleteBuffers(1, &bufferID);
}

void VertexArray::AddBuffer(
	std::uint32_t bufferType, const void* data, size_t dataSize
) noexcept {
	std::uint32_t bufferID{ 0u };
	glGenBuffers(1, &bufferID);

	glBindBuffer(bufferType, bufferID);
	glBufferData(bufferType, dataSize, data, GL_STATIC_DRAW);

	m_bufferIDs.emplace_back(bufferID);
}

VertexArray& VertexArray::AddVertexBuffer(const std::vector<Vertex>& data) noexcept {
	AddBuffer(GL_ARRAY_BUFFER, std::data(data), std::size(data) * sizeof(Vertex));

	return *this;
}

VertexArray& VertexArray::AddIndexBuffer(const std::vector<std::uint32_t>& data) noexcept {
	AddBuffer(
		GL_ELEMENT_ARRAY_BUFFER, std::data(data), std::size(data) * sizeof(std::uint32_t)
	);

	return *this;
}

void VertexArray::AddVertexAttribute() const noexcept {
	std::uint32_t attributeIndex = 0u;

	glVertexAttribPointer(attributeIndex, 3, GL_FLOAT, GL_FALSE, 3u * sizeof(float), nullptr);
	++attributeIndex;

	for (std::uint32_t index = 0u; index < attributeIndex; ++index)
		glEnableVertexAttribArray(index);
}

void VertexArray::BindVertexArray() const noexcept {
	glBindVertexArray(m_vertexArrayID);
}
