#include "GameState.h"
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>

const float globalRatio = 16.0f / 9.0f;
const int defaultWidth = 1920;
const int defaultHeight = defaultWidth / globalRatio;


GameState::GameState(Ndk::Application & app)
	: m_app(app)
	, m_screenRect(calculateScreenRect(globalRatio))
	, m_windowManager(app, m_screenRect)
{

}

void GameState::Enter(Ndk::StateMachine & fsm)
{
	addWindow(Nz::Recti(100, 100, 400, 400), 1, 1);

	addWindow(Nz::Recti(100, 600, 400, 400), 2, 1);

	addWindow(Nz::Recti(1000, 100, 400, 400), 3, 1);

	addWindow(Nz::Recti(600, 600, 400, 400), 4, 1);
}

void GameState::Leave(Ndk::StateMachine & fsm)
{

}

bool GameState::Update(Ndk::StateMachine & fsm, float elapsedTime)
{
	m_world.Update(elapsedTime);
	m_windowManager.update(elapsedTime);

	return m_windowManager.windowCount() > 0;
}

void GameState::addWindow(const Nz::Recti & localGeometry, unsigned int setIndex, unsigned int layer)
{
	auto e = m_world.CreateEntity();
	e->AddComponent<Ndk::NodeComponent>();
	m_windowManager.addWindow(localToGlobalGeometry(localGeometry), e->AddComponent<Ndk::CameraComponent>(), setIndex, layer);
}

Nz::Recti GameState::localToGlobalGeometry(const Nz::Recti & geometry)
{
	float ratioW = float(m_screenRect.width) / defaultWidth;
	float ratioH = float(m_screenRect.height) / defaultHeight;
	return Nz::Recti(m_screenRect.x + geometry.x * ratioW, m_screenRect.y + geometry.y * ratioH, geometry.width * ratioW, geometry.height * ratioH);
}

Nz::Recti GameState::calculateScreenRect(float ratio)
{
	auto mode = Nz::VideoMode::GetDesktopMode();
	if (float(mode.width) / mode.height > ratio)
	{
		int w = mode.height * ratio;
		return Nz::Recti((mode.width - w) / 2, 0, w, mode.height);
	}

	int h = mode.width / ratio;
	return Nz::Recti(0, (mode.height - h) / 2, mode.width, h);
}
