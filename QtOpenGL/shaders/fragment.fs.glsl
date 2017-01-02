#version 330 core

in vec3 vFragPosition;
in vec3 vFragNormal;
in vec3 vFragColor;
in vec2 vTexCoords;

in vec3 viewDirection;

uniform sampler2D texture;
uniform bool textureMode;

uniform vec3 directionalLightDir;
uniform vec4 directionalLightColor;

uniform vec4 ambiantLight;

uniform vec4 objectDiffuseColor;
uniform vec4 objectAmbiantColor;
uniform vec4 objectSpecularColor;

uniform vec4 torchlightColor;
uniform float torchlightPower;

out vec4 fFragColor;

void main() {
	float cosTheta = clamp(dot( vFragNormal, directionalLightDir ),0,1);

	// Eye vector (towards the camera)
	vec3 E = normalize(viewDirection);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-directionalLightDir, vFragNormal);

	float cosAlpha  = clamp(dot( E,R ),0,1);

	vec4 color = vec4(vFragColor,1);
	if(textureMode)
		color = color * texture2D(texture,vTexCoords);

	fFragColor = color * (objectAmbiantColor * ambiantLight +
					 objectDiffuseColor * cosTheta * directionalLightColor +
						objectSpecularColor * pow(cosAlpha,5) * directionalLightColor);

	if(torchlightPower>0.01f){
		vec3 lightDir = E;
		cosTheta = clamp(dot( normalize(vec3(0,0,-1)), -lightDir),0,1);
		if(cosTheta>0.85){
			R = reflect(-E, vFragNormal);
			cosAlpha  = clamp(dot( E,R ),0,1);
			float dist = length(vFragPosition)*0.001;
			float att = pow(cosTheta, 3.f) / (0.5f + 0.1f * dist + 0.25f * dist * dist);
			cosTheta = clamp(dot( vFragNormal, lightDir ),0,1);
			vec4 lightColor = torchlightColor * att * torchlightPower;
			fFragColor += color * (cosTheta * objectDiffuseColor * lightColor
								   +  objectSpecularColor * pow(cosAlpha,5) * lightColor);
		}
	}
};
