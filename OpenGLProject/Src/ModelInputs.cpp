#include <ModelInputs.hpp>
#include <cassert>

const std::vector<Vertex>& ModelInputs::GetVertexData() const noexcept {
	return m_vertices;
}

const std::vector<std::uint32_t>& ModelInputs::GetIndexData() const noexcept {
	return m_indices;
}

std::int32_t ModelInputs::GetIndexCount() const noexcept {
	return static_cast<std::int32_t>(std::size(m_indices));
}

// Sphere
SphereInputs::SphereInputs(std::uint32_t latDiv, std::uint32_t longDiv) noexcept
	: m_latitudeDivision{ latDiv }, m_longitudeDivision{ longDiv } {}

void SphereInputs::InitData() noexcept {
	assert(m_latitudeDivision > 2);
	assert(m_longitudeDivision > 2);

	constexpr float radius = 1.f;
	const DirectX::XMVECTOR base = DirectX::XMVectorSet(0.f, 0.f, radius, 0.f);
	const float lattitudeAngle = DirectX::XM_PI / m_latitudeDivision;
	const float longitudeAngle = DirectX::XM_2PI / m_longitudeDivision;

	// Vertices
	for (std::uint32_t iLat = 1u; iLat < m_latitudeDivision; iLat++) {
		DirectX::XMVECTOR latBase = DirectX::XMVector3Transform(
			base, DirectX::XMMatrixRotationX(lattitudeAngle * iLat)
		);

		for (std::uint32_t iLong = 0u; iLong < m_longitudeDivision; iLong++) {
			DirectX::XMVECTOR v = DirectX::XMVector3Transform(
				latBase, DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
			);

			Vertex vertex{};
			DirectX::XMStoreFloat3(&vertex.position, v);
			m_vertices.emplace_back(vertex);
		}
	}

	// Cap vertices
	const auto iNorthPole = static_cast<std::uint32_t>(std::size(m_vertices));
	Vertex vertex{};
	DirectX::XMStoreFloat3(&vertex.position, base);
	m_vertices.emplace_back(vertex);

	const auto iSouthPole = static_cast<std::uint32_t>(std::size(m_vertices));
	DirectX::XMStoreFloat3(&vertex.position, DirectX::XMVectorNegate(base));
	m_vertices.emplace_back(vertex);

	// Indices
	const auto calcIndex = [longDiv = m_longitudeDivision]
	(std::uint32_t iLat, std::uint32_t iLong)->std::uint32_t {
		return iLat * longDiv + iLong;
	};

	for (std::uint32_t iLat = 0u; iLat < m_latitudeDivision - 2u; iLat++) {
		for (std::uint32_t iLong = 0u; iLong < m_longitudeDivision - 1u; iLong++) {
			m_indices.emplace_back(calcIndex(iLat, iLong));
			m_indices.emplace_back(calcIndex(iLat + 1u, iLong));
			m_indices.emplace_back(calcIndex(iLat, iLong + 1u));
			m_indices.emplace_back(calcIndex(iLat, iLong + 1u));
			m_indices.emplace_back(calcIndex(iLat + 1u, iLong));
			m_indices.emplace_back(calcIndex(iLat + 1u, iLong + 1u));
		}
		// Wrap band
		m_indices.emplace_back(calcIndex(iLat, m_longitudeDivision - 1u));
		m_indices.emplace_back(calcIndex(iLat + 1u, m_longitudeDivision - 1u));
		m_indices.emplace_back(calcIndex(iLat, 0u));
		m_indices.emplace_back(calcIndex(iLat, 0u));
		m_indices.emplace_back(calcIndex(iLat + 1u, m_longitudeDivision - 1u));
		m_indices.emplace_back(calcIndex(iLat + 1u, 0u));
	}

	// Cap fans
	for (std::uint32_t iLong = 0u; iLong < m_longitudeDivision - 1u; iLong++) {
		// North
		m_indices.emplace_back(iNorthPole);
		m_indices.emplace_back(calcIndex(0u, iLong));
		m_indices.emplace_back(calcIndex(0u, iLong + 1u));
		// South
		m_indices.emplace_back(calcIndex(m_latitudeDivision - 2u, iLong + 1u));
		m_indices.emplace_back(calcIndex(m_latitudeDivision - 2u, iLong));
		m_indices.emplace_back(iSouthPole);
	}
	// Wrap Triangles
	// North
	m_indices.emplace_back(iNorthPole);
	m_indices.emplace_back(calcIndex(0u, m_longitudeDivision - 1u));
	m_indices.emplace_back(calcIndex(0u, 0u));
	// South
	m_indices.emplace_back(calcIndex(m_latitudeDivision - 2u, 0u));
	m_indices.emplace_back(calcIndex(m_latitudeDivision - 2u, m_longitudeDivision - 1u));
	m_indices.emplace_back(iSouthPole);
}
