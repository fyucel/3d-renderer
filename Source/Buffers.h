#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

private:
	unsigned int id;
};

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int Count() const { return count; }

private:
	unsigned int id;
	unsigned int count;
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() : stride{ 0 } {}

	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;
	};

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		vertexBufferElements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * sizeof(GLfloat);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		vertexBufferElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * sizeof(GLuint);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		vertexBufferElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * sizeof(GLubyte);
	}

	inline const std::vector<VertexBufferElement>& Elements() const
	{
		return vertexBufferElements;
	}

	inline unsigned int Stride() const { return stride; }

private:
	std::vector<VertexBufferElement> vertexBufferElements;
	unsigned int stride;
};

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void AddBuffer(const VertexBuffer& vertexBuffer,
		const VertexBufferLayout& vertexBufferLayout) const;

	void Bind() const;
	void Unbind() const;

private:
	unsigned int id;

	unsigned int SizeOfType(unsigned int vertexBufferElementType) const;
};