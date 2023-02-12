#include <Engine.hpp>
#include <Camera.hpp>

Engine::Engine(int width, int height, const char* windowTitle)
	: m_windowHandle{ nullptr }, m_viewMatrix{}, m_viewMatrixIndex{ 0 }, m_projectionMatrix{},
	m_projectionMatrixIndex{ 0 } {
	InitOpenGL(width, height, windowTitle);

	m_shaderProgram = std::make_unique<ShaderProgram>();
	{
		// Vertex Shader
		Shader vertexShader{};
		vertexShader.CreateShader(L"shaders/VertexShader.vert", ShaderType::Vertex);
		// Fragment Shader
		Shader fragmentShader{};
		fragmentShader.CreateShader(L"shaders/FragmentShader.frag", ShaderType::Fragment);

		m_shaderProgram->AttachShader(vertexShader.GetShaderID())
			.AttachShader(fragmentShader.GetShaderID()).LinkProgram();
	}

	SetCamera(width, height);

	SphereInputs sphereInput{ 64u, 64u };
	sphereInput.InitData();
	const std::int32_t sphereIndexCount = sphereInput.GetIndexCount();

	auto sphere0 = std::make_unique<Model>();
	sphere0->SetIndexCount(sphereIndexCount);
	sphere0->SetBufferIndices(m_shaderProgram.get());
	sphere0->SetModelColour(DirectX::Colors::Tomato);
	sphere0->SetModelScale(0.2f);
	sphere0->SetModelOffset({ -1.9f, 0.f, 0.f });
	m_models.emplace_back(std::move(sphere0));

	auto sphere1 = std::make_unique<Model>();
	sphere1->SetIndexCount(sphereIndexCount);
	sphere1->SetBufferIndices(m_shaderProgram.get());
	sphere1->SetModelColour(DirectX::Colors::Crimson);
	sphere1->SetModelScale(0.2f);
	sphere1->SetModelOffset({ 1.9f, 0.f, 0.f });
	m_models.emplace_back(std::move(sphere1));

	m_vertexArray = std::make_unique<VertexArray>();
	m_vertexArray->AddVertexBuffer(sphereInput.GetVertexData())
		.AddIndexBuffer(sphereInput.GetIndexData()).AddVertexAttribute();
}

Engine::~Engine() noexcept {
	glfwTerminate();
}

void Engine::Run() {
	while (!glfwWindowShouldClose(m_windowHandle)) {
		processInput(m_windowHandle);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shaderProgram->BindShaderProgram();
		m_vertexArray->BindVertexArray();

		BindCameraBuffer();

		for (auto& model : m_models)
			model->Draw();

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

void Engine::SetCamera(int width, int height) noexcept {
	Camera camera{};
	DirectX::XMStoreFloat4x4(&m_viewMatrix, camera.GetViewMatrix());
	DirectX::XMStoreFloat4x4(
		&m_projectionMatrix, camera.GetProjectionMatrix(width, height)
	);

	m_viewMatrixIndex = m_shaderProgram->GetUniformLocation("viewMatrix");
	m_projectionMatrixIndex = m_shaderProgram->GetUniformLocation("projectionMatrix");
}

void Engine::BindCameraBuffer() noexcept {
	glUniformMatrix4fv(m_viewMatrixIndex, 1u, GL_FALSE, m_viewMatrix.m[0]);
	glUniformMatrix4fv(m_projectionMatrixIndex, 1u, GL_FALSE, m_projectionMatrix.m[0]);
}
