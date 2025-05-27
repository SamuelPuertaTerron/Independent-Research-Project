/* A component around BoxCollision class */
#pragma once
#include "Engine/Game/BoxCollision.h"

namespace GameObject
{
	class ColliderComponent : public IComponent
	{
	public:
		ColliderComponent(const Engine::Vector3& size);
		virtual ~ColliderComponent() override = default;

		virtual bool Init() override;
		virtual void Tick(float dt) override {}
		virtual void Destroy() override;

		Engine::BoxCollision GetCollider() const { return m_Collision; }

	private:
		Engine::BoxCollision m_Collision;
	};
}//namespace GameObject