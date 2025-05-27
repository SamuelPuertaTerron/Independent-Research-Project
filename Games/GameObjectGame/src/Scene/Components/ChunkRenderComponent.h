/* Render the chunks in the game world */
#pragma once
#include "Engine/Game/Chunk.h"

namespace GameObject
{
	class ChunkRenderComponent : public IComponent, public Engine::Chunk
	{
	public:
		ChunkRenderComponent() = default;
		virtual ~ChunkRenderComponent() override = default;

		virtual bool Init() override;
		virtual void Tick(float dt) override {}
		virtual void Destroy() override		 {}
	};
}//namespace GameObject