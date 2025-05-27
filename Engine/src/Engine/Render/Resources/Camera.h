/* The main camera class */
#pragma once

namespace Engine::Render
{
	class ICameraContext;

	class Camera
	{
	public:
		Camera() = default;
		~Camera() = default;

		void Init();

		void UpdateTransform() const;

		void SetPosition(const Vector3& position);
		void SetRotation(const Vector3& rotation);

		[[nodiscard]] Vector3 GetPosition() const;
		[[nodiscard]] Vector3 GetRotation() const;

		std::shared_ptr<ICameraContext> GetContext() const;

	private:
		std::shared_ptr<ICameraContext> m_CameraContext;
		Vector3 m_Position;
		Vector3 m_Rotation;
	};

}//namespace Engine::Render