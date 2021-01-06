#include "Utilities.h"
#include "Renderer.h"

Renderer::Renderer()
{
	InitializeSDLSubsystems();
	InitializeWindow();
	InitializeOpenGLContext();
	InitializeAssets();
}

Renderer::~Renderer()
{
	SDL_GL_DeleteContext(context);

	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_Quit();
}

void Renderer::Render(IAccessEntityRenderInfo* accessEntityRenderInfo)
{
	ClearWindow();

	camera.Update();
	SendCameraUniforms(coreShader.get());

	coreShader->Bind();

	SwapWindow();
}

IAdjustCamera* Renderer::AdjustCamera()
{
	return &camera;
}

void Renderer::ClearWindow()
{
	GL(glClearColor(1, 1, 1, 1));
	GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
		| GL_STENCIL_BUFFER_BIT));
}

void Renderer::SwapWindow()
{
	glFlush();
	SDL_GL_SwapWindow(window);
}

void Renderer::BindShader(Shader* shaderToBind)
{
	if (shaderToBind == currentlyBindedShader) return;
	shaderToBind->Bind();
	currentlyBindedShader = shaderToBind;
}

void Renderer::SendCameraUniforms(Shader* shaderToSendUniforms)
{
	BindShader(shaderToSendUniforms);
	coreShader->UniformMat4fv("viewMatrix", camera.ViewMatrix());
	coreShader->UniformMat4fv("projectionMatrix", camera.ProjectionMatrix());
	coreShader->UniformVec3fv("cameraPosition", camera.Position());
	coreShader->UniformVec3fv("lightSource", camera.LightSource());
}

void Renderer::DrawEntity(Shader* shaderToUse,
	IEntityRenderInfo* entityRenderInfo)
{
	BindShader(shaderToUse);

	assets->GetTexture((TextureEnum)entityRenderInfo->TextureType())->Bind();

	assets->GetMesh((MeshEnum)entityRenderInfo->MeshType())->Position(
		glm::vec3(entityRenderInfo->PositionX(),
			entityRenderInfo->PositionY(),
			entityRenderInfo->PositionZ()));
	assets->GetMesh((MeshEnum)entityRenderInfo->MeshType())->Rotatation(
		glm::vec3(entityRenderInfo->RotationX(),
			entityRenderInfo->RotationY(),
			entityRenderInfo->RotationZ()));
	assets->GetMesh((MeshEnum)entityRenderInfo->MeshType())->Scale(
		glm::vec3(entityRenderInfo->ScaleX(),
			entityRenderInfo->ScaleY(),
			entityRenderInfo->ScaleZ()));

	assets->GetMesh((MeshEnum)entityRenderInfo->MeshType())->Render(
		coreShader.get());

	assets->GetTexture((TextureEnum)entityRenderInfo->TextureType())->Unbind();
}

void Renderer::AdjustWindowSize()
{
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	camera.AdjustProjection(windowWidth, windowHeight);
}

void Renderer::InitializeSDLSubsystems()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << SDL_GetError() << std::endl;
		throw "Failed to initialize SDL subsystem.";
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OpenGLVersionMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OpenGLVersionMinor);
}

void Renderer::InitializeWindow()
{
	DetermineWindowSize();
	window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (!window)
	{
		std::cout << SDL_GetError() << std::endl;
		throw "Failed to initialize window.";
	}
	camera.AdjustProjection(windowWidth, windowHeight);
}

void Renderer::InitializeOpenGLContext()
{
	context = SDL_GL_CreateContext(window);
	if (!context)
	{
		std::cout << SDL_GetError() << std::endl;
		throw "Failed to initialize OpenGL context.";
	}

	if (glewInit() != GLEW_OK)
		throw "Failed to initialize GLEW.";

	GL(glEnable(GL_DEPTH_TEST));
	GL(glFrontFace(GL_CCW));
	GL(glEnable(GL_BLEND));
	GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

void Renderer::InitializeAssets()
{
	coreShader = std::make_unique<Shader>(
		OpenGLVersionMajor, OpenGLVersionMinor,
		"../Assets/Shaders/CoreVertex.glsl",
		"../Assets/Shaders/CoreFragment.glsl");
	assets = std::make_unique<AssetContainer>();

	currentlyBindedShader = nullptr;
}

void Renderer::DetermineWindowSize()
{
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	windowWidth = (int)(DM.w * StartingWindowSizePercentage);
	windowHeight = (int)(DM.h * StartingWindowSizePercentage);
}

const float Renderer::StartingWindowSizePercentage = 0.75;