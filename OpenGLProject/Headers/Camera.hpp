#ifndef CAMERA_HPP_
#define CAMERA_HPP_
#include <DirectXMath.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.hpp"

enum class direction {
	enm_up,
	enm_down,
	enm_left,
	enm_right,
};

class Camera {
public:
	Camera() noexcept;

	void Set3DPersonView() noexcept;
	void SetTopDownView() noexcept;

	[[nodiscard]]
	DirectX::XMMATRIX GetViewMatrix() const noexcept;
	[[nodiscard]]
	DirectX::XMMATRIX GetProjectionMatrix(int width, int height) noexcept;


public:
	// Prevents the camera from jumping around when first clicking left click
	bool m_firstMouseClick = true;

	// Stores the width and height of the window
	int m_windowWidth = 1280;
	int m_windowHeight = 720;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float m_camSpeed = 0.1f;
	float m_camSensitivity = 50.0f;

	void move(direction d);
	void Matrix(float FOVdeg, float nearPlane, float farPlane, ShaderProgram& shader, const char* uniform);
	void Inputs(GLFWwindow* wnd);

public:
	DirectX::XMVECTOR m_cameraPosition;
	DirectX::XMVECTOR m_focusDirection;
	float m_FOV = 60;
	DirectX::XMVECTOR m_up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
	DirectX::XMVECTOR m_right = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
};
#endif
