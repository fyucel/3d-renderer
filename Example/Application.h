#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "RendererAPI.h"

class InputHandler
{
public:
	InputHandler();

	// Polls for mouse and keyboard input to adjust the camera
	bool HandleInput(IAdjustCamera* adjustCamera);

private:
	const unsigned char* keystate;

	int mouseLocationX, mouseLocationY; // pixels
	int previousMouseLocationX, previousMouseLocationY; // pixels

	float secondsSinceLastFrame; // seconds
	int lastFrame; // milliseconds

	void PanCameraWithKeyboard(IAdjustCamera* adjustCamera);
	void MoveCameraWithMouse(IAdjustCamera* adjustCamera);
	void ZoomCameraWithMouseWheel(IAdjustCamera* adjustCamera,
		int wheelDisplacement);

	void FetchKeystate();
	void FetchMouseState();
	void SecondsSinceLastFrame();
};

enum class MeshEnum
{
	Example,
};

enum class TextureEnum
{
	Example,
};

class Entity : public IEntityRenderInfo
{
public:
	Entity(MeshEnum meshType, TextureEnum textureType);

	int MeshType() const override;
	int TextureType() const override;

	float PositionX() const override;
	float PositionY() const override;
	float PositionZ() const override;

	float RotationX() const override;
	float RotationY() const override;
	float RotationZ() const override;

	float ScaleX() const override;
	float ScaleY() const override;
	float ScaleZ() const override;

	void Position(float x, float y, float z);
	void Rotation(float x, float y, float z);
	void Scale(float x, float y, float z);

private:
	MeshEnum meshType;
	TextureEnum textureType;
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	float scaleX, scaleY, scaleZ;
};

class EntityContainer : public IAccessEntityRenderInfo
{
public:
	EntityContainer();

	const std::unordered_set<IEntityRenderInfo*> EntitiesToRender() override;

private:
	std::unordered_map<int, std::unique_ptr<Entity>> entities;
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

	void LoadRendererAssets();
};

