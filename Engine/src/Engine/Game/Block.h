/* The block that is used inside chunks. */
/* Some blocks might contain functionality, so that is why the TickFunc is present */
/* The Renderer batches all blocks within a chunk and then renders that in one draw call. */
#pragma once

namespace Engine
{
	enum class EBlockType;

	using TickFunc = std::function<void(float)>;

	struct Block
	{
		EBlockType BlockType;
	};
}//namespace Engine