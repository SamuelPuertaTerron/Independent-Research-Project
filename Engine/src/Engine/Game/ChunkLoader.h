/* A function to load chunks at runtime */
#pragma once

namespace Engine
{
	class Chunk;

	class ChunkLoader 
	{
	public:
		ChunkLoader();
		~ChunkLoader() = default;

		void Init();
		void Render();
	private:
		int m_ViewDistance;
		std::vector<std::shared_ptr<Chunk>> m_Chunks;
	};
}//namespace Engine