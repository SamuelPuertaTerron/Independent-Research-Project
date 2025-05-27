#include "Game.h"
#include "PlayerMovementScript.h"

#include "Engine/Application/Input/KeyCodes.h"
#include "Engine/Render/Resources/Camera.h"

namespace EntityGame
{
	void PlayerMovementScript::OnInit()
	{

	}

	void PlayerMovementScript::OnTick(float dt)
	{
		const auto& transform = m_Scene->GetComponent<TransformComponent>(m_Entity);
	
		Vector3 position = Renderer::GetCamera()->GetPosition();
		Vector3 rotation = Renderer::GetCamera()->GetRotation();
		
		if (InputManager::IsKeyDown(EKeyCode::W))
		{
			position.Z += g_Settings.CameraSpeed * dt;
		}
		if (InputManager::IsKeyDown(EKeyCode::S))
		{
			position.Z -= g_Settings.CameraSpeed * dt;
		}
		if (InputManager::IsKeyDown(EKeyCode::A))
		{
			position.X -= g_Settings.CameraSpeed * dt;
		}
		if (InputManager::IsKeyDown(EKeyCode::D))
		{
			position.X += g_Settings.CameraSpeed * dt;
		}
		if (InputManager::IsKeyDown(EKeyCode::Q))
		{
			position.Y += g_Settings.CameraSpeed * dt;
		}
		if (InputManager::IsKeyDown(EKeyCode::E))
		{
			position.Y -= g_Settings.CameraSpeed * dt;
		}

		if (InputManager::IsKeyDown(EKeyCode::R))
		{
			rotation.Y += g_Settings.CameraSpeed * dt;
		}
		if (InputManager::IsKeyDown(EKeyCode::F))
		{
			rotation.Y -= g_Settings.CameraSpeed * dt;
		}

		transform->Position = position;
		transform->Rotation = rotation;

		Renderer::GetCamera()->SetRotation(rotation);
		Renderer::GetCamera()->SetPosition(position);
	}
}//namespace EntityGame