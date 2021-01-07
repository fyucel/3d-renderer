#include <SDL.h>

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
			ZoomCameraWithWheel(adjustCamera, event.wheel.y);
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

void InputHandler::ZoomCameraWithWheel(IAdjustCamera* adjustCamera,
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

EntityContainer::EntityContainer()
{

}

const std::unordered_set<IEntityRenderInfo*> EntityContainer::EntitiesToRender()
{
	return entityRenderInfo;
}

Application::Application() :
	running{ false },
	renderer{ InitializeRenderer() } {}

void Application::Run()
{
	running = true;

	while (running)
	{
		inputHandler.HandleInput(renderer->AdjustCamera());
		renderer->Render(&entityContainer);
	}
}

#undef main
int main(int argc, char** argv)
{
	Application application;
	application.Run();
	return 0;
}