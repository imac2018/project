#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texpos;

// global parameter
uniform vec4 globalLight;

// object parameter
uniform mat4 objectMVPMatrix;

out vec3 vFragColor;
out vec2 vTexCoords;

void main(void)
{
	// Passage en coordonnées homogènes
	vec4 vertexPosition = vec4(position, 1);

	// Calcul des valeurs de sortie
	vFragColor = (color * globalLight.rgb);
	vTexCoords = texpos;

	// Calcul de la position projetée
	gl_Position = objectMVPMatrix * vertexPosition;
}
