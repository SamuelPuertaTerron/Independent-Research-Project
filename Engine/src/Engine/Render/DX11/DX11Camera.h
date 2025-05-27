#pragma once
#include <DirectXMath.h>

#include "Engine/Render/Resources/ICameraContext.h"

namespace Engine::Render::DX11
{
	class DX11Camera : public ICameraContext
	{
	public:
		DX11Camera() = default;
		virtual ~DX11Camera() override = default;

		virtual void UpdateTransform(const Vector3& position, const Vector3& rotation) override;

		DirectX::XMFLOAT4X4 GetViewMatrix() const;
		DirectX::XMFLOAT4X4 GetProjectionMatrix() const;

	private:
		DirectX::XMFLOAT4X4 m_ViewMatrix;
		DirectX::XMFLOAT4X4 m_ProjectionMatrix;
	};

}//namespace Engine::Render::DX11
