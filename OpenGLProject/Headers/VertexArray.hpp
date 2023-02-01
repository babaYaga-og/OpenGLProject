#ifndef VERTEX_ARRAY_HPP_
#define VERTEX_ARRAY_HPP_
#include <cstdint>
#include <vector>
#include <ModelInputs.hpp>

class VertexArray {
public:
	VertexArray() noexcept;
	~VertexArray() noexcept;

	[[nodiscard]]
	VertexArray& AddVertexBuffer(const std::vector<Vertex>& data) noexcept;
	[[nodiscard]]
	VertexArray& AddIndexBuffer(const std::vector<std::uint32_t>& data) noexcept;

	void AddVertexAttribute() const noexcept;
	void BindVertexArray() const noexcept;

private:
	void AddBuffer(
		std::uint32_t bufferType, const void* data, size_t dataSize
	) noexcept;

private:
	std::uint32_t m_vertexArrayID;
	std::vector<std::uint32_t> m_bufferIDs;
};
#endif
