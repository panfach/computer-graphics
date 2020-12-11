#version 330 core

layout (location = 0) in vec3 _position;

out vec4 fragCoord;

uniform mat4 lightViewProjection;
uniform mat4 model;

void main()
{
    //fragPosition =  vec3(model * vec4(_position, 1.0f));

    gl_Position = lightViewProjection * model * vec4(_position, 1.0);

    fragCoord = gl_Position;
}