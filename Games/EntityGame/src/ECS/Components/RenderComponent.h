/* A render component */
#pragma once

namespace EntityGame
{
	struct RenderComponent : IComponent
	{
		std::shared_ptr<Engine::Render::IMesh> Mesh;
		Engine::Colour Colour = Engine::Red;
	};
}//namespace EntityGame