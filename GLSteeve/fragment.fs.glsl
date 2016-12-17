#version 330 core

in vec2 vTexCoords;

uniform sampler2D uTexture;

out vec4 fFragColor;

void main() {
  fFragColor = (texture(uTexture, vTexCoords));
};
