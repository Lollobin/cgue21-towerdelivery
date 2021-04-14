#include "tdpch.h"
#include "VertexArray.h"

namespace TowerDelivery {
	VertexArray::VertexArray(VertexData data)
		: elements(data.indices.size())
	{
		// create VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// create positions VBO
		glGenBuffers(1, &vboPositions);
		glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
		glBufferData(GL_ARRAY_BUFFER, data.positions.size() * sizeof(glm::vec3), data.positions.data(), GL_STATIC_DRAW);

		// bind positions to location 0
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// create normals VBO
		glGenBuffers(1, &vboNormals);
		glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
		glBufferData(GL_ARRAY_BUFFER, data.normals.size() * sizeof(glm::vec3), data.normals.data(), GL_STATIC_DRAW);

		// bind normals to location 1
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// create uvs VBO
		//glGenBuffers(1, &_vboUVs);
		//glBindBuffer(GL_ARRAY_BUFFER, _vboUVs);
		//glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

		// bind uvs to location 2
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// create and bind indices VBO
		glGenBuffers(1, &vboIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	VertexArray::~VertexArray() {
		glDeleteBuffers(1, &vboPositions);
		glDeleteBuffers(1, &vboNormals);
		//glDeleteBuffers(1, &_vboUVs);
		glDeleteBuffers(1, &vboIndices);
		glDeleteVertexArrays(1, &vao);
	}

	void VertexArray::draw() {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	VertexData VertexArray::createCubeVertexArray(float width, float height, float depth)
	{
		VertexData data;

		data.positions = {
			// front
			glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
			glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
			glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
			glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
			// back
			glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
			glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
			glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
			glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
			// right
			glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
			glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
			glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
			glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
			// left
			glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
			glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
			glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
			glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
			// top
			glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
			glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
			glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
			glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
			// bottom
			glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
			glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
			glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
			glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f)
		};

		data.normals = {
			// front
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),
			glm::vec3(0, 0, 1),
			// back
			glm::vec3(0, 0, -1),
			glm::vec3(0, 0, -1),
			glm::vec3(0, 0, -1),
			glm::vec3(0, 0, -1),
			// right
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			glm::vec3(1, 0, 0),
			// left
			glm::vec3(-1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec3(-1, 0, 0),
			glm::vec3(-1, 0, 0),
			// top
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 1, 0),
			// bottom
			glm::vec3(0, -1, 0),
			glm::vec3(0, -1, 0),
			glm::vec3(0, -1, 0),
			glm::vec3(0, -1, 0)
		};

		/*
		data.uvs = {
			// front
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1),
			// back
			glm::vec2(1, 1),
			glm::vec2(0, 1),
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			// right
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1),
			// left
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1),
			// top
			glm::vec2(0, 1),
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			// bottom
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(1, 1),
			glm::vec2(0, 1)
		};
		*/
		data.indices = {
			// front
			0, 1, 2,
			2, 3, 0,
			// back
			4, 5, 6,
			6, 7, 4,
			// right
			8, 9, 10,
			10, 11, 8,
			// left
			12, 13, 14,
			14, 15, 12,
			// top
			16, 17, 18,
			18, 19, 16,
			// bottom
			20, 21, 22,
			22, 23, 20
		};

		return std::move(data);
	}
}