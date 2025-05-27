#include "Game.h"
#include "GameObjectGame.h"

#include "Engine/Application/Audio.h"
#include "Gameplay/Player.h"
#include "ImGui/imgui/imgui.h"
#include "Engine/Render/Resources/Camera.h"

namespace GameObject
{
	ApplicationSettings GameObjectGame::GetSettings() const
	{
		ApplicationSettings settings;
		settings.Title = "Game Object Game";
		settings.Size = { 1270, 720 };
		settings.GraphicsAPI = Render::EGraphicsAPI::OpenGL;
		return settings;
	}

	void GameObjectGame::OnAppLoadSettings()
	{
		if (!LoadSettings("Resources/Settings.sav", g_Settings))
		{
			SaveSettings("Resources/Settings.sav", g_Settings);
		}
	}

	void GameObjectGame::PreInit()
	{
		m_GameState = EGameState::Menu;
		m_Camera = std::make_shared<Render::Camera>();
		Renderer::SetCamera(m_Camera);
	}

	bool GameObjectGame::Init()
	{
		Audio::GetInstance().Play("Resources/crafting_game_music.mp3");
		Audio::GetInstance().UpdateVolume(g_Settings.Volume);

		return true;
	}

	void GameObjectGame::Destroy()
	{
		if (m_GameScene)
		{
			m_GameScene->Destroy();
		}
	}

	void GameObjectGame::Tick(float dt)
	{
		const Vector2Int windowSize = Application::GetInstance().GetWindow()->GetWindowSize();

		switch (m_GameState)
		{
		case EGameState::Menu:
		{
			MainMenu(windowSize);
		}
		break;
		case EGameState::Settings:
		{
			SettingsMenu(windowSize);
		}
		break;
		case EGameState::Pause:
		{
			PauseMenu(windowSize);
		}
		break;
		case EGameState::Game:
		{
			if (m_GameScene)
			{
				if (InputManager::IsKeyDown(EKeyCode::Escape))
				{
					m_GameState = EGameState::Pause;
				}

				RunGameScene(dt);
			}
		}
		break;
		case EGameState::None:
			break;
		}
	}
	void GameObjectGame::MainMenu(const Vector2Int& windowSize)
	{
		ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(static_cast<float>(windowSize.X), static_cast<float>(windowSize.Y)));

		if (ImGui::Button("Start Game", ImVec2(200, 50)))
		{
			m_GameState = EGameState::Game;
			if (!m_bHasLoaded)
			{
				LoadGameScene();
			}
			m_bHasLoaded = true;
		}
		if (ImGui::Button("Settings", ImVec2(200, 50)))
		{
			m_GameState = EGameState::Settings;
		}
		if (ImGui::Button("Quit Game", ImVec2(200, 50)))
		{
			Application::GetInstance().Quit();
		}

		ImGui::End();
	}

	void GameObjectGame::SettingsMenu(const Vector2Int& windowSize)
	{
		ImGui::Begin("Settings Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(static_cast<float>(windowSize.X), static_cast<float>(windowSize.Y)));

		ImGui::Text("Graphics. Changing this will result in a game restart");
		const char* graphicsAPIs[] = { "None", "OpenGL", "Direct3D11" };
		int currentAPI = static_cast<int>(g_Settings.GraphicsAPI);
		if (ImGui::Combo("##GraphicsAPI", &currentAPI, graphicsAPIs, IM_ARRAYSIZE(graphicsAPIs)))
		{
			g_Settings.GraphicsAPI = static_cast<Engine::Render::EGraphicsAPI>(currentAPI);
		}

		ImGui::Text("Max Chunk Size:");
		ImGui::SliderInt("##MaxChunkSize", &g_Settings.MaxChunkSize, 4, 64);

		ImGui::Text("Camera Speed:");
		ImGui::SliderFloat("##CameraSpeed", &g_Settings.CameraSpeed, 1.0f, 100.0f, "%.1f");

		ImGui::Text("Volume:");
		ImGui::SliderFloat("##Volume", &g_Settings.Volume, 0.0f, 1.0f, "%.1f");

		ImGui::Text("Credits");
		ImGui::Text("Music: Alex McCulloch - OpenGameArt");

		if (ImGui::Button("Apply", ImVec2(200, 50)))
		{
			SaveSettings("Resources/Settings.sav", g_Settings);
			Audio::GetInstance().UpdateVolume(g_Settings.Volume);
		}

		if (ImGui::Button("Back", ImVec2(200, 50)))
		{

			m_GameState = EGameState::Menu;
		}

		ImGui::End();
	}

	void GameObjectGame::PauseMenu(const Vector2Int& windowSize)
	{
		ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(static_cast<float>(windowSize.X), static_cast<float>(windowSize.Y)));

		if (ImGui::Button("Resume Game", ImVec2(200, 50)))
		{
			m_GameState = EGameState::Game;
		}

		if (ImGui::Button("Quit to Menu", ImVec2(200, 50)))
		{
			m_GameState = EGameState::Menu;
		}

		ImGui::End();
	}

	void GameObjectGame::LoadGameScene()
	{
		m_GameScene = std::make_shared<Scene>();

		auto chunkLoader = m_GameScene->CreateGameObject("ChunkLoader");
		chunkLoader->AddComponent(std::make_shared<ChunkLoaderComponent>());

		auto camera = m_GameScene->CreateGameObject("Camera");
		camera->AddComponent(std::make_shared<Gameplay::PlayerScript>());

		m_GameScene->Init();
	}
	void GameObjectGame::RunGameScene(float dt)
	{
		const auto camera = m_GameScene->FindGameObject("Camera");
		const auto transform = camera->GetComponent<TransformComponent>(typeid(TransformComponent));

		ImGui::Begin("Settings");
		{
			ImGui::Text("Current FPS %f", Application::GetInstance().GetFPS());

			ImGui::Spacing();

			ImGui::Text("Current Height: %i", static_cast<int>(transform->GetPosition().Y));
			ImGui::Text("Current Chunks Rendered %i", g_Settings.MaxChunkSize);
			ImGui::Text("Current Camera Speed %f", g_Settings.CameraSpeed);
			ImGui::Text("Graphics API %s", Render::GraphicsAPIToString().c_str());
		}
		ImGui::End();

		m_GameScene->Tick(dt);
	}
}//namespace GameObject