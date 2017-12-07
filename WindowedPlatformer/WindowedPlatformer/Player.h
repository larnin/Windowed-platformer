#pragma once
#include "Animation2D/Animator2DComponent.h"
#include <Nazara/Math/Vector2.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Entity.hpp>
#include <vector>

class Player
{
	struct RenderInfos
	{
		RenderInfos(Ndk::NodeComponent* _node, Animator2DComponent* _animator, float _scale)
			: node(_node), animator(_animator), scale(_scale) { }

		Ndk::NodeComponent* node;
		Animator2DComponent* animator;
		float scale;
	};

public:
	Player();
	~Player() = default;

	void attachGraphicEntity(Ndk::EntityHandle graphicEntity, unsigned int setLayer, float scale);
	void attachPhysicEntity(Ndk::EntityHandle physicEntity, const Nz::Vector2f & pos);
	void attachPhysWorld(Nz::PhysWorld2D & physWorld);
	void update(float elapsedTime);

private:
	void createCallbacks();
	void setProperty(const std::string & key, int value);
	void checkGround();
	Animator2DRef createAnimator();

	std::vector<RenderInfos> m_renders;
	Ndk::PhysicsComponent2D* m_physics;
	Nz::PhysWorld2D* m_physWorld;
	Animator2DRef m_animator;
	bool m_grounded;
	bool m_jumpPressed;
};

