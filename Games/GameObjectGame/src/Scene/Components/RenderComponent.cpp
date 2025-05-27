#include "Game.h"
#include "RenderComponent.h"

namespace GameObject
{
	bool RenderComponent::Init()
	{
		return false;
	}
	void RenderComponent::Tick(float dt)
	{
		auto transform = GetParentObject()->GetComponent<TransformComponent>(typeid(TransformComponent));
		auto position = transform->GetPosition();
		Engine::Renderer::Draw(position, { 0, 0, 0 }, { 1, 1, 1 }, Engine::Red);
	}
	void RenderComponent::Destroy()
	{
	}
}//namespace GameObject