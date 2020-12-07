#include "PrismPrecompiledHeader.h"
#include "Model.h"

namespace Prism
{
	void Model::Draw(Reference<Shader>& shader)
	{
		for (size_t i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i].DrawMesh(shader);
		}
	}

	void Model::LoadModel(const std::string& filePath) 
	{
		//Declares the importer from Assimp.
		Assimp::Importer importer; 

		//ReadFile() expects a path and several post-processing options as its second argument. These options allow us to specify several options that forces Assimp to do extra calculations
		//operations on the imported data. aiProcess_Triangulate tells Assimp that if the model does not (entirely) consist of triangles, it should transform all of the model's primitive shapes to triangles first.
		//The aiProcess_FlipUVs flip the texture coordinates on the Y-Axis where necessary during processing. This is useful for OpenGL as most of its images are reversed around the Y-Axis.
		const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs); 

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) //If our model can't be loaded.
		{
			PRISM_CLIENT_ERROR("Assimp Error: {0} \n", importer.GetErrorString());
			return;
		}

		m_FilePath = filePath.substr(0, filePath.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		//We pass the root node here. As each node could possibly contain a set of children, we want to first process the node in question and then continue processing all the node's children.
		//This fits a recursive structure.

		//Process all the node's meshes, if any. Remember that each node contains a set of mesh indices, where each index points to a specific messh located in the scene object.
		//We thus want to retrieve these mesh indices, obtain each mesh, process each mesh and then do this all again for each of the node's children nodes.
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; //Retrieve the mesh.
			m_Meshes.push_back(ProcessMesh(mesh, scene)); //Stores the retrieved mesh object.
		}

		//And then do the same for the children.
		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}

		//Once a node no longer has any children, the recursion stops.
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<ModelVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<ModelTexture> textures;

		//Process a mesh is a 3-part process. We retrieve all the vertex data, retrieve the mesh's indices and finally retrieve the relevant material data.
		//The processed data is stored in one of the 3 vectors and from those, a Mesh is created and returned to the function's caller.

		//Walk through each of the mesh's vertices and process their positions, normals and texture coordinates.
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			ModelVertex vertex;
			glm::vec3 placeholderVector; //Placeholder vector as Assimp uses its own vector class that does not directly convert to glm::vec3's class. 

			//Positions
			placeholderVector.x = mesh->mVertices[i].x;
			placeholderVector.y = mesh->mVertices[i].y;
			placeholderVector.z = mesh->mVertices[i].z;
			vertex.Position = placeholderVector;

			//Normals
			if (mesh->HasNormals())
			{
				placeholderVector.x = mesh->mNormals[i].x;
				placeholderVector.y = mesh->mNormals[i].y;
				placeholderVector.z = mesh->mNormals[i].z;
				vertex.Normal = placeholderVector;
			}

			//Texture Coordinates
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 textureVector;
				textureVector.x = mesh->mTextureCoords[0][i].x;
				textureVector.y = mesh->mTextureCoords[0][i].y;
				vertex.TextureCoordinates = textureVector;
			}
			else
			{
				vertex.TextureCoordinates = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		//Now, walk through each of the mesh's faces and retrieve the corresponding vertex indices. Each face represents a single primitive, which in our case is always triangles.
		//A face contains the indices of the vertices we need to draw in what order for its primitive. 
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			//Retrieve all indices of the face and store them in the indices vector.
			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		//With this, we now have a complete set of vertices and index data for drawing the mesh via glDrawElements.

#if 0
		//Process Materials
		//Similar to nodes, a Mesh only contains an index to a material object. To retrieve the material of a mesh, we need to index the scene's mMaterials array.
		//The mesh's material index is set in its mMaterialIndex property, which we can also query to check if the mesh contains a material or not:
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<ModelTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<ModelTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

#endif

		return Mesh(vertices, indices, textures);
	}

	std::vector<ModelTexture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName)
	{
		std::vector<ModelTexture> textures;
		for (size_t i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString string;
			material->GetTexture(type, i, &string);
			bool skip = false;
			for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++)
			{
				if (std::strcmp(m_TexturesLoaded[j].m_Path.data(), string.C_Str()) == 0)
				{
					textures.push_back(m_TexturesLoaded[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				ModelTexture texture;
				texture.m_TextureObject = Texture2D::CreateTexture(string.C_Str() + '/' + m_FilePath);
				texture.m_TextureType = typeName;
				texture.m_Path = string.C_Str();
				textures.push_back(texture);
			}
		}

		return textures;
	}


}