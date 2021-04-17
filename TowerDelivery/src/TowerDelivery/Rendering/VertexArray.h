#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>

namespace TowerDelivery {

	/*!
 * Stores all data for a geometry object
 */
	struct VertexData {
		/*!
		 * Vertex positions
		 */
		std::vector<glm::vec3> positions;
		/*!
		 * Geometry indices
		 */
		std::vector<unsigned int> indices;
		/*!
		 * Vertex normals
		 */
		std::vector<glm::vec3> normals;
		/*!
		 * Vertex UV coordinates
		 */
		std::vector<glm::vec2> uvs;
	};


	class VertexArray {
	public:
		VertexArray(VertexData data);
		~VertexArray();

		void draw();
		static VertexData createCubeVertexArray(float width, float height, float depth);

	protected:
		GLuint vao;
		GLuint vboPositions;
		GLuint vboNormals;
		GLuint _vboUVs;
		GLuint vboIndices;
		unsigned int elements;
	};

}


