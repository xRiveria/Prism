#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Prism/Core/Core.h"
#include "Prism/Renderer/Shader.h"
#include "Prism/Renderer/VertexArray.h"
#include "Prism/Renderer/Texture.h"
#include "Prism/Renderer/Buffer.h"

/* Mesh

- A Mesh represents a single drawable entity.
- Whenever we load a model with Assimp, upon completion they are stored in Assimp's data structures.
- We now need to transform that data to a format that OpenGL understands so that we can render our objects.
- A Mesh needs a set of vertices, where each vertex contains a position vector, a normal vector and a texture coordinate vector.
- A mesh should also contain indices for index drawing, and material data in the form of textures (diffuse/specular maps).
*/

namespace Prism
{
	struct ModelVertex //Minimal requirements for a mesh class for us to define a vertex in OpenGL.
	{
		//The memory layout of structs in C++ is sequential. That is, if we were to represent a struct as an array of data, it would contain the struct's data in sequential data which directly
		//translates to a float (actually byte) array that we would want for an array buffer. 
		//Another great use of structs is a preprocessor directive called offsetof(), that takes as its first argument a struct and as its second argument a variable name of the struct. The macro
		//returns the byte offset of that variable from the start of the struct. This is perfect for defining the offset parameter of the glVertexAttribPointer function.

		//Thus, if we want another vertex attribute, we can simply add it to the struct and due to its flexible nature, the rendering code will not break.
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoordinates;
	};

	struct ModelTexture
	{
		Reference<Texture2D> m_TextureObject;
		std::string m_TextureType;
		std::string m_Path; //We store the path of the texture to compare with other textures.
	};

	class Mesh
	{
	public: //Mesh Data
		Mesh(std::vector<ModelVertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> texture);
		~Mesh();

		void DrawMesh(Reference<Shader>& shader); //Draw the Mesh

	public:
		std::vector<ModelVertex> vertices; //Information on the vertexes of our object. 
		std::vector<unsigned int> indices; //Information on the indices of our object.
		std::vector<ModelTexture> textures; //Information on the textures of our object.

	private: //Render Data
		Reference<VertexArray> m_VertexArrayObject;
		Reference<VertexBuffer> m_VertexBufferObject;
		Reference<IndexBuffer> m_IndexBufferObject;

		void SetupMesh(); //Initialize Buffers
	};
}
