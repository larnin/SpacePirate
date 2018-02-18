#pragma once

#include <NDK/System.hpp>

class PlayerControlerSystem : public Ndk::System<PlayerControlerSystem>
{
public:
	PlayerControlerSystem();

	static Ndk::SystemIndex systemIndex;

protected:
	virtual void OnUpdate(float elapsedTime) override;
};