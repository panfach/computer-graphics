#version 330 core

layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _texCoords;

out vec3 normal;
out vec3 fragPosition;  
out vec3 lightDir;
out vec2 texCoords;
out vec4 lightFragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightDirection;
uniform mat4 lightViewProjection;

void main()
{
    texCoords = _texCoords;
    normal = mat3(transpose(inverse(view * model))) * _normal;
    gl_Position = projection * view * model * vec4(_position, 1.0f);
    normal = mat3(transpose(inverse(view * model))) * _normal;
    lightDir = mat3(transpose(inverse(view))) * lightDirection;
    vec3 modelFragPosition = vec3(model * vec4(_position, 1.0f));
    fragPosition = vec3(view * vec4(modelFragPosition, 1.0f));
    lightFragPosition = lightViewProjection * vec4(modelFragPosition, 1.0f);
}