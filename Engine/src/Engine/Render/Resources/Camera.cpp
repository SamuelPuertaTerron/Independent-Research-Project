#include "Engine.h"
#include "Camera.h"

#include "ICameraContext.h"

namespace Engine::Render
{
	void Camera::Init()
	{
		m_CameraContext = ICameraContext::Create();
	}

	void Camera::UpdateTransform() const
	{
		m_CameraContext->UpdateTransform(m_Position, m_Rotation);
	}

	void Camera::SetPosition(const Vector3& position)
	{
		m_Position = position;
	}

	void Camera::SetRotation(const Vector3& rotation)
	{
		m_Rotation = rotation;
	}

	Vector3 Camera::GetPosition() const
	{
		return m_Position;
	}

	Vector3 Camera::GetRotation() const
	{
		return m_Rotation;
	}
	std::shared_ptr<ICameraContext> Camera::GetContext() const
	{
		return m_CameraContext;
	}
}//namespace Engine::Render