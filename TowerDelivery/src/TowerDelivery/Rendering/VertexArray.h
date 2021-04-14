#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>

namespace TowerDelivery {

	class VertexArray {
	public:
		VertexArray(std::vector<float> positions, std::vector<unsigned int> indices, std::vector<float> normals);
		~VertexArray();
		void draw();

	protected:
		GLuint vao;
		GLuint vboPositions;
		GLuint vboNormals;
		//GLuint _vboUVs;
		GLuint vboIndices;
		unsigned int elements;
	};

}


