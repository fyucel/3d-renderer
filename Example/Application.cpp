#include <SDL.h>
#include <unordered_set>

#include "Application.h"

InputHandler::InputHandler() :
	keystate{ nullptr },
	mouseLocationX{ 0 },
	mouseLocationY{ 0 },
	previousMouseLocationX{ 0 },
	previousMouseLocationY{ 0 },
	lastCameraAdjustment{ 0 } {}

bool InputHandler::HandleInput(IAdjustCamera* adjustCamera)
{
	keystate = SDL_GetKeyboardState(nullptr);
	SDL_GetMouseState(&mouseLocationX, &mouseLocationY);

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
	previousMouseLocationX = mouseLocationX;
	previousMouseLocationY = mouseLocationY;
	return true;
}

void InputHandler::PanCameraWithKeyboard(IAdjustCamera* adjustCamera)
{
	float secondsElapsed = (SDL_GetTicks()
		- (float)lastCameraAdjustment) / 1000.0f;

	bool fastSpeed = keystate[SDL_SCANCODE_LSHIFT]
		|| keystate[SDL_SCANCODE_RSHIFT];

	if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_LEFT])
		adjustCamera->PanForwards(secondsElapsed, fastSpeed);
	if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_RIGHT])
		adjustCamera->PanLeft(secondsElapsed, fastSpeed);
	if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN])
		adjustCamera->PanBackwards(secondsElapsed, fastSpeed);
	if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT])
		adjustCamera->PanRight(secondsElapsed, fastSpeed);

	lastCameraAdjustment = SDL_GetTicks();
}

void InputHandler::MoveCameraWithMouse(IAdjustCamera* adjustCamera)
{
	float secondsElapsed = (SDL_GetTicks()
		- (float)lastCameraAdjustment) / 1000.0f;

	int offsetX = mouseLocationX - previousMouseLocationX;
	int offsetY = previousMouseLocationY - mouseLocationY;
	adjustCamera->Move(offsetX, offsetY, secondsElapsed);
}

void InputHandler::ZoomCameraWithWheel(IAdjustCamera* adjustCamera,
	int wheelDisplacement)
{
	float secondsElapsed = (SDL_GetTicks()
		- (float)lastCameraAdjustment) / 1000.0f;

	bool fastSpeed = keystate[SDL_SCANCODE_LSHIFT]
		|| keystate[SDL_SCANCODE_RSHIFT];

	if (wheelDisplacement < 0)
		adjustCamera->ZoomIn(secondsElapsed, fastSpeed);
	else if (wheelDisplacement > 0)
		adjustCamera->ZoomOut(secondsElapsed, fastSpeed);
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