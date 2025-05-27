#include "Engine.h"
#include "BoxCollision.h"

namespace Engine
{
	void BoxCollision::SetSize(const Vector3& min, const Vector3& max)
	{
		m_Min = min;
		m_Max = max;
	}
	bool BoxCollision::Intersects(const BoxCollision& other) const
	{
		return (m_Min.X < other.m_Max.X && m_Max.X > other.m_Min.X) &&
			(m_Min.Y < other.m_Max.Y && m_Max.Y > other.m_Min.Y) &&
			(m_Min.Z < other.m_Max.Z && m_Max.Z > other.m_Min.Z);
	}
	Vector3 BoxCollision::GetPenetration(const BoxCollision& other) const
	{
		Vector3 penetration;
		penetration.X = (m_Min.X < other.m_Min.X) ?
			other.m_Min.X - m_Max.X : m_Min.X - other.m_Max.X;
		penetration.Y = (m_Min.Y < other.m_Min.Y) ?
			other.m_Min.Y - m_Max.Y : m_Min.Y - other.m_Max.Y;
		penetration.Z = (m_Min.Z < other.m_Min.Z) ?
			other.m_Min.Z - m_Max.Z : m_Min.Z - other.m_Max.Z;
		return penetration;
	}
}//namespace Engine