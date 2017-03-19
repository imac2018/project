#version 330 core
#ifdef GL_ES
precision mediump float;
#endif

in vec3 vFragColor;
in vec2 vTexCoords;

out vec4 fFragColor;

uniform bool textureMode;
uniform sampler2D texture;

void main() {
	vec4 color = vec4(vFragColor,1);
	if(textureMode)
		color = color * texture2D(texture,vTexCoords).rgba;
	fFragColor = color;
}
