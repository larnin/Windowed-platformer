#include "WindowData.h"
#include <Nazara/Graphics/Sprite.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <iostream>


WindowData::WindowData(Ndk::Application & app, const Nz::Recti & geometry, Ndk::CameraComponent & camera, unsigned int setIndex)
	: m_window(&app.AddWindow<Nz::RenderWindow>(Nz::VideoMode(geometry.width, geometry.height, 32), "", Nz::WindowStyle_Threaded))
{
	NazaraAssert(camera.GetEntity()->HasComponent<Ndk::NodeComponent>(), "The camera don't have a node component !");
	m_camera = &camera.GetEntity()->GetComponent<Ndk::NodeComponent>();

	m_window->SetPosition(geometry.x, geometry.y);
	camera.SetTarget(m_window);
	camera.SetProjectionType(Nz::ProjectionType_Orthogonal);
	camera.SetLayer(1);

	auto cameraEntity = m_world.CreateEntity();
	auto& cameraComponent = cameraEntity->AddComponent<Ndk::CameraComponent>();
	cameraComponent.SetTarget(m_window);
	cameraComponent.SetLayer(0);
	cameraComponent.SetProjectionType(Nz::ProjectionType_Orthogonal);
	m_currentCamera = &cameraEntity->AddComponent<Ndk::NodeComponent>();

	m_backgroundNode = createBackground("Res/Backgrounds/Set" + std::to_string(setIndex) + "Background.png", 0);
	m_hillsNode = createBackground("Res/Backgrounds/Set" + std::to_string(setIndex) + "Hills.png", 1);
	m_tilesNode = createBackground("Res/Backgrounds/Set" + std::to_string(setIndex) + "Tiles.png", 2);
	updateObjectsPosition();
}

WindowData::~WindowData()
{
	if (m_window != nullptr)
	{
		m_window->Close();
		m_camera->GetEntity()->Kill();
	}
}

WindowData::WindowData(WindowData && w)
	: m_window(w.m_window)
	, m_camera(w.m_camera)
	, m_currentCamera(w.m_currentCamera)
	, m_world(std::move(w.m_world))
	, m_backgroundNode(w.m_backgroundNode)
	, m_hillsNode(w.m_hillsNode)
	, m_tilesNode(w.m_tilesNode)
{
	w.m_window = nullptr;
}

WindowData & WindowData::operator=(WindowData && w)
{
	if (m_window != nullptr)
	{
		m_window->Close();
		m_camera->GetEntity()->Kill();
	}

	m_window = w.m_window;
	m_camera = w.m_camera;
	m_currentCamera = w.m_currentCamera;
	m_world= std::move(w.m_world);
	m_backgroundNode = w.m_backgroundNode;
	m_hillsNode = w.m_hillsNode;
	m_tilesNode = w.m_tilesNode;

	w.m_window = nullptr;

	return *this;
}

void WindowData::move(const Nz::Vector2i & offset)
{
	m_window->SetPosition(m_window->GetPosition() + offset);
	updateObjectsPosition();
}

void WindowData::update(float elapsedTime)
{
	m_world.Update(elapsedTime);
	m_window->Display();
}

Ndk::NodeComponent * WindowData::createBackground(const std::string & textureName, float height)
{
	auto entity = m_world.CreateEntity();

	auto& graphic = entity->AddComponent<Ndk::GraphicsComponent>();
	auto sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
	sprite->SetTexture(textureName);
	graphic.Attach(sprite);

	return &entity->AddComponent<Ndk::NodeComponent>();
}

void WindowData::updateObjectsPosition()
{
	//todo
}
