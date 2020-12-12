#version 330 core

layout (location = 0) in vec3 _position;

out vec4 lightFragPosition;
out vec4 fragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightViewProjection;

void main()
{
    vec3 modelFragPosition = vec3(model * vec4(_position, 1.0f));
    lightFragPosition = lightViewProjection * vec4(modelFragPosition, 1.0f);

    gl_Position = projection * view * vec4(modelFragPosition, 1.0f);
    fragPosition = gl_Position;
    //gl_Position = vec4(modelFragPosition / 100, 1.0f);
}