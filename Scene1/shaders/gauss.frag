#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D tex;
uniform bool horizontal;

const float xTexSize = 1.0f / 1200;
const float yTexSize = 1.0f / 800;
float amount = 5;
float weight[14] = float[] (0.25f, 0.18f, 0.12f, 0.05f, 0.025f,
                           0.30f, 0.18f, 0.12f, 0.05f,
                           0.45f, 0.25f, 0.05f,
                           0.70f, 0.15f);

void main()
{    
    int shiftIndex;
    float result = 0.0f;
    float depth = texture(tex, texCoords).w;


    if (depth >= 0.96f) { amount = 2; shiftIndex = 12; }
    else if (depth >= 0.93) { amount = 3; shiftIndex = 9; }
    else if (depth >= 0.88) { amount = 4; shiftIndex = 5; }
    else { amount = 5; shiftIndex = 0; }

    if (amount == 0) result = texture(tex, texCoords).r; 
    else result = texture(tex, texCoords).r * weight[shiftIndex + 0];
    if (horizontal) 
        for (int i = shiftIndex + 1; i < shiftIndex + amount; i++) {
            result += texture(tex, texCoords + vec2(xTexSize * (i - shiftIndex), 0.0f)).r * weight[i];
            result += texture(tex, texCoords - vec2(xTexSize * (i - shiftIndex), 0.0f)).r * weight[i];
        }
    else {
        for (int i = shiftIndex + 1; i < shiftIndex + amount; i++) {
            result += texture(tex, texCoords + vec2(0.0f, yTexSize * (i - shiftIndex))).r * weight[i];
            result += texture(tex, texCoords - vec2(0.0f, yTexSize * (i - shiftIndex))).r * weight[i];
        }
    }
    


    color = vec4(vec3(result), texture(tex,texCoords).w);
    //color = vec4(vec3(texture(tex, texCoords)), 1.0f);
    //color = vec4(1.0f);
}