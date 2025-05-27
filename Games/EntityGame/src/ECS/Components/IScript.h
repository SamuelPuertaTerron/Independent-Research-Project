/* The main script, acts similar to a system. Attached to the Script Component */
#pragma once

namespace EntityGame
{
	class Scene;

	class IScript
	{
	public:
		IScript(Entity entity, Scene* scene)
			: m_Entity(entity), m_Scene(scene) {}

		virtual ~IScript() = default;

		virtual void OnInit()		  {}
		virtual void OnTick(float dt) {}
	protected:
		Entity m_Entity;
		Scene* m_Scene;
	};
}//namespace EntityGame