/* The app from the client applications */

#pragma once

namespace Engine
{
	struct ApplicationSettings;

	class IApp
	{
	public:
		virtual ~IApp() = default;

		virtual ApplicationSettings GetSettings() const = 0;
		virtual void OnAppLoadSettings() {};
		virtual void PreInit() {}
		virtual bool Init() = 0;
		virtual void Tick(float dt) = 0;
		virtual void Destroy() = 0;
		virtual void LateDestroy() {}
	};
}//namespace Engine