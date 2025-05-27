#include "Game.h"
#include "Player.h"

#include "Engine/Game/BlockType.h"
#include "Engine/Render/Resources/Camera.h"

namespace GameObject::Gameplay
{
	bool PlayerScript::OnInit()
	{
		m_TransformComponent = GetParentObject()->GetComponent<TransformComponent>(typeid(TransformComponent));
		if (!m_TransformComponent)
		{
			return false;
		}

        m_Size = { 1, 2, 1 };

        m_ColliderComponent = std::make_shared<ColliderComponent>(m_Size);

		GetParentObject()->AddComponent(m_ColliderComponent);

		auto chunkLoaderObject = GetParentObject()->GetScene()->FindGameObject("ChunkLoader");
		m_Chunks = chunkLoaderObject->GetComponent<ChunkLoaderComponent>(typeid(ChunkLoaderComponent))->GetChunks();

		return true;
	}

	void PlayerScript::OnTick(float dt)
	{
		HandleMovement(dt);
	}

	void PlayerScript::OnDestroy()
	{

	}
    void PlayerScript::HandleMovement(float dt)
    {
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

		GetParentObject()->GetComponent<TransformComponent>(typeid(TransformComponent))->SetPosition(position);

		Renderer::GetCamera()->SetRotation(rotation);
		Renderer::GetCamera()->SetPosition(position);

		if (InputManager::IsMouseDown(EMouseButton::Left))
		{
			for (const auto& chunk : m_Chunks)
			{
				const std::shared_ptr<TransformComponent> transform = chunk->GetParentObject()->GetComponent<TransformComponent>(typeid(TransformComponent));
				const Vector3 chunkPos = transform->GetPosition();

				const Vector3 origin = m_CameraPosition - chunkPos;

				Vector3 outHit, blockHit;
				Vector3 worldOutHit = outHit + chunkPos;
				Vector3 worldBlockHit = blockHit + chunkPos;

				if (chunk->Raycast(origin, m_CameraPosition.GetForward() * 100.0f, 20.0f, worldOutHit, worldBlockHit))
				{
					chunk->DestroyBlock((int)worldBlockHit.X, (int)worldBlockHit.Y, (int)worldBlockHit.Z);
					Logger::Log("Looking at Block: " + BlockTypeToString(chunk->GetBlock((int)worldBlockHit.X, (int)worldBlockHit.Y, (int)worldBlockHit.Z)), ELogLevel::Debug);
				}
			}
		}
    }
}//namespace GameObject::Gameplay