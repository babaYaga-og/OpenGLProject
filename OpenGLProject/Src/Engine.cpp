#include <iostream>

#include <Engine.hpp>
#include <Camera.hpp>


Engine::Engine(int width, int height, const char* windowTitle)
	:
	m_windowHandle{ nullptr },
	m_viewMatrix{},
	m_viewMatrixIndex{ 0 },
	m_projectionMatrix{},
	m_projectionMatrixIndex{ 0 },
	m_cameraTopDown{ false },
	m_camera3D{ true }
{

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
			.AttachShader(fragmentShader.GetShaderID())  // method chaining
			.LinkProgram();
	}

	//Camera camera;

	SetCamera(width, height);

	SphereInputs sphereInput{ 64u, 64u };
	sphereInput.InitData();
	const std::int32_t sphereIndexCount = sphereInput.GetIndexCount();



	auto sun = std::make_unique<Model>();
	sun->SetIndexCount(sphereIndexCount);
	sun->SetBufferIndices(m_shaderProgram.get());
	sun->SetModelColour(DirectX::Colors::OrangeRed);
	sun->SetModelScale(0.1f);
	m_models.emplace_back(std::move(sun));

	auto mercury = std::make_unique<OrbitModelClock>();
	mercury->SetIndexCount(sphereIndexCount);
	mercury->SetBufferIndices(m_shaderProgram.get());
	mercury->SetModelColour(DirectX::Colors::SlateGray);
	mercury->SetModelScale(0.02f);
	mercury->SetModelOffset({ 7.f, 0.f, 0.f });
	mercury->MeasureRadius();
	mercury->SetSpeedModifier(0.107f);
	m_models.emplace_back(std::move(mercury));

	auto venus = std::make_unique<OrbitModelClock>();
	venus->SetIndexCount(sphereIndexCount);
	venus->SetBufferIndices(m_shaderProgram.get());
	venus->SetModelColour(DirectX::Colors::LightYellow);
	venus->SetModelScale(0.03f);
	venus->SetModelOffset({ 7.f, 0.f, 0.f });
	venus->MeasureRadius();
	venus->SetSpeedModifier(0.0783f);
	m_models.emplace_back(std::move(venus));

	auto earth = std::make_unique<OrbitModelClock>();
	earth->SetIndexCount(sphereIndexCount);
	earth->SetBufferIndices(m_shaderProgram.get());
	earth->SetModelColour(DirectX::Colors::RoyalBlue);
	earth->SetModelScale(0.035f);
	earth->SetModelOffset({ 8.f, 0.f, 0.f });
	earth->MeasureRadius();
	earth->SetSpeedModifier(0.0666f);
	m_models.emplace_back(std::move(earth));

	auto mars = std::make_unique<OrbitModelClock>();
	mars->SetIndexCount(sphereIndexCount);
	mars->SetBufferIndices(m_shaderProgram.get());
	mars->SetModelColour(DirectX::Colors::IndianRed);
	mars->SetModelScale(0.028f);
	mars->SetModelOffset({ 13.f, 0.f, 0.f });
	mars->MeasureRadius();
	mars->SetSpeedModifier(0.0539f);
	m_models.emplace_back(std::move(mars));

	auto jupiter = std::make_unique<OrbitModelClock>();
	jupiter->SetIndexCount(sphereIndexCount);
	jupiter->SetBufferIndices(m_shaderProgram.get());
	jupiter->SetModelColour(DirectX::Colors::SandyBrown);
	jupiter->SetModelScale(0.06f);
	jupiter->SetModelOffset({ 8.5f, 0.f, 0.f });
	jupiter->MeasureRadius();
	jupiter->SetSpeedModifier(0.0292f);
	m_models.emplace_back(std::move(jupiter));

	auto saturn = std::make_unique<OrbitModelClock>();
	saturn->SetIndexCount(sphereIndexCount);
	saturn->SetBufferIndices(m_shaderProgram.get());
	saturn->SetModelColour(DirectX::Colors::AntiqueWhite);
	saturn->SetModelScale(0.055f);
	saturn->SetModelOffset({ 11.5f, 0.f, 0.f });
	saturn->MeasureRadius();
	saturn->SetSpeedModifier(0.0216f);
	m_models.emplace_back(std::move(saturn));

	auto uranus = std::make_unique<OrbitModelClock>();
	uranus->SetIndexCount(sphereIndexCount);
	uranus->SetBufferIndices(m_shaderProgram.get());
	uranus->SetModelColour(DirectX::Colors::LightCyan);
	uranus->SetModelScale(0.04f);
	uranus->SetModelOffset({ 18.5f, 0.f, 0.f });
	uranus->MeasureRadius();
	uranus->SetSpeedModifier(0.0152f);
	m_models.emplace_back(std::move(uranus));

	auto neptune = std::make_unique<OrbitModelClock>();
	neptune->SetIndexCount(sphereIndexCount);
	neptune->SetBufferIndices(m_shaderProgram.get());
	neptune->SetModelColour(DirectX::Colors::DeepSkyBlue);
	neptune->SetModelScale(0.038f);
	neptune->SetModelOffset({ 21.5f, 0.f, 0.f });
	neptune->MeasureRadius();
	neptune->SetSpeedModifier(0.0121f);
	m_models.emplace_back(std::move(neptune));

	auto pluto = std::make_unique<OrbitModelClock>();
	pluto->SetIndexCount(sphereIndexCount);
	pluto->SetBufferIndices(m_shaderProgram.get());
	pluto->SetModelColour(DirectX::Colors::SandyBrown);
	pluto->SetModelScale(0.038f * 0.9f);
	pluto->SetModelOffset({ 28.5f, 0.f, 0.f });
	pluto->MeasureRadius();
	pluto->SetSpeedModifier(0.0119f);
	m_models.emplace_back(std::move(pluto));


	m_vertexArray = std::make_unique<VertexArray>();
	m_vertexArray->AddVertexBuffer(sphereInput.GetVertexData())
		.AddIndexBuffer(sphereInput.GetIndexData())
		.AddVertexAttribute();
}

Engine::~Engine() noexcept {
	glfwTerminate();
}

void Engine::Run() {
	float accumulatedElaspedTime = 0.f;

	while (!glfwWindowShouldClose(m_windowHandle)) {
		m_frameTime.StartTimer();

		processInput(m_windowHandle);

		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shaderProgram->BindShaderProgram();
		m_vertexArray->BindVertexArray();

		BindCameraBuffer();

		const float deltaTime = m_frameTime.GetDeltaTime();
		const float updateDelta = m_frameTime.GetGraphicsUpdateDelta();
		if (accumulatedElaspedTime >= updateDelta) {
			while (accumulatedElaspedTime >= updateDelta) {
				for (auto& model : m_models)
					model->PhysicsUpdate();

				accumulatedElaspedTime -= updateDelta;
			}

			accumulatedElaspedTime = 0.f;
		}
		else
			accumulatedElaspedTime += deltaTime;

		for (auto& model : m_models)
			model->Draw();

		glfwSwapBuffers(m_windowHandle);
		glfwPollEvents();


		m_frameTime.EndTimer();
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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Engine::processInput(GLFWwindow* window) noexcept {
	static Camera camera1{};
	camera1.Inputs(m_windowHandle);
	static bool tabDown = false;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		tabDown = true;
		if (tabDown) {
			std::cout << "tab pressed\n";
			m_cameraTopDown = !m_cameraTopDown;
			m_camera3D = !m_cameraTopDown;
		}
		if (m_cameraTopDown || !m_camera3D) {
			std::cout << "here\n";
			camera1.Set3DPersonView();
			camera1.Inputs(m_windowHandle);

		}
		else if (m_camera3D || !m_cameraTopDown) {
			std::cout << "there\n";
			camera1.SetTopDownView();
			camera1.Inputs(m_windowHandle);
		}
		else {
		}
		DirectX::XMStoreFloat4x4(&m_viewMatrix, camera1.GetViewMatrix());
	}

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) {

		std::cout << "keyup\n";
		//DirectX::XMStoreFloat4x4(&m_viewMatrix, camera1.GetViewMatrix());

	}

	DirectX::XMStoreFloat4x4(&m_viewMatrix, camera1.GetViewMatrix());
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
	m_VPMatrixIndex = m_shaderProgram->GetUniformLocation("camMatrix");
}

void Engine::BindCameraBuffer() noexcept {
	glUniformMatrix4fv(m_viewMatrixIndex, 1u, GL_FALSE, m_viewMatrix.m[0]);
	glUniformMatrix4fv(m_projectionMatrixIndex, 1u, GL_FALSE, m_projectionMatrix.m[0]);
}
