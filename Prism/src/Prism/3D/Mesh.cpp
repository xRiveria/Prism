#include "PrismPrecompiledHeader.h"
#include "Mesh.h"
#include "Prism/Renderer/Renderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Prism
{
	Mesh::Mesh(std::vector<ModelVertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> texture)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = texture;

		SetupMesh();
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::DrawMesh(Reference<Shader>& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;

		for (size_t i = 0; i < textures.size(); i++)
		{
			textures[i].m_TextureObject->BindTexture(i); //Active the proper texture unit before binding.
			//Retrieve texture number (the N in diffuseN)
			std::string number;
			std::string name = textures[i].m_TextureType;

			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++);
			}

			shader->SetShaderFloat(("material." + name + number).c_str(), i);
			textures[i].m_TextureObject->BindTexture();
			//glBindTexture(GL_TEXTURE_2D, textures[i].m_TextureObject);
		}

		//Draw Mesh
		m_VertexArrayObject->BindVertexArray();
		Prism::Renderer::SubmitToRenderQueue(shader, m_VertexArrayObject, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	}

	void Mesh::SetupMesh()
	{
		//Thanks to the constructor, we now have large lists of mesh data that we can use for rendering.
		//We do need to setup the appproriate buffers and specify the vertx shader layout via vertex attribute pointers.
		m_VertexArrayObject = VertexArray::CreateVertexArray();
		m_VertexArrayObject->BindVertexArray();
	    //m_VertexBufferObject = VertexBuffer::CreateVertexBuffer(vertices, vertices.size() * sizeof(ModelVertex));
		m_IndexBufferObject = IndexBuffer::CreateIndexBuffer(&indices[0], indices.size());

		m_VertexBufferObject->SetBufferLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" },
		});
		
		m_VertexArrayObject->AddVertexBuffer(m_VertexBufferObject);
		m_VertexArrayObject->SetIndexBuffer(m_IndexBufferObject);
	}

}