#pragma once

// Precompiled header for the Renderer project

// STL headers:

#include <iostream>

#include <sstream>

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