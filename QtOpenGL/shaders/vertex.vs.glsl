#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texpos;
layout(location = 3) in vec3 normal;


// global parameter
uniform vec4 globalLight;

// object parameter
uniform mat4 objectMVMatrix;
uniform mat4 objectMVPMatrix;
uniform mat4 objectNormalMatrix;

out vec3 vFragPosition;
out vec3 vFragNormal;
out vec3 vFragColor;
out vec2 vTexCoords;

out vec3 viewDirection;

void main(void)
{
	// Passage en coordonnées homogènes
	vec4 vertexPosition = vec4(position, 1);
	vec4 vertexNormal = vec4(normal, 0);

	// Calcul des valeurs de sortie
	vFragPosition = vec3(objectMVMatrix * vertexPosition);
	viewDirection =  vec3(0,0,0) - vFragPosition;
	vFragNormal = vec3(objectNormalMatrix * vertexNormal);
        vFragColor = color * globalLight.rgb;
	vTexCoords = texpos;

	// Calcul de la position projetée
	gl_Position = (objectMVPMatrix * vertexPosition);
}
