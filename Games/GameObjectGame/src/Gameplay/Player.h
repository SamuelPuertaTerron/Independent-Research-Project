/* The main player object */
#pragma once

namespace GameObject::Gameplay
{
	class PlayerScript : public Script
	{
	public:
		PlayerScript() = default;
		virtual ~PlayerScript() override = default;

		virtual bool OnInit() override;
		virtual void OnTick(float dt) override;
		virtual void OnDestroy() override;

		void HandleMovement(float dt);
	private:
		std::shared_ptr<TransformComponent> m_TransformComponent;
		std::shared_ptr<ColliderComponent> m_ColliderComponent;

		std::vector<std::shared_ptr<ChunkRenderComponent>> m_Chunks;

		Vector3 m_CameraPosition;
		Vector3 m_CameraRotation;

		Vector3 m_Size;
		Vector3 m_Velocity{ 0, 0, 0 };
		bool m_bIsGrounded = false;

	};
}//namespace GameObject::Gameplay