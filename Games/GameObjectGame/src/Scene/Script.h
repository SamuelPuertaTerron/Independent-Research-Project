/* The main script class for scripting gameplay functionality. Similar to  in Unity MonoBehaviour */
#pragma once

namespace GameObject
{
	class Script : public ScriptComponent
	{
	public:
		Script() = default;
		virtual ~Script() override = default;

		virtual bool OnInit() { return false; }
		virtual void OnTick(float dt) {}
		virtual void OnDestroy()	  {}

		virtual bool Init() override;
		virtual void Tick(float dt) override;
		virtual void Destroy() override;
	};

}//namespace GameObject