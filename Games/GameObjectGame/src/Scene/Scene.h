/* The scene this game
 * Contains a list of game objects which are initialized, updated and destroyed. 
 */
#pragma once

namespace GameObject
{
	class GameObject;

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		bool Init();
		void Destroy();

		void Tick(float dt);

		std::shared_ptr<GameObject> CreateGameObject(const std::string& name);

		void UpdateGameObject(const std::shared_ptr<GameObject>& object);

		std::shared_ptr<GameObject> FindGameObject(const std::string& name);
		std::vector<std::shared_ptr<GameObject>> FindGameObjectContaining(const std::string& name);
	private:
		std::unordered_map<std::string, std::shared_ptr<GameObject>> m_GameObjects;
	};
}//namespace GameObject