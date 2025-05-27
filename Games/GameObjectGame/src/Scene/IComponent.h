/* The interface for Components */
#pragma once

namespace GameObject
{
	class IComponent
	{
	public:
		virtual ~IComponent() = default;

		virtual bool Init() = 0;
		virtual void Tick(float dt) = 0;
		virtual void Destroy() = 0;

		void SetObject(GameObject* object)
		{
			m_Object = object;
		}

		GameObject* GetParentObject() const
		{
			if (!m_Object)
			{
				return nullptr;
			}

			return m_Object;
		}

		bool GetShouldUpdate() const { return m_bShouldUpdate; }

	protected:
		GameObject* m_Object{nullptr};
		bool m_bShouldUpdate{true};
	};
}//namespace GameObject