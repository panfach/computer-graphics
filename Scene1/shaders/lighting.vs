#version 330 core

layout (location = 0) in vec3 _position;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _texCoords;
layout (location = 3) in vec3 _tangent;
layout (location = 4) in vec3 _bitangent;

out vec3 normal;
out vec3 fragPosition;
out vec3 tangentFragPosition; //////
out vec3 lightDir;
out vec3 tangentLightDir;  //////////
out vec3 cameraPos; //////////
out vec3 tangentCameraPos; /////////
out vec2 texCoords;
out vec4 lightFragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 tangentMatrix; //////////
uniform vec3 lightDirection;
uniform mat4 lightViewProjection;
uniform vec3 cameraPosition; //////

void main()
{
    /*texCoords = _texCoords;
    normal = mat3(transpose(inverse(view * model))) * _normal;
    gl_Position = projection * view * model * vec4(_position, 1.0f);
    lightDir = mat3(transpose(inverse(view))) * lightDirection;
    vec3 modelFragPosition = vec3(model * vec4(_position, 1.0f));
    fragPosition = vec3(view * vec4(modelFragPosition, 1.0f));
    lightFragPosition = lightViewProjection * vec4(modelFragPosition, 1.0f);*/

    texCoords = _texCoords;
    normal = mat3(model) * _normal;
    lightDir = lightDirection;
    tangentLightDir = lightDirection;
    vec3 modelFragPosition = vec3(model * vec4(_position, 1.0f));
    fragPosition = modelFragPosition;
    lightFragPosition = lightViewProjection * vec4(modelFragPosition, 1.0f);

    mat3 model0 = transpose(inverse(mat3(model)));
    vec3 T = normalize(model0 * _tangent);
    vec3 N = normalize(model0 * _normal);
    vec3 B = normalize(model0 * _bitangent);
    mat3 TBN = transpose(mat3(T, B, N));
    tangentLightDir = normalize(TBN * lightDirection);
    tangentCameraPos = TBN * cameraPosition;
    tangentFragPosition = TBN * modelFragPosition;

    gl_Position = projection * view * model * vec4(_position, 1.0f);
}