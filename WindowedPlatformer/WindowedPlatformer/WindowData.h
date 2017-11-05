#pragma once

#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Math/Rect.hpp>
#include <Nazara/Core/Signal.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/World.hpp>
#include <NDK/Application.hpp>

class WindowData
{
public:
	WindowData(Ndk::Application & app, const Nz::Recti & geometry, Ndk::CameraComponent & camera, unsigned int setIndex);
	~WindowData();
	WindowData(WindowData && w);
	WindowData & operator=(WindowData && w);

	inline Nz::Recti getGeometry() const { return Nz::Recti(m_window->GetPosition(), Nz::Vector2i(m_window->GetSize())); }
	void move(const Nz::Vector2i & offset);
	void update(float elapsedTime);

	inline Nz::EventHandler & eventHandler() { return m_window->GetEventHandler(); }

private:
	Ndk::NodeComponent * createBackground(const std::string & textureName, float height);
	void updateObjectsPosition();

	Nz::RenderWindow * m_window;
	Ndk::NodeComponent * m_camera;
	Ndk::NodeComponent * m_currentCamera;
	Ndk::World m_world;

	Ndk::NodeComponent * m_backgroundNode;
	Ndk::NodeComponent * m_hillsNode;
	Ndk::NodeComponent * m_tilesNode;
};

