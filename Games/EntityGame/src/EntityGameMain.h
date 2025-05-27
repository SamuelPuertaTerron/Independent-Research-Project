/* The main runtime for the entity game */

#pragma once

namespace Engine
{
	class ChunkLoader;
}//namespace Engine

namespace EntityGame
{
	//A very basic state machine for if the user is on the menu or game
	enum class EGameState
	{
		None = 0,
		Menu,
		Settings,
		Pause,
		Game,
	};


	class EntityGameMain : public Engine::IApp
	{
	public:
		EntityGameMain() = default;
		virtual ~EntityGameMain() override = default;

		Engine::ApplicationSettings GetSettings() const override;

		virtual void OnAppLoadSettings() override;
		virtual void PreInit() override;
		virtual bool Init() override;
		virtual void Destroy() override;

		virtual void Tick(float dt) override;

	private:
		void MainMenu(const Vector2Int& windowSize);
		void SettingsMenu(const Vector2Int& windowSize);
		void PauseMenu(const Vector2Int& windowSize);

		void LoadGameScene();
		void RunGameScene(float dt);
	private:
		EGameState m_GameState;
		bool m_bHasLoaded;

		Scene* m_Scene;
		std::shared_ptr<Engine::ChunkLoader> m_Loader;
		std::shared_ptr<Render::Camera> m_Camera;
	};
}//namespace EntityGame