#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 fragPosition;
in vec3 tangentFragPosition;
in vec3 lightDir;
in vec3 tangentLightDir;
in vec3 cameraPos;
in vec3 tangentCameraPos;
in vec2 texCoords;
in vec4 lightFragPosition;

out vec4 color;

uniform Material material;
uniform Light light;
uniform sampler2D diffuseTexture1;
uniform sampler2D specularTexture1;
uniform sampler2D normalTexture1;
uniform sampler2D shadowMap;

void main()
{
    float spec;

    float specularStrength = 0.5f;
    float ambientStrength = 0.1f;

    /*
    //vec3 norm0 = texture(normalTexture1, texCoords).rgb;
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 viewDir = normalize(-fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);

    vec3 projCoords = lightFragPosition.xyz / lightFragPosition.w;
    projCoords = projCoords * 0.5f + 0.5f;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    //float calcShift = max (0.05f * (1.0f - dot(normal, lightDir)), 0.005);
    //float shadow = (currentDepth - 0.005 > closestDepth) ? 1.0f : 0.0f;  
    */




    //vec3 norm = normalize(normal);
    //float diff = max(dot(norm, -lightDir), 0.0);

    vec3 norm = texture(normalTexture1, texCoords).rgb;
    norm = normalize(norm * 2.0f - 1.0f);
    float diff = max(dot(norm, -tangentLightDir), 0.0f);
    vec3 viewDir = normalize(tangentCameraPos - tangentFragPosition);
    /*if (dot(norm, viewDir) < 0.0f) {
        diff = 0.0f;
        spec = 0.0f;
    }
    else {*/
        vec3 reflectDir = reflect(tangentLightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //}

    vec3 projCoords = lightFragPosition.xyz / lightFragPosition.w;
    projCoords = projCoords * 0.5f + 0.5f;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    
    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            float _depth = texture(shadowMap, projCoords.xy + vec2(i, j) * texelSize).r;
            shadow += (currentDepth - 0.005f > _depth) ? 0.111f : 0.0f;
        }
    }

    //if (diff < 0.2f) diff = 0.0f;
    //else if (diff < 0.5) diff = 0.4f;
    //else if (diff < 0.8) diff = 0.8f;
    //else diff = 1.0f;

    vec3 ambient  = light.ambient * vec3(texture(diffuseTexture1, texCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(diffuseTexture1, texCoords));
    vec3 specular = specularStrength * light.specular * spec * vec3(texture(specularTexture1, texCoords)); 

    vec3 result = ambient + (1.0f - shadow) * (diffuse + specular);

    color = vec4(result, 1.0);
}