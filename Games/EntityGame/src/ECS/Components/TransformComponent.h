/* A basic Transform Component with Position, Rotation and Scale */
#pragma once

namespace EntityGame
{
	struct TransformComponent : IComponent
	{
		Engine::Vector3 Position{0, 0, 0};
		Engine::Vector3 Rotation{0, 0, 0};
		Engine::Vector3 Scale{1, 1, 1};
	};
}//namespace EntityGame 