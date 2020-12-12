#version 330 core

layout (location = 0) in vec3 _pos;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _texCoord;

out vec2 texCoords;

void main()
{
    texCoords = _texCoord;
    gl_Position = vec4(_pos.x * 2, _pos.y * 2, _pos.z * 2, 1.0f);
}  