/* The main script component for running gameplay logic on game objects */
#pragma once

namespace GameObject
{
	/* Callbacks for code */

	using InitFunc = std::function<bool()>;
	using TickFunc = std::function<void(float)>;
	using DestroyFunc = std::function<void()>;

	class ScriptComponent : public IComponent
	{
	public:
		ScriptComponent() = default;
		virtual ~ScriptComponent() override = default;

		virtual bool Init() override;
		virtual void Tick(float dt) override;
		virtual void Destroy() override;

		void SetInitFunc(const InitFunc& func);
		void SetTickFunc(const TickFunc& func);
		void SetDestroyFunc(const DestroyFunc& func);

	private:
		InitFunc m_InitFunc;
		TickFunc m_TickFunc;
		DestroyFunc m_DestroyFunc;
	};
}//namespace GameObject