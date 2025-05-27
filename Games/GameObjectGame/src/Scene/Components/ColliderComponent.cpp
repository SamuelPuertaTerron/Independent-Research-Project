#include "Game.h"
#include "ColliderComponent.h"

namespace GameObject
{
	ColliderComponent::ColliderComponent(const Engine::Vector3& size)
	{
		Vector3 min = size / 2.0f;
		Vector3 max = size;
		m_Collision.SetSize(min, max);

		m_bShouldUpdate = false;
	}
	bool ColliderComponent::Init()
	{
		return true;
	}

	void ColliderComponent::Destroy()
	{
		
	}
}//namespace GameObject