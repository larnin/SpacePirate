#pragma once

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector2.hpp>

struct PlayerCommands
{
	Nz::Vector2f direction = Nz::Vector2f::Zero();
	bool jumping = false;
	bool dashing = false;
};

class PlayerCommandsComponent : public Ndk::Component<PlayerCommandsComponent>
{
public:
	PlayerCommandsComponent() = default;

	PlayerCommands evaluateControles();

	static Ndk::ComponentIndex componentIndex;
};