#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D tex;

const float xTexSize = 1.0f / 800;
const float yTexSize = 1.0f / 300;
const float amount = 5;
const float weight[5] = float[] (0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f);

void main()
{    
    float result = 0;
    
    
    result = texture(tex, texCoords).r * weight[0];
    for (int i = 1; i < amount; i++) {
        result += texture(tex, texCoords + vec2(xTexSize * i, 0.0f)).r * weight[i];
        result += texture(tex, texCoords - vec2(xTexSize * i, 0.0f)).r * weight[i];
    }
    for (int i = 1; i < amount; i++) {
        result += texture(tex, texCoords + vec2(0.0f, yTexSize * i)).r * weight[i];
        result += texture(tex, texCoords - vec2(0.0f, yTexSize * i)).r * weight[i];
    }
    


    color = vec4(vec3(result), 1.0f);
    //color = vec4(vec3(texture(tex, texCoords)), 1.0f);
    //color = vec4(1.0f);
}