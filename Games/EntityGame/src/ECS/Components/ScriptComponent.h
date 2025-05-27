#pragma once

namespace EntityGame
{
	struct ScriptComponent : IComponent
	{
		std::unique_ptr<IScript> Script;
	};
}//namespace EntityGame 