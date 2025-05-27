#include "Game.h"
#include "Scene.h"

namespace GameObject
{
	bool Scene::Init()
	{
		for (const auto& [index, object] : m_GameObjects)
		{
			if (!object->Init())
			{
				std::stringstream ss;
				ss << "Game Object " << object->GetName() << " Cannot be Initialized!";
				Engine::Logger::Log(ss.str(), Engine::ELogLevel::Error);
			}
		}

		return true;
	}
	void Scene::Destroy()
	{
		for (const auto& [index, object] : m_GameObjects)
		{
			object->Destroy();
		}
	}

	void Scene::Tick(float dt)
	{
		for (const auto& [index, object] : m_GameObjects)
		{
			object->Tick(dt);
		}
	}

	std::shared_ptr<GameObject> Scene::CreateGameObject(const std::string& name)
	{
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>(this, name);

		auto it = m_GameObjects.find(object->GetName());
		if (it != m_GameObjects.end())
		{
			Engine::Logger::Log("Game Object is already found within the map", Engine::ELogLevel::Warning);
			return m_GameObjects[name];
		}

		object->AddComponent(std::make_shared<TransformComponent>());
		object->AddComponent(std::make_shared<RenderComponent>());

		m_GameObjects[object->GetName()] = object;
		return m_GameObjects[object->GetName()];
	}
	void Scene::UpdateGameObject(const std::shared_ptr<GameObject>& object)
	{
		auto it = m_GameObjects.find(object->GetName());
		if (it != m_GameObjects.end())
		{
			m_GameObjects[object->GetName()] = object;
		}

		Engine::Logger::Log("Game Object is not found within the map", Engine::ELogLevel::Warning);
	}

	std::shared_ptr<GameObject> Scene::FindGameObject(const std::string& name)
	{
		auto it = m_GameObjects.find(name);
		if (it != m_GameObjects.end())
		{
			return m_GameObjects[name];
		}

		Engine::Logger::Log("Game Object is not found within the map", Engine::ELogLevel::Warning);
		return nullptr;
	}

	std::vector<std::shared_ptr<GameObject>> Scene::FindGameObjectContaining(const std::string& name)
	{
		std::vector<std::shared_ptr<GameObject>> results;

		for (const auto& [name, object] : m_GameObjects)
		{
			if (name.find(name) != std::string::npos) //name matches
			{
				results.push_back(object);
			}
		}

		if (results.empty())
		{
			Engine::Logger::Log("No game objects found containing: " + name, Engine::ELogLevel::Warning);
		}

		return results;
	}
}//namespace GameObject