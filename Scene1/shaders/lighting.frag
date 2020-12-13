#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
in vec4 screenFragPos;

out vec4 color;

uniform Material material;
uniform Light light;
uniform sampler2D diffuseTexture1;
uniform sampler2D specularTexture1;
uniform sampler2D normalTexture1;
uniform sampler2D shadowMap;
uniform bool hasNormalMap;
uniform bool hasSpecMap;

void main()
{
    float diff, spec, shadow, defaultSpec = 0.5f;
    vec3 norm, viewDir, reflectDir;
    float specularStrength = 1.0f;
    float ambientStrength = 0.1f;

    if (hasNormalMap) {
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
            spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        }
    }
    
    vec3 projCoord = screenFragPos.xyz / screenFragPos.w;
    projCoord = projCoord * 0.5f + 0.5f;
    shadow = 1.0f - texture(shadowMap, projCoord.xy).r;

    vec3 ambient  = light.ambient * vec3(texture(diffuseTexture1, texCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(diffuseTexture1, texCoords));
    vec3 specular;
    if (hasSpecMap) specular = specularStrength * light.specular * spec * vec3(texture(specularTexture1, texCoords)); 
    else            specular = specularStrength * light.specular * spec * defaultSpec; 

    vec3 result = ambient + (1.0f - shadow) * (diffuse + specular);


    float fogStrength = gl_FragCoord.z * 60f - 59f;
    if (fogStrength < 0.0f) fogStrength = 0.0f;
    result = mix(result, vec3(0.282f, 0.314f, 0.325f), fogStrength);


    float depth = 1.0f - gl_FragCoord.z;
    color = vec4(result, depth); 
}