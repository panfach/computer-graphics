#version 330 core

in vec4 fragCoord;

out vec4 color;

void main()
{
   float value = fragCoord.z;

   color = vec4(value, value * value, 1.0f, 1.0f);
}