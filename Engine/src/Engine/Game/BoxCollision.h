/* An Axis-Aligned Bounding Box */
#pragma once

namespace Engine
{
	class BoxCollision
	{
	public:
		BoxCollision() = default;
		~BoxCollision() = default;

		void SetSize(const Vector3& min, const Vector3& max);

		bool Intersects(const BoxCollision& other) const;
		Vector3 GetPenetration(const BoxCollision& other) const;
	protected:
		Vector3 m_Min, m_Max;
	};
}//namespace Engine