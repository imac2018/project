#version 330 core
#ifdef GL_ES
precision mediump float;
#endif

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
        float cosTheta = clamp(dot( vFragNormal, -directionalLightDir ),0,1);
        vec3 E = normalize(viewDirection);
        vec3 R = normalize(-reflect(-directionalLightDir, vFragNormal));

        float cosAlpha  = clamp(dot( E,R ),0,1);

        vec4 color = vec4(vFragColor,1);
        if(textureMode)
                color = color * texture2D(texture,vTexCoords);
        vec4 totallight = objectAmbiantColor * ambiantLight;
        totallight += vec4((objectDiffuseColor * directionalLightColor * cosTheta).rgb,0);
        totallight += vec4((objectSpecularColor * directionalLightColor * pow(cosAlpha,3)).rgb,0);
        fFragColor = color * totallight;

        if(torchlightPower>0.01f){
                vec3 lightDir = -E;
                cosTheta = clamp(dot( vec3(0,0,-1), lightDir),0,1);
                if(cosTheta>0.82){
                        R = normalize(-reflect(-lightDir, vFragNormal));
                        cosAlpha  = clamp(dot( E,R ),0,1);
                        cosTheta = clamp(dot( vFragNormal, -lightDir ),0,1);
                        float dist = length(vFragPosition)*0.01f;
                        float att = pow(cosTheta, 3.f) / (0.75f + 0.2f * dist + 2.f * dist * dist);
                        vec4 tlightColor = torchlightColor * att;
                        totallight = objectDiffuseColor * tlightColor * cosTheta;
                        totallight += vec4((objectSpecularColor * tlightColor * pow(cosAlpha,3)).rgb,0);
                        fFragColor += color * totallight;
                }
        }
}
