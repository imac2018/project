#version 330 core

in vec2 vFragPos;
in vec3 vFragNormal;
in vec3 vFragColor;
in vec2 vTexCoords;

in vec3 viewDirection;

uniform sampler2D texture;
bool textureMode;

uniform vec3 directionalLightDir;
uniform vec3 directionalLightColor;

uniform vec3 ambiantLight;

uniform vec3 objectDiffuseColor;
uniform vec3 objectAmbiantColor;
uniform vec3 objectSpecularColor;

out vec3 fFragColor;

void main() {
	float cosTheta = clamp(dot( vFragNormal, directionalLightDir ),0,1);

	// Eye vector (towards the camera)
	vec3 E = normalize(viewDirection);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-directionalLightDir, vFragNormal);

	float cosAlpha  = clamp(dot( E,R ),0,1);
	vec3 color = vFragColor;
	if(textureMode)
		color = color * texture2D(texture,vTexCoords).rgb;

	fFragColor = color  *
				(objectAmbiantColor * ambiantLight +
					objectDiffuseColor * cosTheta * directionalLightColor +
						objectSpecularColor * pow(cosAlpha,5) * directionalLightColor);
};
