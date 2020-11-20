#version 330 core

in vec3 normal;
in vec3 fragPosition;
in vec3 lightPosition;

out vec4 color;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float specularStrength = 0.5f;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(-fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
}