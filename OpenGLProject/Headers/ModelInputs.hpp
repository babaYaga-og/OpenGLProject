#ifndef MODEL_INPUTS_HPP_
#define MODEL_INPUTS_HPP_
#include <vector>
#include <DirectXMath.h>

struct Vertex {
	DirectX::XMFLOAT3 position;
};

class ModelInputs {
public:
	virtual ~ModelInputs() = default;

	virtual void InitData() noexcept = 0;

	[[nodiscard]]
	const std::vector<Vertex>& GetVertexData() const noexcept;
	[[nodiscard]]
	const std::vector<std::uint32_t>& GetIndexData() const noexcept;
	[[nodiscard]]
	std::int32_t GetIndexCount() const noexcept;

protected:
	std::vector<Vertex> m_vertices;
	std::vector<std::uint32_t> m_indices;
};

class SphereInputs : public ModelInputs {
public:
	SphereInputs(std::uint32_t latDiv, std::uint32_t longDiv) noexcept;

	void InitData() noexcept override;

private:
	std::uint32_t m_latitudeDivision;
	std::uint32_t m_longitudeDivision;
};
#endif
