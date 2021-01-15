#include "Utilities.h"
#include "Renderer.h"

Renderer::Renderer()
{
	if (!InitializeSDLSubsystems()
		|| !InitializeWindow()
		|| !InitializeOpenGLContext()
		|| !InitializeAssets())
	{
		// Exit the application if initialization fails.
		// The initializer function that failed will
		// report its error through a message box.
		exit(EXIT_FAILURE);
	}
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
	DrawEntities(accessEntityRenderInfo);

	SwapWindow();
}

IAdjustCamera* Renderer::AdjustCamera()
{
	return &camera;
}

void Renderer::LoadMesh(int meshEnum, const std::string& filename)
{
	assetContainer->LoadMesh(meshEnum, filename);
}

void Renderer::LoadTexture(int textureEnum, const std::string& filename)
{
	assetContainer->LoadTexture(textureEnum, filename);
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
	shaderToSendUniforms->UniformMat4fv(
		"viewMatrix", camera.ViewMatrix());
	shaderToSendUniforms->UniformMat4fv(
		"projectionMatrix", camera.ProjectionMatrix());
	shaderToSendUniforms->UniformVec3fv(
		"cameraPosition", camera.Position());
	shaderToSendUniforms->UniformVec3fv(
		"lightSource", camera.LightSource());
}

void Renderer::DrawEntity(Shader* shaderToUse,
	IEntityRenderInfo* entityRenderInfo)
{
	BindShader(shaderToUse);

	assetContainer->GetTexture((int)entityRenderInfo->TextureType())->Bind();

	Mesh* mesh = assetContainer->GetMesh((int)entityRenderInfo->MeshType());
	mesh->Position(glm::vec3(entityRenderInfo->PositionX(),
			entityRenderInfo->PositionY(),
			entityRenderInfo->PositionZ()));
	mesh->Rotatation(glm::vec3(entityRenderInfo->RotationX(),
			entityRenderInfo->RotationY(),
			entityRenderInfo->RotationZ()));
	mesh->Scale(glm::vec3(entityRenderInfo->ScaleX(),
			entityRenderInfo->ScaleY(),
			entityRenderInfo->ScaleZ()));
	mesh->Render(coreShader.get());

	assetContainer->GetTexture((int)entityRenderInfo->TextureType())->Unbind();
}

void Renderer::DrawEntities(IAccessEntityRenderInfo* accessEntityRenderInfo)
{
	for (auto& entity : accessEntityRenderInfo->EntitiesToRender())
		DrawEntity(coreShader.get(), entity);
}

void Renderer::AdjustWindowSize()
{
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	camera.AdjustProjection(windowWidth, windowHeight);
}

bool Renderer::InitializeSDLSubsystems()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Failed to initialize SDL subsystems!",
			SDL_GetError(), nullptr);
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OpenGLVersionMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OpenGLVersionMinor);

	return true;
}

bool Renderer::InitializeWindow()
{
	DetermineWindowSize();
	window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (!window)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Failed to initialize window!",
			SDL_GetError(), nullptr);
		return false;
	}
	camera.AdjustProjection(windowWidth, windowHeight);
	return window != nullptr;
}

bool Renderer::InitializeOpenGLContext()
{
	context = SDL_GL_CreateContext(window);
	if (!context)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Failed to initialize OpenGL context!",
			SDL_GetError(), nullptr);
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Failed to initialize GLEW!", "", nullptr);
		return false;
	}

	GL(glEnable(GL_DEPTH_TEST));
	GL(glFrontFace(GL_CCW));
	GL(glEnable(GL_BLEND));
	GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	return true;
}

bool Renderer::InitializeAssets()
{
	try
	{
		coreShader = std::make_unique<Shader>(
			OpenGLVersionMajor, OpenGLVersionMinor,
			"../Assets/Shaders/CoreVertex.glsl",
			"../Assets/Shaders/CoreFragment.glsl");
	}
	catch (...)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			"Failed to initialize shaders!",
			"Ensure CoreVertex.glsl and CoreFragment.glsl\n"
			"exist in the Assets/Shaders directory.", nullptr);
		return false;
	}
	assetContainer = std::make_unique<AssetContainer>();
	currentlyBindedShader = nullptr;

	return coreShader && assetContainer;
}

void Renderer::DetermineWindowSize()
{
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	windowWidth = (int)(DM.w * StartingWindowSizePercentage);
	windowHeight = (int)(DM.h * StartingWindowSizePercentage);
}

const float Renderer::StartingWindowSizePercentage = 0.75;