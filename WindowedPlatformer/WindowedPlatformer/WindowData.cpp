#include "WindowData.h"
#include <NDK/Components/GraphicsComponent.hpp>
#include <iostream>

unsigned int WindowData::m_windowCount = 0;

WindowData::WindowData(Ndk::Application & app, const Nz::Recti & geometry, unsigned int setIndex, const Nz::Recti & screenRect)
	: m_window(&app.AddWindow<Nz::RenderWindow>(Nz::VideoMode(geometry.width, geometry.height, 32), std::to_string(m_windowCount++), Nz::WindowStyle_Threaded))
	, m_screenRect(screenRect)
	, m_setIndex(setIndex)
	, m_zoom(calculateZoom())
{
	m_worldBack.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
	m_worldGame.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
	m_worldGame.GetSystem<Ndk::RenderSystem>().SetDefaultBackground(nullptr);

	m_window->SetPosition(geometry.x, geometry.y);
	m_window->SetFramerateLimit(60);
	m_window->EnableCloseOnQuit(false);

	auto cameraEntity = m_worldBack.CreateEntity();
	auto& cameraComponent = cameraEntity->AddComponent<Ndk::CameraComponent>();
	cameraComponent.SetTarget(m_window);
	cameraComponent.SetLayer(0);
	cameraComponent.SetProjectionType(Nz::ProjectionType_Orthogonal);
	auto & cameraNode = cameraEntity->AddComponent<Ndk::NodeComponent>();
	cameraNode.SetPosition(-10.0f*cameraComponent.GetForward());

	auto cameraEntity2 = m_worldGame.CreateEntity();
	auto& cameraComponent2 = cameraEntity2->AddComponent<Ndk::CameraComponent>();
	cameraComponent2.SetTarget(m_window);
	cameraComponent2.SetLayer(1);
	cameraComponent2.SetProjectionType(Nz::ProjectionType_Orthogonal);
	m_cameraGame = &cameraEntity2->AddComponent<Ndk::NodeComponent>();

	m_backgroundSprite = createBackground("Res/Img/Set" + std::to_string(setIndex) + "/Background.png", 2);
	m_hillsSprite = createBackground("Res/Img/Set" + std::to_string(setIndex) + "/BackHills.png", 0);
	m_tilesSprite = createBackground("Res/Img/Set" + std::to_string(setIndex) + "/BackTiles.png", 1);
	createBorder();
	updateObjectsPosition();
}

WindowData::~WindowData()
{
	m_window->Close();
}

void WindowData::move(const Nz::Vector2i & offset)
{
	m_window->SetPosition(m_window->GetPosition() + offset);
	updateObjectsPosition();
	updateCamera();
}

void WindowData::update(float elapsedTime)
{
	m_worldBack.Update(elapsedTime);
	m_worldGame.Update(elapsedTime);
	m_worldBorder.Update(elapsedTime);
	m_window->Display();
}

Ndk::EntityHandle WindowData::createEntity()
{
	return m_worldGame.CreateEntity();
}

Nz::SpriteRef WindowData::createBackground(const std::string & textureName, float height)
{
	auto entity = m_worldBack.CreateEntity();

	auto sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
	sprite->SetTexture(textureName);
	auto size = m_window->GetSize();
	sprite->SetSize(size.x, size.y);
	auto& graphic = entity->AddComponent<Ndk::GraphicsComponent>();
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

void WindowData::createBorder()
{
	auto & renderSystem = m_worldBorder.GetSystem<Ndk::RenderSystem>();
	renderSystem.SetGlobalUp(Nz::Vector3f::Down());
	renderSystem.SetDefaultBackground(nullptr);

	auto geometry = getGeometry();
	auto size = std::max(geometry.width, geometry.height);

	auto cameraEntity = m_worldBorder.CreateEntity();
	auto & cameraComp = cameraEntity->AddComponent<Ndk::CameraComponent>();
	cameraComp.SetTarget(m_window);
	cameraComp.SetProjectionType(Nz::ProjectionType_Orthogonal);
	cameraComp.SetLayer(2);
	auto & cameraNode = cameraEntity->AddComponent<Ndk::NodeComponent>();
	cameraNode.SetPosition(-10.0f * cameraComp.GetForward());

	auto sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
	sprite->SetTexture("Res/Img/Border.png");
	sprite->SetSize(size, sprite->GetMaterial()->GetDiffuseMap()->GetHeight());

	auto upEntity = m_worldBorder.CreateEntity();
	auto & upGraphic = upEntity->AddComponent<Ndk::GraphicsComponent>();
	upGraphic.Attach(sprite);
	auto & upNode = upEntity->AddComponent<Ndk::NodeComponent>();
	upNode.SetPosition(Nz::Vector3f(0, 0, 0));

	auto downEntity = m_worldBorder.CreateEntity();
	auto & downGraphic = downEntity->AddComponent<Ndk::GraphicsComponent>();
	downGraphic.Attach(sprite);
	auto & downNode = downEntity->AddComponent<Ndk::NodeComponent>();
	downNode.SetPosition(Nz::Vector3f(geometry.width, geometry.height, 0));
	downNode.SetRotation(Nz::Quaternionf(Nz::EulerAnglesf(0, 0, 180)));

	auto leftEntity = m_worldBorder.CreateEntity();
	auto & leftGraphic = leftEntity->AddComponent<Ndk::GraphicsComponent>();
	leftGraphic.Attach(sprite);
	auto & leftNode = leftEntity->AddComponent<Ndk::NodeComponent>();
	leftNode.SetPosition(Nz::Vector3f(0, geometry.height, 0));
	leftNode.SetRotation(Nz::Quaternionf(Nz::EulerAnglesf(0, 0, -90)));

	auto rightEntity = m_worldBorder.CreateEntity();
	auto & rightGraphic = rightEntity->AddComponent<Ndk::GraphicsComponent>();
	rightGraphic.Attach(sprite);
	auto & rightNode = rightEntity->AddComponent<Ndk::NodeComponent>();
	rightNode.SetPosition(Nz::Vector3f(geometry.width, 0, 0));
	rightNode.SetRotation(Nz::Quaternionf(Nz::EulerAnglesf(0, 0, 90)));
}

void WindowData::updateCamera()
{
	auto pos = m_window->GetPosition();
	pos -= Nz::Vector2i(m_screenRect.GetPosition());
	pos.x *= m_zoom;
	pos.y *= m_zoom;
	m_cameraGame->SetPosition(Nz::Vector2f(pos));
}

float WindowData::calculateZoom()
{
	auto mode = Nz::VideoMode::GetDesktopMode();
	return std::min(float(m_screenRect.width) / mode.width, float(m_screenRect.height) / mode.height);
}
