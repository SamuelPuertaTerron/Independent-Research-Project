#include "Game.h"
#include "ChunkLoaderComponent.h"

namespace GameObject
{
	bool ChunkLoaderComponent::Init()
	{
		// Generate initial chunks around the player
		for (int x = -g_Settings.MaxChunkSize; x <= g_Settings.MaxChunkSize; x++)
		{
			for (int z = -g_Settings.MaxChunkSize; z <= g_Settings.MaxChunkSize; z++)
			{
				auto chunkObject = m_Object->GetScene()->CreateGameObject("Chunk " + std::to_string(Random::GetRandomFloat() * 1500));
				auto transform = chunkObject->GetComponent<TransformComponent>(typeid(TransformComponent));

				transform->SetPosition({ static_cast<float>(x * CHUNK_SIZE), 0.0f, static_cast<float>(z * CHUNK_SIZE)});

				chunkObject->AddComponent(std::make_shared<ChunkRenderComponent>());
				chunkObject->AddComponent(std::make_shared<ColliderComponent>(Vector3(1, 1, 1)));

				auto chunk = chunkObject->GetComponent<ChunkRenderComponent>(typeid(ChunkRenderComponent));
				if (!chunk->Init())
				{
					continue;
				}
				m_Chunks.push_back(chunk);
			}
		}

		return false;
	}

	void ChunkLoaderComponent::Tick(float dt)
	{
		for (auto& chunk : m_Chunks)
		{
			if (auto mesh = chunk->GetMesh())
			{
				std::shared_ptr<TransformComponent> comp = chunk->GetParentObject()->GetComponent<TransformComponent>(typeid(TransformComponent));
				Renderer::DrawMesh(mesh,comp->GetPosition(), {0, 0, 0}, {1, 1, 1}, Red);
			}
		}
	}

	void ChunkLoaderComponent::Destroy()
	{

	}
}//namespace GameObject