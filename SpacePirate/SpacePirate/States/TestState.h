#pragma once

#include <NDK/State.hpp>
#include <NDK/World.hpp>

class TestState : public Ndk::State
{
public:
	TestState();

	void Enter(Ndk::StateMachine& fsm) override;
	void Leave(Ndk::StateMachine& fsm) override;
	bool Update(Ndk::StateMachine& fsm, float elapsedTime) override;

private:
	Ndk::World m_world;
};