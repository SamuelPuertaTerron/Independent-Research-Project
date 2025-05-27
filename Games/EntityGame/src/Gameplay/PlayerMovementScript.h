/* The player movement script. Handles player input and transformations */
#pragma once

namespace EntityGame
{
	class PlayerMovementScript : public IScript
	{
	public:
		PlayerMovementScript(Entity entity, Scene* scene)
			: IScript(entity, scene) {}

		virtual ~PlayerMovementScript() override = default;

		virtual void OnInit() override;
		virtual void OnTick(float dt) override;
	};
}//namespace EntityGame