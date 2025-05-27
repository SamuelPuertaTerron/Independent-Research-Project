/* A template singleton class */
#pragma once

namespace Engine
{
	template<typename TClass>
	class TSingleton
	{
	public:
		/* Gets the singleton instance */
		static TClass& GetInstance()
		{
			if (!s_pInstance)
			{
				s_pInstance = new TClass();
			}

			return *s_pInstance;
		}

		/* Cleanups the singleton instance */
		static void Release()
		{
			delete s_pInstance;
		}

	protected:
		virtual ~TSingleton() = default;
	protected:
		inline static TClass* s_pInstance;
	};
}//namespace Engine