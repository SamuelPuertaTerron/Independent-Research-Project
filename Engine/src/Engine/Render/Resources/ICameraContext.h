/* A class that handles different transforms for different Graphics API */
#pragma once

namespace Engine::Render
{
	class ICameraContext
	{
	public:
		virtual ~ICameraContext() = default;

		virtual void UpdateTransform(const Vector3& position, const Vector3& rotation) = 0;

		static std::shared_ptr<ICameraContext> Create();
	};
}//namespace Engine::Render