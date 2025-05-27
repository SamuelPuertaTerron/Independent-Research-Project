/* The main class for handling Entities and Components */
#pragma once

namespace EntityGame
{
	namespace Utils
	{
		// Helper to check if all components derive from Component
		template<typename...>
		struct AllComponentsValid;

		template<typename TFirst, typename... TRest>
		struct AllComponentsValid<TFirst, TRest...> {
			static constexpr bool value =
				std::is_base_of_v<IComponent, TFirst>&& 
				AllComponentsValid<TRest...>::value;
		};

		template<>
		struct AllComponentsValid<> {
			static constexpr bool value = true;
		};
	}//namespace Utils

	class Registry
	{
	public:
		Registry() = default;
		~Registry() = default;

		Entity CreateEntity()
		{
			return m_NextEntity++;
		}

		template<typename T, typename... Args>
		void AddComponent(Entity entity, Args&&... args)
		{
			if (!std::is_base_of_v<IComponent, T>) 
			{
				Logger::Log("Trying to add component but is not a type of IComponent", ELogLevel::Warning);
			}

			m_Components[typeid(T)][entity] = std::make_unique<T>(std::forward<Args>(args)...);
		}

		template<typename T>
		T* GetComponent(Entity entity)
		{
			if (!std::is_base_of_v<IComponent, T>)
			{
				Logger::Log("Trying to get component but is not a type of IComponent", ELogLevel::Warning);
			}

			auto& typedComps = m_Components[typeid(T)];
			auto it = typedComps.find(entity);
			return (it != typedComps.end()) ? static_cast<T*>(it->second.get()) : nullptr;
		}

		template<typename... TComponents>
		std::vector<Entity> GetEntitiesWith() {
			static_assert(sizeof...(TComponents) > 0, "Must specify at least one component");
			static_assert(Utils::AllComponentsValid<TComponents...>::value,
				"All components must derive from IComponent");

			std::vector<Entity> result;

			for (Entity e = 0; e < m_NextEntity; e++) {
				// Use fold expression to check all components
				bool hasAll = ((GetComponent<TComponents>(e) != nullptr) && ...);

				if (hasAll) {
					result.push_back(e);
				}
			}

			if (result.empty()) {
				Logger::Log("No entities found with all specified components",
					ELogLevel::Warning);
			}

			return result;
		}

	private:
		Entity m_NextEntity = 0;

		std::unordered_map<std::type_index, std::unordered_map<Entity, std::unique_ptr<IComponent>>> m_Components;
	};
}//namespace EntityGame