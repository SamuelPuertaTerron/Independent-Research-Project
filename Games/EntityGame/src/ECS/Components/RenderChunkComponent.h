/* A render chunk component */
#pragma once

namespace Engine
{
	class Chunk;
}//namespace Engine

namespace EntityGame
{
	struct RenderChunkComponent : public IComponent
	{
		std::shared_ptr<Chunk> Chunk;
		Engine::Colour Colour = Engine::Red;
	};
}//namespace EntityGame