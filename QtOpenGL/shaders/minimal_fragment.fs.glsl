#version 330 core

in vec3 vFragColor;
in vec2 vTexCoords;

uniform bool textureMode;
uniform sampler2D texture;

out vec4 fFragColor;

void main() {
	vec4 color = vec4(vFragColor,1);
	if(textureMode)
		color = color * texture2D(texture,vTexCoords).rgba;
		//color = vec3(vTexCoords.x,vTexCoords.y,0);
	fFragColor = color;
};
