#pragma once

#include "Buffers.h"
#include "Shader.h"

class Mesh
{
public:
	Mesh(Vertex* verticesArray, const unsigned int numVertices,
		unsigned int* indicesArray, const unsigned int numIndices,
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 rotation = glm::vec3(0.0f),
		glm::vec3 scale = glm::vec3(1.0f));

	void Render(Shader* shader);

	void Bind() const;
	void Unbind() const;

	inline void Move(glm::vec3 position) { this->position += position; }
	inline void Rotate(glm::vec3 rotation) { this->rotation += rotation; }
	inline void Enlarge(glm::vec3 scale) { this->scale += scale; }

	inline void Position(glm::vec3 position) { this->position = position; }
	inline void Rotatation(glm::vec3 rotation) { this->rotation = rotation; }
	inline void Scale(glm::vec3 scale) { this->scale = scale; }

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 modelMatrix;

	unsigned numVertices;
	unsigned numIndices;

	std::unique_ptr<VertexArrayObject> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<VertexBufferLayout> vertexBufferLayout;

	void UpdateModelMatrix();

	void InitializeVertexBuffer(Vertex* verticesArray,
		const unsigned int numVertices);
	void InitializeVertexArrayAndBufferLayout();
	void InitializeIndexBuffer(unsigned int* indicesArray,
		const unsigned int numIndices);
};