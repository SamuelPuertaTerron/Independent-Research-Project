/* A transform component containing Position, Rotation and Scale values */

#pragma once

namespace GameObject
{
	class TransformComponent : public IComponent
	{
	public:
		TransformComponent() = default;
		virtual ~TransformComponent() override = default;

		virtual bool Init() override;
		virtual void Tick(float dt) override;
		virtual void Destroy() override;

		Engine::Vector3 GetPosition() const;
		void SetPosition(const Engine::Vector3& position);
	private:
		Engine::Vector3 m_Position;
	};
}//namespace GameObject
