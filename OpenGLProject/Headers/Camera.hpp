#ifndef CAMERA_HPP_
#define CAMERA_HPP_
#include <DirectXMath.h>

class Camera {
public:
	Camera() noexcept;

	void SetCameraPosition(const DirectX::XMFLOAT3& cameraPostion) noexcept;

	[[nodiscard]]
	DirectX::XMMATRIX GetViewMatrix() const noexcept;
	[[nodiscard]]
	static DirectX::XMMATRIX GetProjectionMatrix(int width, int height) noexcept;

private:
	DirectX::XMVECTOR m_cameraPosition;
};
#endif
