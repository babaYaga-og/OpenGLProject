#ifndef ENGINE_HPP_
#define ENGINE_HPP_
#include <memory>
#include <vector>
#include <TimeManager.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
#include <VertexArray.hpp>
#include <Model.hpp>

class Engine {
public:
	Engine(int width, int height, const char* windowTitle);
	~Engine() noexcept;

	void Run();

private:
	void InitOpenGL(int width, int height, const char* windowTitle);
	void processInput(GLFWwindow* window) noexcept;
	void SetCamera(int width, int height) noexcept;
	void BindCameraBuffer() noexcept;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* m_windowHandle;
	std::unique_ptr<ShaderProgram> m_shaderProgram;
	std::unique_ptr<VertexArray> m_vertexArray;
	std::vector<std::unique_ptr<Model>> m_models;
	DirectX::XMFLOAT4X4 m_viewMatrix;
	DirectX::XMFLOAT4X4 m_projectionMatrix;
	std::int32_t m_viewMatrixIndex;
	std::int32_t m_projectionMatrixIndex;
	FrameTime m_frameTime;
	bool m_cameraTopDown;
};
#endif
