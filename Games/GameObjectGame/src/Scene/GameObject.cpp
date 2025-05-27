#include "Game.h"
#include "GameObject.h"

namespace GameObject
{
	bool GameObject::Init()
	{
		for (auto& component : m_Components)
		{
			component->Init();
		}

		return true;
	}

	void GameObject::Tick(float dt)
	{
		for (auto& component : m_Components)
		{
			if (!component->GetShouldUpdate())
			{
				return;
			}

			component->Tick(dt);
		}
	}

	void GameObject::Destroy()
	{
		for (auto& component : m_Components)
		{
			component->Destroy();
		}
	}

	void GameObject::AddComponent(const std::shared_ptr<IComponent>& component)
	{
		if (!component->GetParentObject())
			component->SetObject(this);

		m_Components.push_back(component);
	}

	Scene* GameObject::GetScene() const
	{
		if (!m_Scene)
		{
			Engine::Logger::Log("Scene is null", Engine::ELogLevel::Error);
			return nullptr;
		}

		return m_Scene;
	}
	std::string GameObject::GetName() const
	{
		return m_Name;
	}
}//namespace GameObject