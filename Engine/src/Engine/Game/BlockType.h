/* An enum to represent each block type i.e. dirt, stone etc */
#pragma once

namespace Engine
{
	enum class EBlockType
	{
		None,
		Air,
		Grass,
		Dirt,
		Stone,
		Water,
		Leaves,
		Bark,
		Wood,
	};

	static std::string BlockTypeToString(EBlockType type)
	{
		switch (type)
		{
		case EBlockType::None:
				break;
		case EBlockType::Air:
			return "Air";
		case EBlockType::Grass:
			return "Grass";
		case EBlockType::Dirt:
			return "Dirt";
		case EBlockType::Stone:
			return "Stone";
		case EBlockType::Water:
			return "Water";
		case EBlockType::Leaves:
			return "Leaves";
		case EBlockType::Bark:
			return "Bark";
		case EBlockType::Wood:
			return "Wood";
		}

		return "BLOCK_NULL";
	}
}//namespace Engine