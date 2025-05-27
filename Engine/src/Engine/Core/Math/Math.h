/* The main header file for math functionality */
#pragma once

#include "TVector.h"
#include "../../../../../ThirdParty/glm/glm/fwd.hpp"
#include "Engine/Render/RenderContextAPI.h"

namespace DirectX
{
	struct XMFLOAT3;
}

namespace DirectX
{
	struct XMMATRIX;
}//namespace DirectX

namespace Engine
{
	using Vector2Int = Math::TVector<int, 2>;

	using Vector2 = Math::TVector<float, 2>;
	using Vector3 = Math::TVector<float, 3>;

	using Vector4 = Math::TVector<float, 4>;
	using Colour = Math::TVector<float, 4>;

	inline Colour Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	inline Colour White = { 1.0f, 1.0f, 1.0f, 1.0f };
	inline Colour Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	inline Colour Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	inline Colour Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	inline Colour Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	inline Colour Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	inline Colour Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	inline Colour Gray = { 0.5f, 0.5f, 0.5f, 1.0f };
	inline Colour DarkGray = { 0.2f, 0.2f, 0.2f, 1.0f };
	inline Colour LightGray = { 0.8f, 0.8f, 0.8f, 1.0f };
	inline Colour Orange = { 1.0f, 0.65f, 0.0f, 1.0f };
	inline Colour Purple = { 0.5f, 0.0f, 0.5f, 1.0f };
	inline Colour Pink = { 1.0f, 0.75f, 0.8f, 1.0f };
	inline Colour Brown = { 0.6f, 0.4f, 0.2f, 1.0f };
	inline Colour Transparent = { 0.0f, 0.0f, 0.0f, 0.0f }; // Fully transparent
	inline Colour HalfTransparentWhite = { 1.0f, 1.0f, 1.0f, 0.5f };
	inline Colour HalfTransparentBlack = { 0.0f, 0.0f, 0.0f, 0.5f };
	inline Colour Lime = { 0.75f, 1.0f, 0.0f, 1.0f };
	inline Colour Teal = { 0.0f, 0.5f, 0.5f, 1.0f };
	inline Colour Lavender = { 0.9f, 0.9f, 1.0f, 1.0f };
	inline Colour Maroon = { 0.5f, 0.0f, 0.0f, 1.0f };
	inline Colour Olive = { 0.5f, 0.5f, 0.0f, 1.0f };
	inline Colour Navy = { 0.0f, 0.0f, 0.5f, 1.0f };
	inline Colour Coral = { 1.0f, 0.5f, 0.3f, 1.0f };
	inline Colour Salmon = { 0.98f, 0.5f, 0.45f, 1.0f };
	inline Colour Gold = { 1.0f, 0.84f, 0.0f, 1.0f };
	inline Colour Silver = { 0.75f, 0.75f, 0.75f, 1.0f };

	class Random
	{
	public:
		/* Returns a float value between 0.0f (inclusive) and 1.0f (exclusive) */
		static float GetRandomFloat()
		{
			std::uniform_real_distribution<float> dist(0.0f, 1.0f);
			return dist(engine);
		}

		/* Returns an int value between min (inclusive) and max (inclusive) */
		static int32_t GetRandomIntInRange(int32_t min, int32_t max)
		{
			if (min > max) std::swap(min, max);
			std::uniform_int_distribution<int32_t> dist(min, max);
			return dist(engine);
		}
	private:
		//For thread safety
		static inline std::mt19937 engine = []() {
			std::random_device rd;
			return std::mt19937(rd());
			}();

	};

}//namespace Engine