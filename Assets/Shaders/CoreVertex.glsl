#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoordinate;
layout(location = 2) in vec3 normal;

out vec3 v_position;
out vec2 v_textureCoordinate;
out vec3 v_normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	v_position = vec4(modelMatrix * vec4(position, 1.0f)).xyz;
	v_textureCoordinate = vec2(textureCoordinate.x, textureCoordinate.y);
	v_normal = mat3(modelMatrix) * normal;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
}