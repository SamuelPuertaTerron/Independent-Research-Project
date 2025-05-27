#include "Engine.h"
#include "Chunk.h"

#include "Engine/Render/Vertext.h"

#include "BlockType.h"
#include "../../../../ThirdParty/FastNoise/FastNoiseLite.h"

namespace Engine
{
	int Chunk::Index(int x, int y, int z) const
	{
		return x + (z * CHUNK_SIZE) + (y * CHUNK_SIZE * CHUNK_SIZE);
	}

	void Chunk::Generate(const Vector3& position)
	{
		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFrequency(0.05f);
		noise.SetFractalOctaves(5);

		static FastNoiseLite treeNoise; // Static to maintain state across chunks
		treeNoise.SetSeed(1337);
		treeNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		treeNoise.SetFrequency(0.008f);
		treeNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
		treeNoise.SetFractalOctaves(3);

		// Use world-space position to keep terrain seamless across chunks
		int worldX = static_cast<int>(position.X) * CHUNK_SIZE;
		int worldZ = static_cast<int>(position.Z) * CHUNK_SIZE;

		int terrainHeight {-1};

		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			for (int z = 0; z < CHUNK_SIZE; ++z)
			{
				// World space position
				int globalX = worldX + x;
				int globalZ = worldZ + z;

				// Scale world coordinates to prevent steep spikes
				float nx = static_cast<float>(globalX) / 100.0f;
				float nz = static_cast<float>(globalZ) / 100.0f;

				// Get smooth terrain height
				float rawHeight = noise.GetNoise(nx, nz);
				float height = rawHeight * 20.0f + 64.0f;
				terrainHeight = static_cast<int>(height);

				for (int y = 0; y < CHUNK_HEIGHT; ++y)
				{
					if (y > terrainHeight)
					{
						m_Blocks[Index(x, y, z)] = EBlockType::Air;
					}
					else if (y == 0)
					{
						m_Blocks[Index(x, y, z)] = EBlockType::Stone;
					}
					else if (y < terrainHeight - 4)
					{
						m_Blocks[Index(x, y, z)] = EBlockType::Stone;
					}
					else if (y < terrainHeight - 1)
					{
						m_Blocks[Index(x, y, z)] = EBlockType::Dirt;
					}
					else
					{
						m_Blocks[Index(x, y, z)] = EBlockType::Grass;
					}
				}
			}
		}

		if (terrainHeight == -1)
		{
			return;
		}

		constexpr int TREE_SPACING = 7;

		for (int x = 2; x < CHUNK_SIZE - 2; x += TREE_SPACING)
		{
			for (int z = 2; z < CHUNK_SIZE - 2; z += TREE_SPACING)
			{
				// World space position with random offset
				int globalX = worldX + x + Random::GetRandomIntInRange(-2, 2);
				int globalZ = worldZ + z + Random::GetRandomIntInRange(-2, 2);

				// Get local position within chunk
				int localX = globalX - worldX;
				int localZ = globalZ - worldZ;

				if (localX < 0 || localX >= CHUNK_SIZE ||
					localZ < 0 || localZ >= CHUNK_SIZE) continue;

				// Check if this is a valid tree position
				if (terrainHeight < 60 || terrainHeight > CHUNK_HEIGHT - 8) continue;

				// Check flat area (3x3 cells)
				bool isFlat = true;
				for (int dx = -1; dx <= 1 && isFlat; dx++) {
					for (int dz = -1; dz <= 1 && isFlat; dz++) {
						float neighborHeight = noise.GetNoise(
							(globalX + dx) / 100.0f,
							(globalZ + dz) / 100.0f
						) * 20.0f + 64.0f;
						if (abs(neighborHeight - static_cast<float>(terrainHeight)) > 1.5f) {
							isFlat = false;
						}
					}
				}
				if (!isFlat) continue;

				// Get tree noise value
				float treeValue = treeNoise.GetNoise(static_cast<float>(globalX), static_cast<float>(globalZ));
				if (treeValue > 0.45f && Random::GetRandomFloat() < 0.3f) {
					GenerateTree(localX, terrainHeight, localZ);
				}
			}
		}
	}

	void Chunk::BuildMesh()
	{
		const Vector3 faceVertices[6][4] = {
			// +X
			{{1,0,0}, {1,1,0}, {1,1,1}, {1,0,1}},
			// -X
			{{0,0,1}, {0,1,1}, {0,1,0}, {0,0,0}},
			// +Y
			{{0,1,1}, {1,1,1}, {1,1,0}, {0,1,0}},
			// -Y
			{{0,0,0}, {1,0,0}, {1,0,1}, {0,0,1}},
			// +Z
			{{0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}},
			// -Z
			{{1,0,0}, {0,0,0}, {0,1,0}, {1,1,0}},
		};

		const Vector3 faceNormals[6] = {
			{ 1, 0, 0}, {-1, 0, 0}, { 0, 1, 0},
			{ 0,-1, 0}, { 0, 0, 1}, { 0, 0,-1}
		};

		std::vector<Render::Vertex> vertices;
		std::vector<uint32_t> indices;
		uint32_t vertexOffset = 0;

		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			for (int y = 0; y < CHUNK_HEIGHT; ++y)
			{
				for (int z = 0; z < CHUNK_SIZE; ++z)
				{
					EBlockType block = m_Blocks[Index(x, y, z)];
					if (block == EBlockType::Air)
						continue;

					Vector3 blockPos = {
						static_cast<float>(x),
						static_cast<float>(y),
						static_cast<float>(z)
					};

					for (int face = 0; face < 6; ++face)
					{
						// Neighbor offset
						int nx = x + static_cast<int>(faceNormals[face].X);
						int ny = y + static_cast<int>(faceNormals[face].Y);
						int nz = z + static_cast<int>(faceNormals[face].Z);

						if (!IsBlockVisible(nx, ny, nz))
							continue;

						// Add face
						for (int i = 0; i < 4; ++i)
						{
							Vector3 val = blockPos + faceVertices[face][i];
							Render::Vertex vert;
							vert.Position = { val.X, val.Y, val.Z };
							vert.Normal = { faceNormals[face].X, faceNormals[face].Y, faceNormals[face].Z };

							switch (block)
							{
							case EBlockType::Dirt:
								vert.Colour = { Brown.X, Brown.Y, Brown.Z, Brown.W };
								break;
							case EBlockType::Grass:
								vert.Colour = { 0.1f, 0.9f, 0.4f, 1.0f };
								break;
							case EBlockType::Stone:
								vert.Colour = { Gray.X, Gray.Y, Gray.Z, Gray.W };
								break;
							case EBlockType::Water:
								vert.Colour = { Blue.X, Blue.Y, Blue.Z, 0.25f }; //TODO: Transparency
								break;
							case EBlockType::Leaves:
								vert.Colour = { Green.X, Green.Y, Green.Z, Green.W };
								break;
							case EBlockType::Bark:
								vert.Colour = { Maroon.X, Maroon.Y, Maroon.Z, Maroon.W };
								break;
							case EBlockType::Wood:
								vert.Colour = { Maroon.X, Maroon.Y, Maroon.Z, Maroon.W };
								break;
							default:
								vert.Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
							}
							vertices.push_back(vert);
						}

						indices.push_back(vertexOffset + 0);
						indices.push_back(vertexOffset + 1);
						indices.push_back(vertexOffset + 2);
						indices.push_back(vertexOffset + 0);
						indices.push_back(vertexOffset + 2);
						indices.push_back(vertexOffset + 3);
						vertexOffset += 4;
					}
				}
			}
		}

		m_Mesh = Render::IMesh::Create();
		m_Mesh->Init(vertices, indices);
	}

	bool Chunk::IsMeshValid() const
	{
		return m_Mesh != nullptr;
	}

	bool Chunk::Raycast(const Vector3& origin, const Vector3& direction, float maxDistance, Vector3& outHitPos, Vector3& outBlockPos)
	{
		Vector3 pos = origin;
		Vector3 step = direction.Normalize() * 0.1f; // smaller step for precision

		for (float dist = 0; dist < maxDistance; dist += 0.1f)
		{
			int bx = static_cast<int>(floor(pos.X));
			int by = static_cast<int>(floor(pos.Y));
			int bz = static_cast<int>(floor(pos.Z));

			if (bx >= 0 && bx < CHUNK_SIZE &&
				by >= 0 && by < CHUNK_HEIGHT &&
				bz >= 0 && bz < CHUNK_SIZE)
			{
				if (m_Blocks[Index(bx, by, bz)] != EBlockType::Air)
				{
					outHitPos = pos;
					outBlockPos = Vector3{ (float)bx, (float)by, (float)bz };
					return true;
				}
			}

			//TODO: Make a plus equals operator for Vector class types
			pos.X += step.X;
			pos.Y += step.Y;
			pos.Z += step.Z;
		}

		return false;
	}

	void Chunk::DestroyBlock(int x, int y, int z)
	{
		if (x >= 0 && x < CHUNK_SIZE &&
			y >= 0 && y < CHUNK_HEIGHT &&
			z >= 0 && z < CHUNK_SIZE)
		{
			m_Blocks[Index(x, y, z)] = EBlockType::Air;
			BuildMesh();
		}
	}

	EBlockType Chunk::GetBlock(int x, int y, int z) const
	{
		int index = Index(x, y, z);

		if (m_Blocks.size() >= index || m_Blocks.size() < index)
		{
			return EBlockType::None;
		}

		return m_Blocks[Index(x, y, z)];
	}

	void Chunk::GenerateTree(int x, int y, int z)
	{
		// Trunk generation
		const int trunkHeight = 4 + Random::GetRandomIntInRange(0, 2); // 4-6 blocks

		// Generate trunk
		for (int ty = 0; ty < trunkHeight; ty++) {
			if (IsInBounds(x, y + ty, z)) {
				m_Blocks[Index(x, y + ty, z)] = EBlockType::Bark;
			}
		}

		// Leaf generation parameters
		const int leafBottom = y + trunkHeight - 2;
		const int leafTop = y + trunkHeight + 1;
		const Vector3 center(x + 0.5f, leafBottom + 2.5f, z + 0.5f);

		// Generate leaf sphere
		for (int ly = leafBottom; ly <= leafTop; ly++) {
			for (int lx = x - 2; lx <= x + 2; lx++) {
				for (int lz = z - 2; lz <= z + 2; lz++) {
					// Skip trunk area
					if (lx == x && lz == z) continue;

					// Calculate distance from center
					float dx = (lx - center.X);
					float dy = (ly - center.Y) * 0.8f; // Vertical compression
					float dz = (lz - center.Z);
					float dist = sqrt(dx * dx + dy * dy + dz * dz);

					// Shape thresholds
					if (dist < 2.0f + Random::GetRandomFloat() * 0.5f) {
						if (IsInBounds(lx, ly, lz) &&
							m_Blocks[Index(lx, ly, lz)] == EBlockType::Air) {
							m_Blocks[Index(lx, ly, lz)] = EBlockType::Leaves;
						}
					}
				}
			}
		}

		// Add some random leaves outside main sphere
		for (int i = 0; i < 8; i++) {
			int lx = x + Random::GetRandomIntInRange(-3, 3);
			int lz = z + Random::GetRandomIntInRange(-3, 3);
			int ly = leafBottom + Random::GetRandomIntInRange(0, 3);

			if (IsInBounds(lx, ly, lz) &&
				m_Blocks[Index(lx, ly, lz)] == EBlockType::Air) {
				m_Blocks[Index(lx, ly, lz)] = EBlockType::Leaves;
			}
		}
	}

	bool Chunk::IsInBounds(int x, int y, int z)
	{
		return x >= 0 && x < CHUNK_SIZE &&
			y >= 0 && y < CHUNK_HEIGHT &&
			z >= 0 && z < CHUNK_SIZE;
	}

	bool Chunk::IsBlockVisible(int x, int y, int z) const
	{
		if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_SIZE)
			return true; // Assume outside blocks are air

		return m_Blocks[Index(x, y, z)] == EBlockType::Air;
	}

}//namespace Engine
