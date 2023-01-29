#ifndef ENGINE_HPP_
#define ENGINE_HPP_
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Engine {
public:
	Engine(int width, int height, const char* windowTitle);
	~Engine() noexcept;

	void Run();

private:
	void processInput(GLFWwindow* window) const noexcept;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
	GLFWwindow* m_windowHandle;
};
#endif
