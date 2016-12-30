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
	fFragColor = color * (objectAmbiantColor * ambiantLight +
					 objectDiffuseColor * cosTheta * directionalLightColor +
						objectSpecularColor * pow(cosAlpha,5) * directionalLightColor).rgb;
	if(torchlightPower>0.01f){
		cosTheta = abs(dot( E , normalize(vec3(0,-0.05,-1)) ));
		if(cosTheta>0.75){
			float power = 1.f/torchlightPower;
			float distance = length(vFragPosition)*0.2f;
			cosTheta = clamp(dot( vFragNormal, -E ),0,1);;
			fFragColor += color * cosTheta *
					objectDiffuseColor * torchlightColor/pow(distance,power);
		}

		//fFragColor = vec3(cosTheta,cosTheta,cosTheta);
	}
};
