#include <Camera.hpp>

Camera::Camera() noexcept
	: m_cameraPosition{ DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f) },
	m_focusDirection{ DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f) } {};

void Camera::Set3DPersonView() noexcept {
	Camera::Camera::m_cameraPosition = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
	Camera::Camera::m_focusDirection = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
}

void Camera::SetTopDownView() noexcept {
	m_FOV = 20;
	Camera::Camera::m_cameraPosition = DirectX::XMVectorSet(0.f, 1.f, 1.0f, 0.f);
	Camera::Camera::m_focusDirection = DirectX::XMVectorSet(0.f, -1.f, -1.0f, 0.f);

}

using namespace DirectX;

DirectX::XMMATRIX Camera::GetViewMatrix() const noexcept {
	static const DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);

	return DirectX::XMMatrixLookAtLH(
		Camera::Camera::m_cameraPosition,
		Camera::Camera::m_cameraPosition + Camera::Camera::m_focusDirection, // focus m_Position
		upVector // UpVector
	);

	//return glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(int width, int height) noexcept {
	return 	DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(m_FOV),
		static_cast<float>(width) / height,
		0.1f, 100.f
	);
}

void Camera::move(direction enm_d)
{
	// this needs working

}

void Camera::Inputs(GLFWwindow* window)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		std::cout << "W key pressed\n";
		m_cameraPosition += m_camSpeed * m_focusDirection;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_cameraPosition += m_camSpeed * XMVector3Normalize(XMVector3Cross(m_focusDirection, m_up));
		//m_cameraPosition += m_camSpeed * -glm::normalize(glm::cross(m_Orientation, m_Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_cameraPosition += m_camSpeed * -m_focusDirection;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_cameraPosition += m_camSpeed * -XMVector3Normalize(XMVector3Cross(m_focusDirection, m_up));
		//m_cameraPosition += m_camSpeed * glm::normalize(glm::cross(m_Orientation, m_Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_cameraPosition += m_camSpeed * m_up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_cameraPosition += m_camSpeed * -m_up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		m_camSpeed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		m_camSpeed = 0.1f;
	}

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (m_firstMouseClick)
		{
			glfwSetCursorPos(window, (m_windowWidth / 2), (m_windowHeight / 2));
			m_firstMouseClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = m_camSensitivity * (float)(mouseY - (m_windowHeight / 2)) / m_windowHeight;
		float rotY = m_camSensitivity * (float)(mouseX - (m_windowWidth / 2)) / m_windowWidth;

		// Calculates upcoming vertical change in the Orientation
		float angleX = XMConvertToRadians(-rotX);
		float angleY = XMConvertToRadians(-rotY);
		XMVECTOR rad90 = { XMConvertToRadians(90.0f) };
		XMVECTOR rad85 = { XMConvertToRadians(89.0f) };

		XMVECTOR normalAxis = XMVector3Normalize(XMVector3Cross(m_focusDirection, m_up));

		XMVECTOR quatX = XMQuaternionRotationNormal(normalAxis, angleX);
		XMVECTOR quatY = XMQuaternionRotationNormal(normalAxis, angleY);

		XMVECTOR newOri = XMVector3Rotate(m_focusDirection, quatY);


		// Decides whether or not the next vertical Orientation is legal or not
		if ((XMVectorAbs((XMVector3AngleBetweenVectors(newOri, m_up) - rad90)).m128_f32 <= rad85.m128_f32)) {
			m_focusDirection = newOri;
		}

		// Decides whether or not the next vertical Orientation is legal or not

	// Rotates the Orientation left and right or up and down... depends on quatX/Y
		//m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);
		m_focusDirection = XMVector3Rotate(m_focusDirection, quatX);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (m_windowWidth / 2), (m_windowHeight / 2));
		//glfwSetCursorPos(window, mouseX, mouseY); causes the camera to move continiously
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{

		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		m_firstMouseClick = true;
	}
}