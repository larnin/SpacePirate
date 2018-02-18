#pragma once

#include <NDK/Component.hpp>
#include "PlayerCommandsComponent.h"

class PlayerControlerComponent : public Ndk::Component<PlayerControlerComponent>
{
public:
	PlayerControlerComponent() = default;

	void update(const PlayerCommands & commands, float elapsedTime);

	static Ndk::ComponentIndex componentIndex;

private:
};