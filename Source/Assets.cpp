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

void Mesh::Render(Shader* shaderToUse)
{
	UpdateModelMatrix();
	Bind(shaderToUse);

	GL(glDrawElements(GL_TRIANGLES, indexBuffer->Count(),
		GL_UNSIGNED_INT, nullptr));
}

void Mesh::Bind(Shader* shaderToUse) const
{
	shaderToUse->UniformMat4fv("modelMatrix", modelMatrix);

	shaderToUse->Bind();

	vertexArray->Bind();
	vertexBuffer->Bind();
	indexBuffer->Bind();
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

Texture::Texture(const std::string& path) :
	width{ 0 }, height{ 0 }, filename{ path }
{
	int bits;

	// TO DO: use SDL to load texture

	unsigned char* localBuffer = nullptr;
	// stbi_set_flip_vertically_on_load(1);
	// localBuffer = stbi_load(path.c_str(), &width, &height, &bits, 4);

	GL(glGenTextures(1, &id));
	GL(glBindTexture(GL_TEXTURE_2D, id));

	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	GL(glBindTexture(GL_TEXTURE_2D, 0));

	// if (localBuffer) stbi_image_free(localBuffer);
}

Texture::~Texture()
{
	GL(glDeleteTextures(1, &id));
}

void Texture::Bind(unsigned int slot) const
{
	GL(glActiveTexture(GL_TEXTURE0 + slot));
	GL(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::Unbind() const
{
	GL(glBindTexture(GL_TEXTURE_2D, 0));
}

AssetContainer::AssetContainer()
{
	LoadMesh(MeshEnum::Example, "../Assets/Objects/Example.obj");
}

Mesh* AssetContainer::GetMesh(MeshEnum meshEnum)
{
	if (meshes.find(meshEnum) == meshes.end())
	{
		std::cout << "Mesh: " << (int)meshEnum
			<< " not found in the Asset Container" << std::endl;
		return nullptr;
	}
	return meshes.at(meshEnum).get();
}

Texture* AssetContainer::GetTexture(TextureEnum textureEnum)
{
	if (textures.find(textureEnum) == textures.end())
	{
		std::cout << "Texture: " << (int)textureEnum
			<< " not found in the Asset Container" << std::endl;
		return nullptr;
	}
	return textures.at(textureEnum).get();
}

void AssetContainer::LoadMesh(MeshEnum meshToLoad, const std::string& filename)
{
	auto meshObj = OBJLoader(filename);
	meshes[meshToLoad] = std::make_unique<Mesh>(
		(Vertex*)meshObj.Vertices().data(),
		meshObj.Vertices().size(),
		(unsigned int*)meshObj.Indices().data(),
		meshObj.Indices().size());
}

void AssetContainer::LoadTexture(TextureEnum textureToLoad,
	const std::string& filename)
{
	textures[textureToLoad] = std::make_unique<Texture>(filename);
}

OBJLoader::OBJLoader(const std::string& filename)
{
	// TO DO: load OBJ files
}
