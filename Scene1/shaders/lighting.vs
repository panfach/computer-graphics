#version 330 core

layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _texCoords;

out vec3 normal;
out vec3 fragPosition;  
out vec3 lightPosition;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 _lightPosition;

void main()
{
    gl_Position = projection * view * model * vec4(_position, 1.0f);
    normal = mat3(transpose(inverse(view * model))) * _normal;
    lightPosition = vec3(view * vec4(_lightPosition, 1.0));
    fragPosition = vec3(view * model * vec4(_position, 1.0f));
    texCoords = _texCoords;
}