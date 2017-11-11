#include "WindowData.h"
#include <NDK/Components/GraphicsComponent.hpp>
#include <iostream>

unsigned int WindowData::m_windowCount = 0;

WindowData::WindowData(Ndk::Application & app, const Nz::Recti & geometry, Ndk::CameraComponent & camera, unsigned int setIndex, const Nz::Recti & screenRect)
	: m_window(&app.AddWindow<Nz::RenderWindow>(Nz::VideoMode(geometry.width, geometry.height, 32), std::to_string(m_windowCount++), Nz::WindowStyle_Threaded))
	, m_screenRect(screenRect)
{
	m_world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());

	NazaraAssert(camera.GetEntity()->HasComponent<Ndk::NodeComponent>(), "The camera don't have a node component !");
	m_camera = &camera.GetEntity()->GetComponent<Ndk::NodeComponent>();

	m_window->SetPosition(geometry.x, geometry.y);
	m_window->SetFramerateLimit(60);

	camera.SetTarget(m_window);
	camera.SetProjectionType(Nz::ProjectionType_Orthogonal);
	camera.SetLayer(1);

	auto cameraEntity = m_world.CreateEntity();
	auto& cameraComponent = cameraEntity->AddComponent<Ndk::CameraComponent>();
	cameraComponent.SetTarget(m_window);
	cameraComponent.SetLayer(0);
	cameraComponent.SetProjectionType(Nz::ProjectionType_Orthogonal);
	m_currentCamera = &cameraEntity->AddComponent<Ndk::NodeComponent>();
	m_currentCamera->SetPosition(-10.0f*m_currentCamera->GetEntity()->GetComponent<Ndk::CameraComponent>().GetForward());

	m_backgroundSprite = createBackground("Res/Backgrounds/Set" + std::to_string(setIndex) + "Background.png", 2);
	m_hillsSprite = createBackground("Res/Backgrounds/Set" + std::to_string(setIndex) + "Hills.png", 0);
	m_tilesSprite = createBackground("Res/Backgrounds/Set" + std::to_string(setIndex) + "Tiles.png", 1);
	updateObjectsPosition();
}

WindowData::~WindowData()
{
	m_window->Close();
	m_camera->GetEntity()->Kill();
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

Nz::SpriteRef WindowData::createBackground(const std::string & textureName, float height)
{
	auto entity = m_world.CreateEntity();

	auto& graphic = entity->AddComponent<Ndk::GraphicsComponent>();
	auto sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
	sprite->SetTexture(textureName);
	auto size = m_window->GetSize();
	sprite->SetSize(size.x, size.y);
	graphic.Attach(sprite);

	auto& node = entity->AddComponent<Ndk::NodeComponent>();
	node.SetPosition(Nz::Vector3f(0, 0, height));
	return sprite;
}

void WindowData::updateObjectsPosition()
{
	auto geometry = getGeometry();

	auto hillsTextureSize = m_hillsSprite->GetMaterial()->GetDiffuseMap()->GetSize();
	m_hillsSprite->SetTextureRect(Nz::Rectui(geometry.x/1.5f, hillsTextureSize.y - geometry.height, geometry.width, geometry.height));

	auto tileTextureSize = m_tilesSprite->GetMaterial()->GetDiffuseMap()->GetSize();

	int y = (geometry.y + geometry.height / 2 - (m_screenRect.y + m_screenRect.height / 2)) / 2;
	y += (tileTextureSize.y - geometry.height) / 2;
	while (y < 0)
		y += tileTextureSize.y;

	m_tilesSprite->SetTextureRect(Nz::Rectui(geometry.x/2, y, geometry.width, geometry.height));
}
