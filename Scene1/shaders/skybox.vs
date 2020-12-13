#version 330 core

layout (location = 0) in vec3 _pos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texCoords = _pos;
    vec4 pos = projection * view * vec4(_pos, 1.0);
    gl_Position = pos.xyww;                                // ���� ����� xyzw , �� �� �� ����� ������� �� ���������
                                                           // ����� ������ ������ �� ����� �������������� �������
}  