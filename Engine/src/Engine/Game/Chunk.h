/* Chunk Generation that will be loaded */
#pragma once

#include "Block.h"
#include "Engine/Render/Resources/IMesh.h"

namespace Engine
{
    constexpr int CHUNK_SIZE = 16;
    constexpr int CHUNK_HEIGHT = 256;

	namespace Render::DX11
	{
		struct Vertex;
		class DX11Mesh;
	}//	namespace Render::DX11

    class Chunk
    {
    public:
        Chunk() = default;

        void Generate(const Vector3& position);
        void BuildMesh();

        bool IsMeshValid() const;

        std::shared_ptr<Render::IMesh> GetMesh() { return m_Mesh; }

        bool Raycast(const Vector3& origin, const Vector3& direction, float maxDistance, Vector3& outHitPos, Vector3& outBlockPos);

        void DestroyBlock(int x, int y, int z);

        EBlockType GetBlock(int x, int y, int z) const;

	private:
        void GenerateTree(int x, int y, int z);
        bool IsInBounds(int x, int y, int z);
    private:
        std::array<EBlockType, CHUNK_SIZE* CHUNK_HEIGHT* CHUNK_SIZE> m_Blocks;
        std::shared_ptr<Render::IMesh> m_Mesh;
        int Index(int x, int y, int z) const;
        bool IsBlockVisible(int x, int y, int z) const;
    };
}//namespace Engine