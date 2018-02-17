#include "States/TestState.h"
#include "Env.h"
#include <NDK/Application.hpp>
#include <Ndk/StateMachine.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <memory>
#include <iostream>

void initializeComponentsAndSystems()
{

}

int main()
{
	Ndk::Application application;

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>();
	mainWindow.Create(Nz::VideoMode(800, 600, 32), "Test");

	Env env;
	env.application(application);
	env.window(mainWindow);

	initializeComponentsAndSystems();

	Ndk::StateMachine fsm(std::make_shared<TestState>());

	while (application.Run())
	{
		if (!fsm.Update(application.GetUpdateTime()))
			break;

		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}