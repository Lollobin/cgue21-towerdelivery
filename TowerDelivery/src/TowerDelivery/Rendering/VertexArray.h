#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"


namespace TowerDelivery {

	class VertexArray {
	public:
		VertexArray(std::vector<float> positions, std::vector<unsigned int> indices, std::vector<float> normals);
		~VertexArray();
		void draw();

	protected:
		GLuint _vao;
		GLuint _vboPositions;
		GLuint _vboNormals;
		//GLuint _vboUVs;
		GLuint _vboIndices;
		unsigned int _elements;
	};

}


