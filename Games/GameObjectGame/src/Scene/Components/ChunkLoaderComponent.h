/* Loads and renders chunks around the player */
#pragma once

namespace Engine
{
	class Chunk;
}//namespace Chunk

namespace GameObject
{
	class ChunkLoaderComponent : public IComponent
	{
	public:
		ChunkLoaderComponent() = default;
		virtual ~ChunkLoaderComponent() override = default;

		virtual bool Init() override;
		virtual void Tick(float dt) override;
		virtual void Destroy() override;

		std::vector<std::shared_ptr<ChunkRenderComponent>> GetChunks() const
		{
			return m_Chunks;
		}

	private:
		int m_ViewDistance{12}; //Maybe store in player 
		std::vector<std::shared_ptr<ChunkRenderComponent>> m_Chunks; //Queue
	};
}//namespace GameObject