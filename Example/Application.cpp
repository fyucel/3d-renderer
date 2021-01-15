#include <SDL.h>
#include <thread>

#include "Application.h"

InputHandler::InputHandler() :
	keystate{ nullptr },
	mouseLocationX{ 0 },
	mouseLocationY{ 0 },
	previousMouseLocationX{ 0 },
	previousMouseLocationY{ 0 },
	secondsSinceLastFrame{ 0.0f },
	lastFrame{ 0 } {}

bool InputHandler::HandleInput(IAdjustCamera* adjustCamera)
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

void InputHandler::PanCameraWithKeyboard(IAdjustCamera* adjustCamera)
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

void InputHandler::MoveCameraWithMouse(IAdjustCamera* adjustCamera)
{
	int offsetX = mouseLocationX - previousMouseLocationX;
	int offsetY = previousMouseLocationY - mouseLocationY;
	adjustCamera->Move(offsetX, offsetY, secondsSinceLastFrame);
}

void InputHandler::ZoomCameraWithMouseWheel(IAdjustCamera* adjustCamera,
	int wheelDisplacement)
{
	bool fastSpeed = keystate[SDL_SCANCODE_LSHIFT]
		|| keystate[SDL_SCANCODE_RSHIFT];

	if (wheelDisplacement < 0)
		adjustCamera->ZoomIn(secondsSinceLastFrame, fastSpeed);
	else if (wheelDisplacement > 0)
		adjustCamera->ZoomOut(secondsSinceLastFrame, fastSpeed);
}

void InputHandler::FetchKeystate()
{
	keystate = SDL_GetKeyboardState(nullptr);
}

void InputHandler::FetchMouseState()
{
	previousMouseLocationX = mouseLocationX;
	previousMouseLocationY = mouseLocationY;
	SDL_GetMouseState(&mouseLocationX, &mouseLocationY);
}

void InputHandler::SecondsSinceLastFrame()
{
	secondsSinceLastFrame = (SDL_GetTicks()
		- (float)lastFrame) / 1000.0f;
}

Entity::Entity(MeshEnum meshType, TextureEnum textureType) :
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

int Entity::MeshType() const
{
	return (int)meshType;
}

int Entity::TextureType() const
{
	return (int)textureType;
}

float Entity::PositionX() const
{
	return positionX;
}

float Entity::PositionY() const
{
	return positionY;
}

float Entity::PositionZ() const
{
	return positionZ;
}

float Entity::RotationX() const
{
	return rotationX;
}

float Entity::RotationY() const
{
	return rotationY;
}

float Entity::RotationZ() const
{
	return rotationZ;
}

float Entity::ScaleX() const
{
	return scaleX;
}

float Entity::ScaleY() const
{
	return scaleY;
}

float Entity::ScaleZ() const
{
	return scaleZ;
}

void Entity::Position(float x, float y, float z)
{
	positionX = x;
	positionY = y;
	positionZ = z;
}

void Entity::Rotation(float x, float y, float z)
{
	rotationX = x;
	rotationY = y;
	rotationZ = z;
}

void Entity::Scale(float x, float y, float z)
{
	scaleX = x;
	scaleY = y;
	scaleZ = z;
}

EntityContainer::EntityContainer()
{
	auto entity = std::make_unique<Entity>(
		MeshEnum::Example, TextureEnum::Example);
	entityRenderInfo.insert(entity.get());
	entities[0] = std::move(entity);
}

const std::unordered_set<IEntityRenderInfo*> EntityContainer::EntitiesToRender()
{
	return entityRenderInfo;
}

Application::Application() :
	running{ false },
	renderer{ InitializeRenderer() }
{
	LoadRendererAssets();
}

void Application::Run()
{
	running = true;
	while (running)
	{
		inputHandler.HandleInput(renderer->AdjustCamera());
		renderer->Render(&entityContainer);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void Application::LoadRendererAssets()
{
	renderer->LoadMesh((int)MeshEnum::Example,
		"../Assets/Objects/Example.obj");
	renderer->LoadTexture((int)TextureEnum::Example,
		"../Assets/Textures/Example.png");
}

#undef main
int main(int argc, char** argv)
{
	Application application;
	application.Run();
	return 0;
}