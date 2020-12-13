#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D tex;
uniform bool horizontal;
uniform bool depthCheck;
uniform float focus;

const float xTexSize = 1.0f / 1200;
const float yTexSize = 1.0f / 800;
float amount = 5;
/*float weight[14] = float[] (0.25f, 0.18f, 0.12f, 0.05f, 0.025f,
                           0.30f, 0.18f, 0.12f, 0.05f,
                           0.45f, 0.25f, 0.05f,
                           0.70f, 0.15f); */

float weight[14] = float[] (0.25f, 0.18f, 0.12f, 0.05f, 0.025f,
                           0.30f, 0.18f, 0.12f, 0.05f,
                           0.45f, 0.22f, 0.055f,
                           0.70f, 0.15f);

void main()
{    
    int shiftIndex;
    vec3 result = vec3(1.0f);
    float depth = texture(tex, texCoords).w;
    float linearDepth;

    if (!depthCheck) {
        if (depth >= 0.96f) { amount = 2; shiftIndex = 12; }
        else if (depth >= 0.93) { amount = 3; shiftIndex = 9; }
        else if (depth >= 0.88) { amount = 4; shiftIndex = 5; }
        else { amount = 5; shiftIndex = 0; }

        if (amount == 0) result = texture(tex, texCoords).rgb; 
        else result = texture(tex, texCoords).rgb * weight[shiftIndex + 0];
        if (horizontal) 
            for (int i = shiftIndex + 1; i < shiftIndex + amount; i++) {
                result += texture(tex, texCoords + vec2(xTexSize * (i - shiftIndex), 0.0f)).rgb * weight[i];
                result += texture(tex, texCoords - vec2(xTexSize * (i - shiftIndex), 0.0f)).rgb * weight[i];
            }
        else {
            for (int i = shiftIndex + 1; i < shiftIndex + amount; i++) {
                result += texture(tex, texCoords + vec2(0.0f, yTexSize * (i - shiftIndex))).rgb * weight[i];
                result += texture(tex, texCoords - vec2(0.0f, yTexSize * (i - shiftIndex))).rgb * weight[i];
            }
        }
    }
    else {
        linearDepth = (2.0f * 0.1f * 20.0f) / (20.0f + 0.1f + (depth * 2.0f - 1.0f) * (20.0f - 0.1f));
        linearDepth /= 20.0f;
        linearDepth = 1.0f - linearDepth;
        linearDepth += focus;
        if (linearDepth >= 1.0f) linearDepth = 2.0f - linearDepth;


        if (linearDepth >= 0.95f) { amount = 0; }
        else if (linearDepth >= 0.91f) { amount = 2; shiftIndex = 12; }
        else if (linearDepth >= 0.87f) { amount = 3; shiftIndex = 9; }
        else if (linearDepth >= 0.84f) { amount = 4; shiftIndex = 5; }
        else { amount = 5; shiftIndex = 0; }

        if (amount == 0) result = texture(tex, texCoords).rgb; 
        else result = texture(tex, texCoords).rgb * weight[shiftIndex + 0];
        if (horizontal) 
            for (int i = shiftIndex + 1; i < shiftIndex + amount; i++) {
                result += texture(tex, texCoords + vec2(xTexSize * (i - shiftIndex), 0.0f)).rgb * weight[i];
                result += texture(tex, texCoords - vec2(xTexSize * (i - shiftIndex), 0.0f)).rgb * weight[i];
            }
        else {
            for (int i = shiftIndex + 1; i < shiftIndex + amount; i++) {
                result += texture(tex, texCoords + vec2(0.0f, yTexSize * (i - shiftIndex))).rgb * weight[i];
                result += texture(tex, texCoords - vec2(0.0f, yTexSize * (i - shiftIndex))).rgb * weight[i];
            }
        }
    }
    


    color = vec4(result, texture(tex, texCoords).w);
    //color = vec4(vec3(linearDepth), depth);
    //color = vec4(1.0f);
}