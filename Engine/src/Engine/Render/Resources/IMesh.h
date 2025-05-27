/* The base mesh */
#pragma once
namespace Engine::Render
{
	struct Vertex;

	class IMesh
	{
	public:
		IMesh() = default;
		virtual ~IMesh() = default;

		virtual bool Init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices = {}) { return true; }
		virtual void Render() {}            
		virtual void Destroy() {}

		static std::shared_ptr<IMesh> Create();

	protected:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices; //This could possibly be null
	};
}//namespace Engine