/* The component used to render objects to the world */
#pragma once
#include "Scene/IComponent.h"

namespace GameObject
{
	class RenderComponent : public IComponent
	{
	public:
		RenderComponent() = default;
		virtual ~RenderComponent() override = default;

		virtual bool Init() override;
		virtual void Tick(float dt) override;
		virtual void Destroy() override;
	};
}//namespace GameObject