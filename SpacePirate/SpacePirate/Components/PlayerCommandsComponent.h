#pragma once

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector2.hpp>
#include "Event/Event.h"
#include "Event/WindowEventArgs.h"

struct PlayerCommands
{
	Nz::Vector2f direction = Nz::Vector2f::Zero();
	bool jumping = false;
	bool startJump = false;
	bool dashing = false;
};

class PlayerCommandsComponent : public Ndk::Component<PlayerCommandsComponent>
{
public:
	PlayerCommandsComponent();
	PlayerCommandsComponent(const PlayerCommandsComponent & c);
	PlayerCommandsComponent(PlayerCommandsComponent && c);

	PlayerCommands evaluateControles();

	static Ndk::ComponentIndex componentIndex;

private:
	void onKeyPressed(const KeyPressedEvent & e);

	EventHolder<KeyPressedEvent> m_keyPressedHolder;

	bool m_dashing;
	bool m_jumping;
};