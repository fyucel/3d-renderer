#pragma once

#include "Camera.h"
#include "RendererAPI.h"
#include "Assets.h"
#include "Shader.h"

class Renderer : public IRenderer
{
public:
	Renderer();
	~Renderer();

	void Render(IAccessEntityRenderInfo* accessEntityRenderInfo) override;

	IAdjustCamera* AdjustCamera() override;

	void LoadMesh(int meshEnum, const std::string& filename) override;
	void LoadTexture(int textureEnum, const std::string& filename) override;

private:
	int windowWidth, windowHeight; // pixels

	SDL_Window* window;
	SDL_GLContext context;

	Camera camera;
	std::unique_ptr<AssetContainer> assetContainer;
	std::unique_ptr<Shader> coreShader;

	Shader* currentlyBindedShader;

	static const int OpenGLVersionMajor = 4;
	static const int OpenGLVersionMinor = 4;

	static const float StartingWindowSizePercentage;

	void ClearWindow();
	void SwapWindow();

	void BindShader(Shader* shaderToBind);
	void SendCameraUniforms(Shader* shaderToSendUniforms);

	void DrawEntity(Shader* shader, IEntityRenderInfo* entityRenderInfo);
	void DrawEntities(IAccessEntityRenderInfo* accessEntityRenderInfo);

	void AdjustWindowSize();

	void InitializeSDLSubsystems();
	void InitializeWindow();
	void InitializeOpenGLContext();
	void InitializeAssets();
	void DetermineWindowSize();

	Renderer(const Renderer& other) = delete;
	Renderer(Renderer&& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;
	Renderer& operator=(Renderer&& other) = delete;
};

RENDERER_API IRenderer* InitializeRenderer()
{
	return new Renderer;
}