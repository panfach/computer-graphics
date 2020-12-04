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
in vec3 lightPosition;
in vec2 texCoords;

out vec4 color;
  
uniform Material material;
uniform Light light;
uniform sampler2D diffuseTexture1;
uniform sampler2D specularTexture1;
uniform sampler2D normalTexture1;

void main()
{
    float specularStrength = 0.5f;
    float ambientStrength = 0.1f;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 viewDir = normalize(-fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);

    //if (diff < 0.2f) diff = 0.0f;
    //else if (diff < 0.5) diff = 0.4f;
    //else if (diff < 0.8) diff = 0.8f;
    //else diff = 1.0f;


    vec3 ambient  = light.ambient * vec3(texture(diffuseTexture1, texCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(diffuseTexture1, texCoords));
    vec3 specular = 0.3f * light.specular * spec * vec3(texture(specularTexture1, texCoords)); 

    vec3 result = ambient + diffuse + specular;
    
    

    color = vec4(result, 1.0f);
}