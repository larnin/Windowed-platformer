#include "GameState.h"
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <NDK/Systems/RenderSystem.hpp>
#include <NDK/Systems/PhysicsSystem2D.hpp>

#include <iostream>

const float globalRatio = 16.0f / 9.0f;
const int defaultWidth = 1920;
const int defaultHeight = defaultWidth / globalRatio;

GameState::GameState(Ndk::Application & app)
	: m_app(app)
	, m_screenRect(calculateScreenRect(globalRatio))
	, m_windowManager(app, m_screenRect)
	, m_tilemap(32, 18)
{
	auto & physWorld = m_world.GetSystem<Ndk::PhysicsSystem2D>().GetWorld();
	physWorld.SetGravity(Nz::Vector2f(0, 5));

	/*Nz::PhysWorld2D::Callback playerGroundCallback;
	playerGroundCallback.startCallback = [](Nz::PhysWorld2D& world, Nz::RigidBody2D& bodyA, Nz::RigidBody2D& bodyB, void*)
	{
		std::cout << "Start" << std::endl;
		return true;
	};

	playerGroundCallback.endCallback = [](Nz::PhysWorld2D& world, Nz::RigidBody2D& bodyA, Nz::RigidBody2D& bodyB, void*)
	{
		std::cout << "end" << std::endl;
	};

	playerGroundCallback.preSolveCallback = [](Nz::PhysWorld2D& world, Nz::RigidBody2D& bodyA, Nz::RigidBody2D& bodyB, void*)
	{
		std::cout << "pre" << std::endl;
		return true;
	};

	playerGroundCallback.postSolveCallback = [](Nz::PhysWorld2D& world, Nz::RigidBody2D& bodyA, Nz::RigidBody2D& bodyB, void*)
	{
		std::cout << "post" << std::endl;
	};

	physWorld.RegisterCallbacks((unsigned int)(ColliderID::PLAYER), (unsigned int)(ColliderID::GROUND), playerGroundCallback);*/


	m_tilemap.setBackTile(7, 9, 1);
	//m_tilemap.setBackTile(8, 8, 1);
	m_tilemap.setBackTile(9, 7, 1);
	m_tilemap.setBackTile(10, 6, 1);
	m_tilemap.setBackTile(11, 5, 1);
	m_tilemap.setBackTile(12, 4, 1);

	unsigned int y = 10;
	for (int i(1); i < 25; i++)
	{
		m_tilemap.setBackTile(i, y, 1);
		m_tilemap.setBackTile(i, y+1, 1);
	}
	/*m_tilemap.setBackTile(1, y, 2);
	for (unsigned int i(2); i < 25; i++)
		m_tilemap.setBackTile(i, y, 3);
	m_tilemap.setBackTile(25, y, 4);
	for (unsigned int i(1); i < 26; i++)
		m_tilemap.setBackTile(i, y+1, 1);
	m_tilemap.setBackTile(10, y+1, 10);
	m_tilemap.setBackTile(15, y+1, 19);*/

	auto entity = m_world.CreateEntity();
	entity->AddComponent<Ndk::NodeComponent>();
	m_tilemap.attachColliders(entity->AddComponent<Ndk::CollisionComponent2D>());
	m_player.attachPhysicEntity(m_world.CreateEntity(), Nz::Vector2f(5, 1));
	m_player.createCallbacks(physWorld);
}

void GameState::Enter(Ndk::StateMachine & fsm)
{
	addWindow(Nz::Recti(0, 0, 400, 400), 1, 1);

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
	m_player.update(elapsedTime);
	return m_windowManager.windowCount() > 0;
}

void GameState::addWindow(const Nz::Recti & localGeometry, unsigned int setIndex, unsigned int layer)
{
	auto& w = m_windowManager.addWindow(localToGlobalGeometry(localGeometry), setIndex, layer);

	auto tilemapEntity = w.createEntity();
	tilemapEntity->AddComponent<Ndk::NodeComponent>();
	auto & tilemapGraphic = tilemapEntity->AddComponent<Ndk::GraphicsComponent>();
	auto frontTilemap = createTileMap(setIndex);
	auto backTilemap = createTileMap(setIndex);
	tilemapGraphic.Attach(frontTilemap, 1);
	tilemapGraphic.Attach(backTilemap, 0);
	m_tilemap.attachBackTilemap(backTilemap);
	m_tilemap.attachFrontTilemap(frontTilemap);

	m_player.attachGraphicEntity(w.createEntity(), setIndex, float(m_screenRect.width) / m_tilemap.width());
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

Nz::TileMapRef GameState::createTileMap(unsigned int setIndex)
{
	auto tilemap = Nz::TileMap::New(Nz::Vector2ui(m_tilemap.width(), m_tilemap.height()), Nz::Vector2f(float(m_screenRect.width) / m_tilemap.width(), float(m_screenRect.height) / m_tilemap.height()));
	auto mat = Nz::Material::New("Translucent2D");
	mat->SetDiffuseMap("Res/Img/Set" + std::to_string(setIndex) + "/Tileset.png");
	tilemap->SetMaterial(0, mat);
	return tilemap;
}
