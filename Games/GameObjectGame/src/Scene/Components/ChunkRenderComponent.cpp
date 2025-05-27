#include "Game.h"
#include "ChunkRenderComponent.h"

namespace GameObject
{
	bool ChunkRenderComponent::Init()
	{
		auto transform = GetParentObject()->GetComponent<TransformComponent>(typeid(TransformComponent));
		Generate(transform->GetPosition());
		BuildMesh();

		if (!IsMeshValid())
		{
			return false;
		}

		m_bShouldUpdate = false;

		return true;
	}
}//namespace GameObject