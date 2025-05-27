#include "Game.h"

#include "ECS/Components/ChunkLoaderComponent.h"
#include "ECS/Components/RenderChunkComponent.h"
#include "Engine/Game/Chunk.h"

namespace EntityGame
{
	void Scene::Init()
	{
		m_ChunkLoader = CreateEntity();
		AddComponent<chunkLoaderComponent>(m_ChunkLoader);
		chunkLoaderComponent* loader = GetComponent<chunkLoaderComponent>(m_ChunkLoader);

		// Generate initial chunks around the player
		for (int x = -g_Settings.MaxChunkSize; x <= g_Settings.MaxChunkSize; x++)
		{
			for (int z = -g_Settings.MaxChunkSize; z <= g_Settings.MaxChunkSize; z++)
			{
				Entity chunkEntity = CreateEntity();

				AddComponent<TransformComponent>(chunkEntity);
				AddComponent<RenderChunkComponent>(chunkEntity);

				auto chunk = GetComponent<RenderChunkComponent>(chunkEntity);
				auto transform = GetComponent<TransformComponent>(chunkEntity);

				transform->Position = { static_cast<float>(x * CHUNK_SIZE), 0.0f, static_cast<float>(z * CHUNK_SIZE) };

				chunk->Chunk = std::make_shared<Chunk>();
				chunk->Chunk->Generate(transform->Position);
				chunk->Chunk->BuildMesh();

				loader->m_Chunks.push_back(*chunk->Chunk);
			}
		}
		
	}

	void Scene::Tick(float dt)
	{
		ScriptSystem(dt);
		RenderSystem();
		ChunkRenderSystem();
	}

	void Scene::Destroy()
	{
		
	}

	void Scene::ScriptSystem(float dt)
	{
		auto scriptEntities = GetEntitiesWith<TransformComponent, RenderComponent>();
		for (Entity e : scriptEntities)
		{
			auto* script = GetComponent<ScriptComponent>(e);
			if (!script || !script->Script)
			{
				continue;
			}

			script->Script->OnTick(dt);
		}
	}

	void Scene::RenderSystem()
	{
		auto renderEntities = GetEntitiesWith<TransformComponent, RenderComponent>();
		for (Entity e : renderEntities)
		{
			auto* transform = GetComponent<TransformComponent>(e);
			auto* render = GetComponent<RenderComponent>(e);
			if (!transform || !render)
			{
				continue;
			}

			Renderer::Draw(transform->Position, transform->Rotation, transform->Scale, render->Colour);
		}
	}
	void Scene::ChunkRenderSystem()
	{
		auto renderEntities = GetEntitiesWith<TransformComponent, RenderChunkComponent>();
		for (Entity e : renderEntities)
		{
			auto* transform = GetComponent<TransformComponent>(e);
			auto* render = GetComponent<RenderChunkComponent>(e);
			if (!transform || !render)
			{
				continue;
			}

			Renderer::DrawMesh(render->Chunk->GetMesh(), transform->Position, transform->Rotation, transform->Scale, render->Colour);
		}
	}
}//namespace EntityGame