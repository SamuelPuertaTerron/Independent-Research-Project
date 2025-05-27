#include "Game.h"
#include "ScriptComponent.h"

namespace GameObject
{
	bool ScriptComponent::Init()
	{
		if (m_InitFunc != nullptr)
		{
			return m_InitFunc();
		}

		/* Note: This will still run the tick and destroy functions, even if this returns false */
		return false;
	}

	void ScriptComponent::Tick(float dt)
	{
		if (m_TickFunc != nullptr)
		{
			m_TickFunc(dt);
		}
	}

	void ScriptComponent::Destroy()
	{
		if (m_DestroyFunc != nullptr)
		{
			m_DestroyFunc();
		}
	}

	void ScriptComponent::SetInitFunc(const InitFunc& func)
	{
		m_InitFunc = func;
	}
	void ScriptComponent::SetTickFunc(const TickFunc& func)
	{
		m_TickFunc = func;
	}
	void ScriptComponent::SetDestroyFunc(const DestroyFunc& func)
	{
		m_DestroyFunc = func;
	}
}//namespace GameObject