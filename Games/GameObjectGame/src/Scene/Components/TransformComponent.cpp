#include "Game.h"
#include "TransformComponent.h"

namespace GameObject
{
	bool TransformComponent::Init()
	{
		return false;
	}
	void TransformComponent::Tick(float dt)
	{
	}
	void TransformComponent::Destroy()
	{
	}
	Engine::Vector3 TransformComponent::GetPosition() const
	{
		return m_Position;
	}
	void TransformComponent::SetPosition(const Engine::Vector3& position)
	{
		m_Position = position;
	}
}//namespace