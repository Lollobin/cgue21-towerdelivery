#include "tdpch.h"
#include "VertexArray.h"

namespace TowerDelivery {

	VertexArray::VertexArray(std::vector<glm::vec3> positions, std::vector<unsigned int> indices,
		std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs)
		: _elements(indices.size())
	{
		
		// create VAO
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		// create positions VBO
		glGenBuffers(1, &_vboPositions);
		glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

		// bind positions to location 0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// create normals VBO
		glGenBuffers(1, &_vboNormals);
		glBindBuffer(GL_ARRAY_BUFFER, _vboNormals);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

		// bind normals to location 1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// create uvs VBO
		glGenBuffers(1, &_vboUVs);
		glBindBuffer(GL_ARRAY_BUFFER, _vboUVs);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

		// bind uvs to location 2
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// create and bind indices VBO
		glGenBuffers(1, &_vboIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	VertexArray::~VertexArray() {
		glDeleteBuffers(1, &_vboPositions);
		glDeleteBuffers(1, &_vboNormals);
		glDeleteBuffers(1, &_vboUVs);
		glDeleteBuffers(1, &_vboIndices);
		glDeleteVertexArrays(1, &_vao);
	}

	void VertexArray::draw() {
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _elements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

}
