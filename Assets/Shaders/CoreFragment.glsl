#version 440

in vec3 v_position;
in vec2 v_textureCoordinate;
in vec3 v_normal;

out vec4 color;

uniform sampler2D modelTexture;
uniform vec3 lightSource;

vec3 CalculateDiffuseLighting()
{
	vec3 positionToLightDirection = normalize(lightSource - v_position);
	return vec3(1.0f) * clamp(dot(positionToLightDirection, v_normal), 0, 1);
}

void main()
{
	vec3 lightVector = normalize(lightSource - v_position);
	color = texture(modelTexture, v_textureCoordinate)
		* (vec4(CalculateDiffuseLighting(), 1.0f));
}