#ifndef ENGINE_HPP_
#define ENGINE_HPP_
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
#include <VertexArray.hpp>

class Engine {
public:
	Engine(int width, int height, const char* windowTitle);
	~Engine() noexcept;

	void Run();

private:
	void InitOpenGL(int width, int height, const char* windowTitle);
	void processInput(GLFWwindow* window) const noexcept;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* m_windowHandle;
	std::unique_ptr<ShaderProgram> m_shaderProgram;
	std::unique_ptr<VertexArray> m_vertexArray;
	std::int32_t m_sphereIndices;
};
#endif
