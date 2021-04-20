#pragma once

#include "tdpch.h"
#include "TowerDelivery/Rendering/Shader.h"

using namespace std;

//Mesh class following the tutorial from LearnOpenGL https://learnopengl.com/Model-Loading/Mesh
namespace TowerDelivery {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct Texture {
		unsigned int id;
		string type;
		string path;
	};

    class Mesh {
    public:
        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        vector<Texture>      textures;
        unsigned int VAO;

    public:
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void Draw(Shader& shader);

    private:
        unsigned int VBO, EBO;
        void setupMesh();
    };
}