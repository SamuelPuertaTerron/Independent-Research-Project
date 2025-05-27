/* The main application class, Initializes, Runs and Destroys the client app */
#pragma once

namespace Engine
{
	class IImGuiWindow;
}

namespace Engine
{
	class IApp;
	class Window;

	struct ApplicationSettings
	{
		std::string Title;
		Vector2Int Size;
		Render::EGraphicsAPI GraphicsAPI;
	};

	class Application : public TSingleton<Application>
	{
	public:
		/**
		 * @brief Runs the application using the client app.
		 * @remark Returns assertion if it failed to initialize. 
		 */
		void RunApplication(const std::shared_ptr<IApp>& app);

		void Quit();

		/* Getter/Setter Functions */

		[[nodiscard]] std::shared_ptr<IApp> GetApp() const;
		[[nodiscard]] std::shared_ptr<Window> GetWindow() const;

		[[nodiscard]] float GetDeltaTime() const { return m_DeltaTime; }
		[[nodiscard]] float GetFPS() const { return m_FPS; }

	private:
		/**
		 * @brief  Initializes the main game engine
		 * @return True if succeeded, false otherwise. 
		 * @remark This will return an assertion upon failure. 
		 */
		bool Init();
		void Tick();
		void Destroy();

		void UpdateFPSCounter();

	private:
		bool m_bIsRunning;

		std::shared_ptr<IApp> m_App;
		std::shared_ptr<Window> m_Window;
		std::shared_ptr<IImGuiWindow> m_ImGuiWindow;

		//Timing Variables
		std::chrono::time_point<std::chrono::high_resolution_clock> m_LastFrameTime;
		float m_DeltaTime = 0.0f;
		float m_FPS = 0.0f;
		int m_FrameCount = 0;
		float m_FPSTimer = 0.0f;
 	};
}//namespace Engine
