#include <Engine.hpp>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

Engine::Engine(int width, int height, const char* windowTitle)
	: m_windowHandle{ nullptr }, m_sphereIndices{ 0 } {
	InitOpenGL(width, height, windowTitle);

	m_shaderProgram = std::make_unique<ShaderProgram>();
	{
		// Vertex Shader
		Shader vertexShader{ vertexShaderSource, ShaderType::Vertex };
		// Fragment Shader
		Shader fragmentShader{ fragmentShaderSource, ShaderType::Fragment };

		m_shaderProgram->AttachShader(vertexShader.GetShaderID())
			.AttachShader(fragmentShader.GetShaderID()).LinkProgram();
	}

	SphereInputs sphere{ 64u, 64u };
	sphere.InitData();
	m_sphereIndices = sphere.GetIndexCount();

	m_vertexArray = std::make_unique<VertexArray>();
	m_vertexArray->AddVertexBuffer(sphere.GetVertexData())
		.AddIndexBuffer(sphere.GetIndexData()).AddVertexAttribute();
}

Engine::~Engine() noexcept {
	glfwTerminate();
}

void Engine::Run() {
	while (!glfwWindowShouldClose(m_windowHandle)) {
		processInput(m_windowHandle);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shaderProgram->BindShaderProgram();
		m_vertexArray->BindVertexArray();

		glDrawElements(GL_TRIANGLES, m_sphereIndices, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(m_windowHandle);
		glfwPollEvents();
	}
}

void Engine::InitOpenGL(int width, int height, const char* windowTitle) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_windowHandle = glfwCreateWindow(width, height, windowTitle, nullptr, nullptr);
	glfwMakeContextCurrent(m_windowHandle);
	glfwSetFramebufferSizeCallback(m_windowHandle, framebuffer_size_callback);

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

void Engine::processInput(GLFWwindow* window) const noexcept {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
