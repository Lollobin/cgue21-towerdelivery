#pragma once
#include <string>

namespace TowerDelivery {

	class Shader {
	public:
		Shader(const char* vertexSrc, const char* fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID;
	};
}