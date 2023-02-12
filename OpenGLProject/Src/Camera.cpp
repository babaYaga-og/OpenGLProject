#include <Camera.hpp>

Camera::Camera() noexcept : m_cameraPosition{ DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f) } {};

void Camera::SetCameraPosition(const DirectX::XMFLOAT3& cameraPostion) noexcept {
	m_cameraPosition = DirectX::XMLoadFloat3(&cameraPostion);
}

using namespace DirectX;

DirectX::XMMATRIX Camera::GetViewMatrix() const noexcept {
	static const DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
	static const DirectX::XMVECTOR focusDirection = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);

	return DirectX::XMMatrixLookAtLH(
		m_cameraPosition,
		m_cameraPosition + focusDirection,
		upVector
	);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(int width, int height) noexcept {
	return 	DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(65.f),
		static_cast<float>(width) / height,
		0.1f, 100.f
	);
}
