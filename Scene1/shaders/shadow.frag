#version 330 core

in vec4 lightFragPosition;
in vec4 fragPosition;

out vec4 color;

uniform sampler2D lightShadowMap;

void main()
{
    vec3 fragPosition = fragPosition.xyz / fragPosition.w;
    vec3 projCoord = lightFragPosition.xyz / lightFragPosition.w;

    projCoord = projCoord * 0.5f + 0.5f;
    float closestDepth = texture(lightShadowMap, projCoord.xy).r;
    float currentDepth = projCoord.z;
    
    color = vec4((currentDepth - 0.005f > closestDepth) ? 0.0f : 1.0f, 0.0f, 0.0f, fragPosition.z);
}