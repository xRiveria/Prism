#include "PrismPrecompiledHeader.h"
#include "Mesh.h"

namespace Prism
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;

		SetupMesh();
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::SetupMesh()
	{
		//Thanks to the constructor, we now have large lists of mesh data that we can use for rendering.
		//We do need to setup the appproriate buffers and specify the vertx shader layout via vertex attribute pointers.


	}

}