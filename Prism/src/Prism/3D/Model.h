#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Prism/Core/Core.h"
#include "Prism/3D/Mesh.h"
#include "Prism/Renderer/Shader.h"
#include "Prism/Renderer/Texture.h"

#include "assimp/Importer.hpp";
#include "assimp/scene.h";
#include "assimp/postprocess.h";


/* Model
- A model contains multiple meshes, posssibly with multiple textures. It could be a house that consists of a wooden balcony, a tower and perhaps a swimming pool that is still loaded with a single model.
- We will load the model via Assimp and translate it to multiple Mesh objects with our Mesh class.
*/

namespace Prism
{
	class Model
	{
	public:
		Model() = default;
		Model(const std::string& filePath)
		{
			LoadModel(filePath);
		}

		//Within here, we use Assimp the load the model into a data structure of Assimp called a scene object. This is the root object of Assimp's data interface.
		void LoadModel(const std::string& filePath);

		void Draw(Reference<Shader>& shader); //This loops over each mesh and calls their respective Draw functions.

	private:
		//The root node of an Assimp scene may contain child nodes (like other nodes) and could have a set of indices that point to mesh data in the scene object's mMeshes array.
		//The scene's mMeshes array contains the actual Mesh objects, the values in the mMeshes array of a node are only indices for the scene's meshes array. 
		//ProcessNode will process each node in the scene recursively.
		void ProcessNode(aiNode* node, const aiScene* scene);  

		//The mesh object itself contains all the relevant data required for rendering. Think of vertex positions, normal vectors, texture coordinates, faces and the material of the object.
		//ProcessMesh will translate an aiMesh object to a mesh object of our own.
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<ModelTexture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);

	private:
		//Model Data
		std::vector<ModelTexture> m_TexturesLoaded; //Stores all the texture loaded so far. This is an optimization to make sure textures aren't loaded more thanm once.
		std::vector<Mesh> m_Meshes;
		std::string m_FilePath;
	};
}