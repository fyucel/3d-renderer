#pragma once

// Precompiled header for the Renderer

// STL headers:

#include <fstream>

#include <iostream>

#include <memory>

#include <sstream>

#include <unordered_map>
#include <unordered_set>

// GLEW, GLM, and SDL dependencies

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL.h>

// Wrapper around OpenGL function calls to report any errors
#define GL(x) x; GLLogCall(__FILE__, #x, __LINE__)
static void GLLogCall(const char* file, const char* function, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OpenGL Error: " << std::hex << error << " at " << function
			<< ":" << std::dec << line << " (" << file << ") " << std::endl;
	}
}

struct Vertex
{
	Vertex() : Position{ 0.0f }, TextureCoordinate{ 0.0f }, Normal{ 0.0f } {}

	Vertex(glm::vec3 position, glm::vec2 textureCoordinate, glm::vec3 normal) :
		Position{ position },
		TextureCoordinate{ textureCoordinate },
		Normal{ normal } {}

	glm::vec3 Position;
	glm::vec2 TextureCoordinate;
	glm::vec3 Normal;
};