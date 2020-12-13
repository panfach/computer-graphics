#version 330 core

in vec3 texCoords;

out vec4 color;

uniform samplerCube skybox;

void main()
{    
    color = vec4(vec3(texture(skybox, texCoords)), 0.0f);
}