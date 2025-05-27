#include "Engine.h"
#include "Audio.h"

namespace Engine
{
	void Audio::Init()
	{
		ma_result result = ma_engine_init(nullptr, &m_Engine);
		if (result != MA_SUCCESS)
		{
			Logger::Log("Failed to Cretae Audio Engine", ELogLevel::Error);
		}
	}
	void Audio::Destroy()
	{
		ma_sound_uninit(&m_Sound);
		ma_engine_uninit(&m_Engine);
	}
	void Audio::Play(const std::filesystem::path& path, bool loop)
	{
		ma_result result = ma_sound_init_from_file(&m_Engine, path.string().c_str(), 0, nullptr, nullptr, &m_Sound);
		if (result != MA_SUCCESS)
		{
			Logger::Log("Failed to Cretae sound", ELogLevel::Error);
		}

		ma_sound_set_looping(&m_Sound, loop);
		ma_sound_start(&m_Sound);
	}

	void Audio::Stop()
	{
		ma_sound_stop(&m_Sound);
	}
	void Audio::UpdateVolume(float volume)
	{
		ma_sound_set_volume(&m_Sound, volume);
	}
}//namespace Engine