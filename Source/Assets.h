#pragma once

#include "AssetEnums.h"
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

	void Render(Shader* shaderToUse);

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

	void Bind(Shader* shaderToUse) const;
	void Unbind() const;

	void UpdateModelMatrix();

	void InitializeVertexBuffer(Vertex* verticesArray,
		const unsigned int numVertices);
	void InitializeVertexArrayAndBufferLayout();
	void InitializeIndexBuffer(unsigned int* indicesArray,
		const unsigned int numIndices);
};

class Texture
{
public:
	Texture(const std::string& filename);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int Width() const { return width; }
	inline int Height() const { return height; }

private:
	unsigned int id;
	std::string filename;
	int width, height;
};

class AssetContainer
{
public:
	AssetContainer();

	Mesh* GetMesh(MeshEnum meshEnum);
	Texture* GetTexture(TextureEnum textureEnum);

private:
	std::unordered_map<MeshEnum, std::unique_ptr<Mesh>> meshes;
	std::unordered_map<TextureEnum, std::unique_ptr<Texture>> textures;

	void LoadMesh(MeshEnum meshToLoad, const std::string& filename,
		float meshHeight = 0.0);
	void LoadTexture(TextureEnum textureToLoad, const std::string& filename);
};

class OBJLoader
{
public:
	OBJLoader(const std::string& filename);

	inline const std::vector<Vertex>& Vertices() const { return vertices; }
	inline const std::vector<unsigned int>& Indices() const { return indices; }

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};