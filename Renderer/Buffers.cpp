#include "Utilities.h"
#include "Buffers.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) : id{ 0 }
{
	GL(glGenBuffers(1, &id));
	GL(glBindBuffer(GL_ARRAY_BUFFER, id));
	GL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GL(glDeleteBuffers(1, &id));
}

void VertexBuffer::Bind() const
{
	GL(glBindBuffer(GL_ARRAY_BUFFER, id));
}

void VertexBuffer::Unbind() const
{
	GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :
	id{ 0 }, count{ count }
{
	GL(glGenBuffers(1, &id));
	GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
		data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GL(glDeleteBuffers(1, &id));
}

void IndexBuffer::Bind() const
{
	GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void IndexBuffer::Unbind() const
{
	GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

VertexArrayObject::VertexArrayObject() : id{ 0 }
{
	GL(glGenVertexArrays(1, &id));
}

VertexArrayObject::~VertexArrayObject()
{
	GL(glDeleteVertexArrays(1, &id));
}

void VertexArrayObject::AddBuffer(const VertexBuffer& vertexBuffer,
	const VertexBufferLayout& vertexBufferLayout) const
{
	Bind();
	vertexBuffer.Bind();

	unsigned int offset = 0;
	const auto& elements = vertexBufferLayout.Elements();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GL(glEnableVertexAttribArray(i));
		GL(glVertexAttribPointer(i, element.count, element.type,
			element.normalized, vertexBufferLayout.Stride(),
			(const void*)offset));
		offset += element.count * SizeOfType(element.type);
	}
}

void VertexArrayObject::Bind() const
{
	GL(glBindVertexArray(id));
}

void VertexArrayObject::Unbind() const
{
	GL(glBindVertexArray(0));
}

unsigned int VertexArrayObject::SizeOfType(unsigned int
	vertexBufferElementType) const
{
	switch (vertexBufferElementType)
	{
	case GL_FLOAT: return sizeof(GLfloat);
	case GL_UNSIGNED_INT: return sizeof(GLuint);
	case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
	}
	return 0;
}
