#include "Utilities.h"
#include "Assets.h"

Mesh::Mesh(Vertex* verticesArray, const unsigned int numVertices,
	unsigned int* indicesArray, const unsigned int numIndices,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
	numVertices{ numVertices },
	numIndices{ numIndices },
	position{ position },
	rotation{ rotation },
	scale{ scale }
{
	InitializeVertexBuffer(verticesArray, numVertices);
	InitializeVertexArrayAndBufferLayout();
	InitializeIndexBuffer(indicesArray, numIndices);
	Unbind();
}

void Mesh::Render(Shader* shader)
{
	UpdateModelMatrix();
	shader->UniformMat4fv("modelMatrix", modelMatrix);

	shader->Bind();
	Bind();

	GL(glDrawElements(GL_TRIANGLES, indexBuffer->Count(),
		GL_UNSIGNED_INT, nullptr));
}

void Mesh::Bind() const
{
	vertexArray->Bind();
	vertexBuffer->Bind();
	if (numIndices) indexBuffer->Bind();
}

void Mesh::Unbind() const
{
	vertexArray->Unbind();
	vertexBuffer->Unbind();
	if (numIndices) indexBuffer->Unbind();
}

void Mesh::UpdateModelMatrix()
{
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x),
		glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y),
		glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z),
		glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, scale);
}

void Mesh::InitializeVertexBuffer(Vertex* verticesArray,
	const unsigned int numVertices)
{
	vertexBuffer = std::make_unique<VertexBuffer>((void*)verticesArray,
		(unsigned int)(sizeof(Vertex) * numVertices));
}

void Mesh::InitializeVertexArrayAndBufferLayout()
{
	vertexArray = std::make_unique<VertexArrayObject>();
	vertexBufferLayout = std::make_unique<VertexBufferLayout>();
	vertexBufferLayout->Push<float>(3);
	vertexBufferLayout->Push<float>(2);
	vertexBufferLayout->Push<float>(3);
	vertexArray->AddBuffer(*vertexBuffer, *vertexBufferLayout);
}

void Mesh::InitializeIndexBuffer(unsigned int* indicesArray,
	const unsigned int numIndices)
{
	indexBuffer = std::make_unique<IndexBuffer>(indicesArray,
		numIndices);
	indexBuffer->Unbind();
}