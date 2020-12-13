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
in vec4 screenFragPos;

out vec4 color;

uniform Material material;
uniform Light light;
uniform sampler2D diffuseTexture1;
uniform sampler2D specularTexture1;
uniform sampler2D normalTexture1;
uniform sampler2D shadowMap;

uniform int hasNormalMap;

void main()
{
    float diff, spec, shadow;
    vec3 norm, viewDir, reflectDir;

    float specularStrength = 1.0f;
    float ambientStrength = 0.1f;



    if (hasNormalMap == 1) {
        norm = texture(normalTexture1, texCoords).rgb;
        norm = normalize(norm * 2.0f - 1.0f);

        diff = max(dot(norm, -tangentLightDir), 0.0f);
        viewDir = normalize(tangentCameraPos - tangentFragPosition);
        if (dot(norm, viewDir) < -0.2f) {
            diff = 0.0f;
            spec = 0.0f;
        }
        else {
            reflectDir = reflect(tangentLightDir, norm);
            spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        }
    }
    else {
        norm = normal;

        diff = max(dot(norm, -lightDir), 0.0f);
        viewDir = normalize(cameraPos - fragPosition);
        if (dot(norm, viewDir) < -0.2f) {
            diff = 0.0f;
            spec = 0.0f;
        }
        else {
            reflectDir = reflect(lightDir, norm);
            spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
        }
    }

    /*vec3 projCoords = lightFragPosition.xyz / lightFragPosition.w;
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
    }*/
    
    vec3 projC = screenFragPos.xyz / screenFragPos.w;
    projC = projC * 0.5f + 0.5f;
    shadow = 1.0f - texture(shadowMap, projC.xy).r;
    //if (screenFragCoord.x > 0) shadow = 1.0f; else shadow = 0.0f;
    //shadow = screenFragCoord.y;
    //shadow = 0.0f;

    //if (diff < 0.2f) diff = 0.0f;
    //else if (diff < 0.5) diff = 0.4f;
    //else if (diff < 0.8) diff = 0.8f;
    //else diff = 1.0f;

    vec3 ambient  = light.ambient * vec3(texture(diffuseTexture1, texCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(diffuseTexture1, texCoords));
    vec3 specular = specularStrength * light.specular * spec * vec3(texture(specularTexture1, texCoords)); 

    vec3 result = ambient + (1.0f - shadow) * (diffuse + specular);

    float fogStrength = gl_FragCoord.z * 60f - 59f;
    if (fogStrength < 0.0f) fogStrength = 0.0f;
    result = mix(result, vec3(0.282f, 0.314f, 0.325f), fogStrength);


    float depth = gl_FragCoord.z;
    depth = 1.0f - depth;
    color = vec4(result, depth); /////
    //color = vec4(vec3(shadow), 1.0f);
}