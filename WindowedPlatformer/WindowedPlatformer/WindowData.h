#pragma once

#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Math/Rect.hpp>
#include <Nazara/Core/Signal.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <NDK/World.hpp>
#include <NDK/Application.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>

#include <iostream>

class WindowData
{
public:
	WindowData(Ndk::Application & app, const Nz::Recti & geometry, Ndk::CameraComponent & camera, unsigned int setIndex, const Nz::Recti & screenRect);
	~WindowData();
	WindowData(const WindowData & ) = delete;
	WindowData & operator=(const WindowData &) = delete;
	WindowData(WindowData &&) = delete;
	WindowData & operator=(WindowData &&) = delete;

	inline Nz::Recti getGeometry() const { return Nz::Recti(m_window->GetPosition(), Nz::Vector2i(m_window->GetSize()) + m_window->GetPosition()); }
	void move(const Nz::Vector2i & offset);
	void update(float elapsedTime);

	inline Nz::EventHandler & eventHandler() { return m_window->GetEventHandler(); }

private:
	Nz::SpriteRef createBackground(const std::string & textureName, float height);
	void updateObjectsPosition();

	Nz::RenderWindow * m_window;
	Ndk::NodeComponent * m_camera;
	Ndk::NodeComponent * m_currentCamera;
	Ndk::World m_world;

	Nz::SpriteRef m_backgroundSprite;
	Nz::SpriteRef m_hillsSprite;
	Nz::SpriteRef m_tilesSprite;
	Nz::Recti m_screenRect;

	static unsigned int m_windowCount;
};

