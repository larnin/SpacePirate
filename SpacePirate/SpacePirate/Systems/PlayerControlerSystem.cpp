#include "PlayerControlerSystem.h"
#include "Components/PlayerCommandsComponent.h"
#include "Components/PlayerControlerComponent.h"
#include <NDK/Components/NodeComponent.hpp>

Ndk::SystemIndex PlayerControlerSystem::systemIndex;

PlayerControlerSystem::PlayerControlerSystem()
{
	Requires<Ndk::NodeComponent, PlayerCommandsComponent, PlayerControlerComponent>();
}

void PlayerControlerSystem::OnUpdate(float elapsedTime)
{
	for (const Ndk::EntityHandle& entity : GetEntities())
	{
		/*auto controles = entity->GetComponent<PlayerCommandsComponent>().evaluateControles();
		auto & controler = entity->GetComponent<PlayerControlerComponent>();
		controler.update(controles, elapsedTime);*/
	}
}
