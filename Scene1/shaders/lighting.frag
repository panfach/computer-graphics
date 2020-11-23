#version 330 core

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
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

void main()
{
    float specularStrength = 0.5f;
    float ambientStrength = 0.1f;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 viewDir = normalize(-fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient * material.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));  

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}