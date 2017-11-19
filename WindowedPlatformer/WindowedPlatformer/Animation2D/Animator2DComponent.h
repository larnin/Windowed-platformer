#pragma once

#include "Animation2D/Animator2D.h"
#include "Animation2D/PropertiesHolder.h"
#include <Nazara/Graphics/Sprite.hpp>
#include <NDK/Component.hpp>
#include <vector>

class Animator2DComponent : public Ndk::Component<Animator2DComponent>
{
public:
	friend class Animator2DSystem;

	Animator2DComponent(Animator2DRef animator, float animationSpeed = 1);
	~Animator2DComponent() = default;

	inline void setAnimator(Animator2DRef animator) { m_animator = animator; }
	inline Animator2DRef getAnimator() const { return m_animator; }

	void resetAnimator();
	//return true if the frame have changed from the last addTime
	bool update(float elapsedTime);
	void setAnimationSpeed(float value);
	float getAnimationSpeed() const;
	const Frame & getCurrentFrame() const;
	void pause();
	void start();
	void stop();
	bool isPaused() const;
	void attach(Nz::SpriteRef sprite);
	void detach(Nz::SpriteRef sprite);
	void detachAll();

	void setProperty(const std::string & key, int value);
	int getProperty(const std::string & key) const;

	static void setFrame(Nz::SpriteRef & sprite, const Frame & f);

	static Ndk::ComponentIndex componentIndex;

private:
	bool currentAnimationFinished() const;
	void updateCurrentFrame();
	bool checkTransitions();

	Animator2DRef m_animator;
	std::vector<Nz::SpriteRef> m_sprites;
	PropertiesHolder m_properties;

	Animation2DState * m_currentState;
	float m_currentStateTime;
	Frame m_currentFrame;
	float m_animationSpeed;
	bool m_paused;
	bool m_needToUpdate;
};

