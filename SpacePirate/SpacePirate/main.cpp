#include "States/TestState.h"
#include "Env.h"
#include "Event/WindowEventsHolder.h"
#include "Components/CameraScreenScaleComponent.h"
#include "Components/PlayerCommandsComponent.h"
#include "Components/PlayerControlerComponent.h"
#include "Systems/PlayerControlerSystem.h"
#include <NDK/Application.hpp>
#include <Ndk/StateMachine.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <memory>
#include <iostream>

void initializeComponentsAndSystems()
{
	Ndk::InitializeComponent<CameraScreenScaleComponent>("001CSS");
	Ndk::InitializeComponent<PlayerCommandsComponent>("002PCC");
	Ndk::InitializeComponent<PlayerControlerComponent>("003PCC");

	Ndk::InitializeSystem<PlayerControlerSystem>();
}

int main()
{
	Ndk::Application application;

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>();
	mainWindow.Create(Nz::VideoMode(800, 600, 32), "Test");

	Env env;
	env.application(application);
	env.window(mainWindow);

	WindowEventsHolder windowEventsHolder(mainWindow.GetEventHandler());

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