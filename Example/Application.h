#pragma once

#include "RendererAPI.h"

#include <memory>

class InputHandler
{
public:
	InputHandler();

	// Polls for mouse and keyboard input to adjust the camera
	void HandleInput(IAdjustCamera* adjustCamera);

private:
	const unsigned char* keystate;

	float mouseLocationX, mouseLocationY;
	float previousMouseLocationX, previousMouseLocationY;

	int lastCameraAdjustment; // ms

	void ScrollCameraWithKeyboard(IAdjustCamera* adjustCamera);
	void MoveCameraWithMouse(IAdjustCamera* adjustCamera);
	void ZoomCameraWithWheel(IAdjustCamera* adjustCamera,
		int wheelDisplacement);
};

class EntityContainer : public IAccessEntityRenderInfo
{
public:
	EntityContainer();

	const std::unordered_set<IEntityRenderInfo*> EntitiesToRender() override;

private:
	std::unordered_set<IEntityRenderInfo*> entityRenderInfo;
};

class Application
{
public:
	Application();

	void Run();

private:
	bool running;

	EntityContainer entityContainer;
	InputHandler inputHandler;
	std::unique_ptr<IRenderer> renderer;
};

