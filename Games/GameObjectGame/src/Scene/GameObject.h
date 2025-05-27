/* The class to act as an object within the scene */
#pragma once
#include "IComponent.h"

namespace GameObject
{
	class GameObject
	{
	public:
		GameObject() = default;
		GameObject(Scene* scene, std::string name)
			: m_Scene(scene), m_Name(std::move(name))
		{
			m_Components.reserve(4);
		}

		~GameObject() = default;

		bool Init();
		void Tick(float dt);
		void Destroy();

		void AddComponent(const std::shared_ptr<IComponent>& component);
		template<typename TComponent>
		std::shared_ptr<TComponent> GetComponent(const type_info& info) const
		{
			SE_ASSERT(!m_Components.empty(), "Components are empty");

			for (std::shared_ptr<IComponent> component : m_Components)
			{
				if (typeid(*component).name() == info.name())
				{
					// Try to dynamic_pointer_cast to the desired type
					auto casted = std::dynamic_pointer_cast<TComponent>(component);
					if (casted)
					{
						return casted;
					}
				}
			}

			std::stringstream ss;
			ss << "Could not find component with name " << info.name();

			Engine::Logger::Log(ss.str(), Engine::ELogLevel::Warning);
			return nullptr;
		}

		Scene* GetScene() const;

		std::string GetName() const;
	private:
		Scene* m_Scene;
		std::string m_Name;
		std::vector<std::shared_ptr<IComponent>> m_Components;
	};
}//namespace GameObject