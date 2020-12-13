#version 330 core

layout (location = 0) in vec3 _position;

out vec4 lightFragPosition;
out vec4 fragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightViewProjection;                    // Матрица для системы отсчета относительно источника света 

void main()
{
    vec4 modelFragPosition = model * vec4(_position, 1.0f);
    lightFragPosition = lightViewProjection * modelFragPosition;
    gl_Position = projection * view * modelFragPosition;
    fragPosition = gl_Position;
}