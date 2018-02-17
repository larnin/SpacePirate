
#include <NDK/StateMachine.hpp>
#include "TestState.h"

TestState::TestState()
{

}

void TestState::Enter(Ndk::StateMachine & fsm)
{
}

void TestState::Leave(Ndk::StateMachine & fsm)
{
}

bool TestState::Update(Ndk::StateMachine & fsm, float elapsedTime)
{
	return false;
}
