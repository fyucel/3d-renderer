#pragma once

#include <chrono>
#include <memory>
#include <SDL.h>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include "RendererAPI.h"

class InputHandler
{
public:
	InputHandler() :
		keystate{ nullptr },
		mouseLocationX{ 0 },
		mouseLocationY{ 0 },
		previousMouseLocationX{ 0 },
		previousMouseLocationY{ 0 },
		secondsSinceLastFrame{ 0.0f },
		lastFrame{ 0 } {}

	// Polls for mouse and keyboard input to adjust the camera
	bool HandleInput(IAdjustCamera* adjustCamera)
	{
		FetchKeystate();
		FetchMouseState();
		SecondsSinceLastFrame();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return false;

			if (event.button.button == SDL_BUTTON_MIDDLE
				&& event.type == SDL_MOUSEMOTION)
				MoveCameraWithMouse(adjustCamera);

			if (event.type == SDL_MOUSEWHEEL)
				ZoomCameraWithMouseWheel(adjustCamera, event.wheel.y);
		}

		PanCameraWithKeyboard(adjustCamera);

		lastFrame = SDL_GetTicks();
		return true;
	}

private:
	const unsigned char* keystate;

	int mouseLocationX, mouseLocationY; // pixels
	int previousMouseLocationX, previousMouseLocationY; // pixels

	float secondsSinceLastFrame; // seconds
	int lastFrame; // milliseconds

	void PanCameraWithKeyboard(IAdjustCamera* adjustCamera)
	{
		bool fastSpeed = keystate[SDL_SCANCODE_LSHIFT]
			|| keystate[SDL_SCANCODE_RSHIFT];

		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_LEFT])
			adjustCamera->PanForwards(secondsSinceLastFrame, fastSpeed);
		if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_RIGHT])
			adjustCamera->PanLeft(secondsSinceLastFrame, fastSpeed);
		if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN])
			adjustCamera->PanBackwards(secondsSinceLastFrame, fastSpeed);
		if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT])
			adjustCamera->PanRight(secondsSinceLastFrame, fastSpeed);
	}

	void MoveCameraWithMouse(IAdjustCamera* adjustCamera)
	{
		int offsetX = mouseLocationX - previousMouseLocationX;
		int offsetY = previousMouseLocationY - mouseLocationY;
		adjustCamera->Move(offsetX, offsetY, secondsSinceLastFrame);
	}

	void ZoomCameraWithMouseWheel(IAdjustCamera* adjustCamera,
		int wheelDisplacement)
	{
		bool fastSpeed = keystate[SDL_SCANCODE_LSHIFT]
			|| keystate[SDL_SCANCODE_RSHIFT];

		if (wheelDisplacement < 0)
			adjustCamera->ZoomIn(secondsSinceLastFrame, fastSpeed);
		else if (wheelDisplacement > 0)
			adjustCamera->ZoomOut(secondsSinceLastFrame, fastSpeed);
	}

	void FetchKeystate()
	{
		keystate = SDL_GetKeyboardState(nullptr);
	}

	void FetchMouseState()
	{
		previousMouseLocationX = mouseLocationX;
		previousMouseLocationY = mouseLocationY;
		SDL_GetMouseState(&mouseLocationX, &mouseLocationY);
	}

	void SecondsSinceLastFrame()
	{
		secondsSinceLastFrame = (SDL_GetTicks()
			- (float)lastFrame) / 1000.0f;
	}
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
	Entity(MeshEnum meshType, TextureEnum textureType) :
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

	int MeshType() const override { return (int)meshType; }
	int TextureType() const override { return (int)textureType; }

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
	MeshEnum meshType;
	TextureEnum textureType;
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	float scaleX, scaleY, scaleZ;
};

class EntityContainer : public IAccessEntityRenderInfo
{
public:
	EntityContainer()
	{
		auto entity = std::make_unique<Entity>(
			MeshEnum::Example, TextureEnum::Example);
		entityRenderInfo.insert(entity.get());
		entities[0] = std::move(entity);
	}

	const std::unordered_set<IEntityRenderInfo*> EntitiesToRender() override
	{
		return entityRenderInfo;
	}

private:
	std::unordered_map<int, std::unique_ptr<Entity>> entities;
	std::unordered_set<IEntityRenderInfo*> entityRenderInfo;
};

class Application
{
public:
	Application() :
		running{ false },
		renderer{ InitializeRenderer() }
	{
		LoadRendererAssets();
	}

	void Run()
	{
		running = true;
		while (running)
		{
			inputHandler.HandleInput(renderer->AdjustCamera());
			renderer->Render(&entityContainer);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

private:
	bool running;

	EntityContainer entityContainer;
	InputHandler inputHandler;
	std::unique_ptr<IRenderer> renderer;

	void LoadRendererAssets()
	{
		renderer->LoadMesh((int)MeshEnum::Example,
			"../Assets/Objects/Example.obj");
		renderer->LoadTexture((int)TextureEnum::Example,
			"../Assets/Textures/Example.png");
	}
};

#undef main
int main(int argc, char** argv)
{
	Application application;
	application.Run();
	return 0;
}
