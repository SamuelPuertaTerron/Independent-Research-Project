/* A generic Vertex struct. Contains data for position, colour, texture etc. */
#pragma once

namespace Engine::Render
{
	struct Vertex
	{
		Vector3 Position;
		Vector3 Normal;
		Colour Colour;
	};

}//namespace Engine::Render