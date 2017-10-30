#include "WindowData.h"



WindowData::WindowData(const Nz::Recti & m_geometry, Ndk::CameraComponent & camera)
	: m_window(Nz::VideoMode(m_geometry.width, m_geometry.height, 32), "", Nz::WindowStyle_Threaded)
	, m_camera(camera)
{
	m_window.SetPosition(m_geometry.x, m_geometry.y);
	camera.SetTarget(&m_window);


}


WindowData::~WindowData()
{
	m_window.Close();
}

void WindowData::onWindowMouseMoved(const Nz::EventHandler*, const Nz::WindowEvent::MouseMoveEvent & e)
{

}

void WindowData::onWindowMouseButtonPressed(const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent & e)
{

}

void WindowData::updateCameraPosition()
{

}