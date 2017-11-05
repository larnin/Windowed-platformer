#pragma once

#include "WindowManager.h"
#include <NDK/State.hpp>
#include <NDK/Application.hpp>
#include <NDK/World.hpp>

class GameState : public Ndk::State
{
public:
	GameState(Ndk::Application & app);
	~GameState() = default;

	void Enter(Ndk::StateMachine& fsm) override;
	void Leave(Ndk::StateMachine& fsm) override;
	bool Update(Ndk::StateMachine& fsm, float elapsedTime) override;

private:
	void addWindow(const Nz::Recti & localGeometry, unsigned int setIndex, unsigned int layer);
	Nz::Recti localToGlobalGeometry(const Nz::Recti & geometry);
	Nz::Recti calculateScreenRect(float ratio);

	Ndk::Application & m_app;
	Nz::Recti m_screenRect;
	Ndk::World m_world;
	WindowManager m_windowManager;
};

