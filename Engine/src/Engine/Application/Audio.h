#pragma once

#include "../../ThirdParty/miniaudio/miniaudio/miniaudio.h"

namespace Engine
{
	class Audio : public TSingleton<Audio>
	{
	public:
		void Init();
		void Destroy();

		void Play(const std::filesystem::path& path, bool loop = true);
		void Stop();

		void UpdateVolume(float volume);
	private:
		ma_engine m_Engine;
		ma_sound m_Sound;
	};
}//namespace Engine