#include <unordered_set>

#include "Application.h"

InputHandler::InputHandler() :
	keystate{ nullptr },
	mouseLocationX{ 0.0f },
	mouseLocationY{ 0.0f },
	previousMouseLocationX{ 0.0f },
	previousMouseLocationY{ 0.0f },
	lastCameraAdjustment{ 0 } {}

void InputHandler::HandleInput(IAdjustCamera* adjustCamera)
{

}

void InputHandler::ScrollCameraWithKeyboard(IAdjustCamera* adjustCamera)
{

}

void InputHandler::MoveCameraWithMouse(IAdjustCamera* adjustCamera)
{

}

void InputHandler::ZoomCameraWithWheel(IAdjustCamera* adjustCamera,
	int wheelDisplacement)
{

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

int main(int argc, char** argv)
{
	Application application;
	application.Run();
}