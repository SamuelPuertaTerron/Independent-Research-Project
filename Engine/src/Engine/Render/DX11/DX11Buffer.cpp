#include "Engine.h"
#include "DX11Buffer.h"

namespace Engine::Render::DX11
{
	void DX11ConstantBuffer::Destroy() const
	{
		if (m_pBuffer)
		{
			m_pBuffer->Release();
		}
	}
}//namespace Engine::Render::DX11