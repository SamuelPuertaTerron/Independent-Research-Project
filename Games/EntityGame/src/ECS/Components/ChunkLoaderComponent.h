/* Loads a chunk and renders each chunk block */
#pragma once

namespace Engine
{
	class Chunk;
}//namespace Engine

namespace EntityGame
{
	struct chunkLoaderComponent : IComponent
	{
		std::vector<Chunk> m_Chunks;
	};
}//namespace EntityGame