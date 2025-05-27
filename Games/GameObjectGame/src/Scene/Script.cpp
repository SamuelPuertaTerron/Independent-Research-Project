#include "Game.h"
#include "Script.h"


namespace GameObject
{
	bool Script::Init()
	{
		SetInitFunc([this]()
		{
			return this->OnInit();
		});
		SetTickFunc([this](float dt)
		{
			this->OnTick(dt);
		});
		SetDestroyFunc([this]()
		{
			this->OnDestroy();
		});

		return ScriptComponent::Init();
	}
	void Script::Tick(float dt)
	{
		ScriptComponent::Tick(dt);
	}
	void Script::Destroy()
	{
		ScriptComponent::Destroy();
	}
}//namespace GameObject