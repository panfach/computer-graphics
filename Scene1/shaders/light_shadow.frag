#version 330 core

in vec4 fragCoord;

out vec4 color;

void main()
{
   color = vec4(fragCoord.z, vec3(0.0f));
}