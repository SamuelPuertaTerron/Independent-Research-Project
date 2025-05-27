#include "Engine.h"
#include "DX11Camera.h"

namespace Engine::Render::DX11
{
    void DX11Camera::UpdateTransform(const Vector3& position, const Vector3& rotation)
    {
        using namespace DirectX;

        XMFLOAT3 dxVector = { position.X, position.Y, position.Z };

        XMVECTOR eyePosition = XMLoadFloat3(&dxVector);
        XMVECTOR focusPoint = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
        XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

        // Apply rotation
        XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(rotation.X),
            XMConvertToRadians(rotation.Y),
            XMConvertToRadians(rotation.Z)
        );

        focusPoint = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
        focusPoint = XMVectorAdd(eyePosition, focusPoint);
        upDirection = XMVector3TransformCoord(upDirection, rotationMatrix);

        const float fov = 45.0f; // Field of view
        const float aspectRatio = 16.0f / 9.0f; // Should match viewport aspect ratio
        const float nearPlane = 0.1f;
        const float farPlane = 1000.0f;

        XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane));

        XMMATRIX viewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
        DirectX::XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);
    }

    DirectX::XMFLOAT4X4 DX11Camera::GetViewMatrix() const
    {
        return m_ViewMatrix;
    }

    DirectX::XMFLOAT4X4 DX11Camera::GetProjectionMatrix() const
    {
        return m_ProjectionMatrix;
    }

}//namespace Engine::Render::DX11