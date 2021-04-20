#include "tdpch.h"

#include <assimp/scene.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <TowerDelivery/Rendering/Mesh.h>
#include <TowerDelivery/Rendering/Shader.h>

//Model Class following the tutorial from LearnOpenGL https://learnopengl.com/Model-Loading/Model
namespace TowerDelivery {
    class Model
    {

    public:
        // model data 
        vector<Texture> textures_loaded; // stores all the textures loaded so far, do that textures aren't loaded more than once.
        vector<Mesh> meshes;
        string directory; //to load textures from the same directories as the model.obj file
        bool gammaCorrection;

    public:
        Model(string const& path);
        void Draw(Shader& shader);

    private:
        void loadModel(string const& path);
        void processNode(aiNode* node, const aiScene* scene);

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    };
}