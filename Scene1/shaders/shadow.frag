#version 330 core

in vec4 lightFragPosition;
in vec4 fragPosition;

out vec4 color;

uniform sampler2D lightShadowMap;

void main()
{
    vec3 projCoords = lightFragPosition.xyz / lightFragPosition.w;
    projCoords = projCoords * 0.5f + 0.5f;
    float closestDepth = texture(lightShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    
    float _depth = texture(lightShadowMap, projCoords.xy).r;
    
    vec3 fragPosition = fragPosition.xyz / fragPosition.w;
    color = vec4(vec3((currentDepth - 0.005f > _depth) ? 0.0f : 1.0f), fragPosition.z);
    //color = vec4(currentDepth, 0.0f, 0.0f, 1.0f);
    //color = vec4(0.7f, 0.7f, 0.7f, 1.0f);
}