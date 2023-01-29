#include <Engine.hpp>

Engine::Engine(int width, int height, const char* windowTitle) : m_windowHandle{ nullptr } {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_windowHandle = glfwCreateWindow(width, height, windowTitle, nullptr, nullptr);
	glfwMakeContextCurrent(m_windowHandle);
	glfwSetFramebufferSizeCallback(m_windowHandle, framebuffer_size_callback);

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

Engine::~Engine() noexcept {
	glfwTerminate();
}

void Engine::Run() {
	while (!glfwWindowShouldClose(m_windowHandle)) {
		processInput(m_windowHandle);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(m_windowHandle);
		glfwPollEvents();
	}
}

void Engine::processInput(GLFWwindow* window) const noexcept {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
