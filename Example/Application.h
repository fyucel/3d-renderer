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
	void ZoomCameraWithWheel(IAdjustCamera* adjustCamera,
		int wheelDisplacement);

	void FetchKeystate();
	void FetchMouseState();
	void SecondsSinceLastFrame();
};

class Entity : public IEntityRenderInfo
{
public:
	Entity(int meshType, int textureType) :
		meshType{ meshType },
		textureType{ textureType },
		positionX{ 0.0f },
		positionY{ 0.0f },
		positionZ{ 0.0f },
		rotationX{ 0.0f },
		rotationY{ 0.0f },
		rotationZ{ 0.0f },
		scaleX{ 1.0f },
		scaleY{ 1.0f },
		scaleZ{ 1.0f } {}

	int MeshType() const override { return meshType; }
	int TextureType() const override { return textureType; }

	float PositionX() const override { return positionX; }
	float PositionY() const override { return positionY; }
	float PositionZ() const override { return positionZ; }

	float RotationX() const override { return rotationX; }
	float RotationY() const override { return rotationY; }
	float RotationZ() const override { return rotationZ; }

	float ScaleX() const override { return scaleX; }
	float ScaleY() const override { return scaleY; }
	float ScaleZ() const override { return scaleZ; }

	void Position(float x, float y, float z)
	{
		positionX = x;
		positionY = y;
		positionZ = z;
	}

	void Rotation(float x, float y, float z)
	{
		rotationX = x;
		rotationY = y;
		rotationZ = z;
	}

	void Scale(float x, float y, float z)
	{
		scaleX = x;
		scaleY = y;
		scaleZ = z;
	}

private:
	int meshType, textureType;
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
};

