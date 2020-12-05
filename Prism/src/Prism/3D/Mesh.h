#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Prism/Core/Core.h"
#include "Prism/Renderer/Buffer.h"
#include "Prism/Renderer/VertexArray.h"

/* Mesh

- A Mesh represents a single drawable entity.
- Whenever we load a model with Assimp, upon completion they are stored in Assimp's data structures.
- We now need to transform that data to a format that OpenGL understands so that we can render our objects.
- A Mesh needs a set of vertices, where each vertex contains a position vector, a normal vector and a texture coordinate vector.
- A mesh should also contain indices for index drawing, and material data in the form of textures (diffuse/specular maps).
*/

namespace Prism
{
	struct Vertex //Minimal requirements for a mesh class for us to define a vertex in OpenGL.
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TextureCoordinates;
	};

	class Mesh
	{
	public: //Mesh Data
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		~Mesh();

		void Draw(); //Draw the Mesh

	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

	private: //Render Data

		Reference<VertexArray> m_VertexArray;
		Reference<VertexBuffer> m_VertexBuffer;
		Reference<IndexBuffer> m_IndexBuffer;
		void SetupMesh(); //Initialize Buffers
	};
}
