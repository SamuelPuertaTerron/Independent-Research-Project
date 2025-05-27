/* The scene class, encapsulates basic functionality for the ECS */
#pragma once

namespace EntityGame
{
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		Entity CreateEntity()
		{
			return m_Registry.CreateEntity();
		}

		template<typename T, typename... Args>
		void AddComponent(Entity entity, Args&&... args)
		{
			m_Registry.AddComponent<T, Args...>(entity, args...);
		}

		template<typename T>
		T* GetComponent(Entity entity)
		{
			return m_Registry.GetComponent<T>(entity);
		}

		template<typename... TComponents>
		std::vector<Entity> GetEntitiesWith()
		{
			return m_Registry.GetEntitiesWith<TComponents...>();
		}

		Entity GetEntityWithName(const std::string& name)
		{
			
		}

		void Tick(float dt);
		void Init();
		void Destroy();

	private:
		void ScriptSystem(float dt);
		void RenderSystem();
		void ChunkRenderSystem();
	private:
		Registry m_Registry;
		Entity m_ChunkLoader;
	};
}//namespace EntityGame
