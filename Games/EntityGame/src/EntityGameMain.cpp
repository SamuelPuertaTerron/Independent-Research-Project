#include "Game.h"
#include "EntityGameMain.h"

#include "Engine/Application/Audio.h"
#include "Engine/Application/Input/KeyCodes.h"
#include "Engine/Render/Vertext.h"
#include "Engine/Render/Resources/IMesh.h"
#include "Gameplay/PlayerMovementScript.h"

#include "Engine/Game/ChunkLoader.h"
#include "Engine/Render/Resources/Camera.h"
#include "ImGui/imgui/imgui.h"

namespace EntityGame
{
	Engine::ApplicationSettings EntityGameMain::GetSettings() const
	{
		Engine::ApplicationSettings settings;
		settings.Title = "Entity Game";
		settings.Size = { 1270, 720 };
		settings.GraphicsAPI = g_Settings.GraphicsAPI;
		return settings;
	}

	void EntityGameMain::OnAppLoadSettings()
	{
		if (!LoadSettings("Resources/Settings.sav", g_Settings))
		{
			SaveSettings("Resources/Settings.sav", g_Settings);
		}
	}

	void EntityGameMain::PreInit()
	{
		m_Camera = std::make_shared<Render::Camera>();
		Renderer::SetCamera(m_Camera);
	}

	bool EntityGameMain::Init()
	{
		Audio::GetInstance().Play("Resources/crafting_game_music.mp3");
		Audio::GetInstance().UpdateVolume(g_Settings.Volume);

		m_GameState = EGameState::Menu;
		return true;
	}

	void EntityGameMain::Destroy()
	{
		m_Scene->Destroy();
		delete m_Scene;
	}

	void EntityGameMain::Tick(float dt)
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
			if (m_Scene)
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

	void EntityGameMain::MainMenu(const Vector2Int& windowSize)
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

	void EntityGameMain::SettingsMenu(const Vector2Int& windowSize)
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

	void EntityGameMain::PauseMenu(const Vector2Int& windowSize)
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

	void EntityGameMain::LoadGameScene()
	{
		m_Scene = new Scene();
		m_Scene->Init();

		Entity player = m_Scene->CreateEntity();
		m_Scene->AddComponent<TagComponent>(player);

		TagComponent* comp = m_Scene->GetComponent<TagComponent>(player);
		comp->Name = "Player";

		m_Scene->AddComponent<TransformComponent>(player);
		m_Scene->AddComponent<RenderComponent>(player);
		m_Scene->AddComponent<ScriptComponent>(player);

		auto* script = m_Scene->GetComponent<ScriptComponent>(player);
		script->Script = std::make_unique<PlayerMovementScript>(player, m_Scene);
	}

	void EntityGameMain::RunGameScene(float dt)
	{
		auto entities = m_Scene->GetEntitiesWith<TagComponent, TransformComponent>();
		for (const auto& e : entities)
		{
			TagComponent* tag = m_Scene->GetComponent<TagComponent>(e);
			TransformComponent* transform = m_Scene->GetComponent<TransformComponent>(e);

			if (tag->Name == "Player")
			{
				ImGui::Begin("Settings");
				{
					ImGui::Text("Current FPS %f", Application::GetInstance().GetFPS());

					ImGui::Spacing();

					ImGui::Text("Current Height: %i", static_cast<int>(transform->Position.Y));
					ImGui::Text("Current Chunks Rendered %i", g_Settings.MaxChunkSize);
					ImGui::Text("Current Camera Speed %f", g_Settings.CameraSpeed);
					ImGui::Text("Graphics API %s", Render::GraphicsAPIToString().c_str());
				}
				ImGui::End();
			}
		}

		m_Scene->Tick(dt);
	}
}//namespace EntityGame