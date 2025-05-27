/* The base struct for components to inherit from */
#pragma once

namespace EntityGame
{
	struct IComponent
	{
		virtual ~IComponent() = default;
	};
}//namespace EntityGame 