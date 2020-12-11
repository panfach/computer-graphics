#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D frameTexture;

const float xOffset = 1.0 / 600.0f;
const float yOffset = 1.0 / 400.0f;

const vec2 offsets[9] = vec2[](
        vec2(-xOffset,  yOffset), // top-left
        vec2( 0.0f,    yOffset), // top-center
        vec2( xOffset,  yOffset), // top-right
        vec2(-xOffset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( xOffset,  0.0f),   // center-right
        vec2(-xOffset, -yOffset), // bottom-left
        vec2( 0.0f,   -yOffset), // bottom-center
        vec2( xOffset, -yOffset)  // bottom-right    
);

/*const float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
);*/

const float kernel[9] = float[](
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
);

void main()
{
    vec3 result = vec3(0.0f);
    vec3 colors[9];
    for (int i = 0; i < 9; i++) {
        colors[i] = vec3(texture(frameTexture, texCoord.st + offsets[i]));
    }
    for (int i = 0; i < 9; i++) {
        result += colors[i] * kernel[i];
    }

    color = vec4(result, 1.0f);
    

    //color = texture(frameTexture, texCoord);
}