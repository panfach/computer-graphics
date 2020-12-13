#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D frameTexture;
uniform float time;
uniform bool floatEffect;

const float xOffset = 1.0 / 300.0f;
const float yOffset = 1.0 / 200.0f;

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
        0, 0, 0,
        0,  1, 0,
        0, 0, 0
);*/

/*const float kernel[9] = float[](
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
);*/

void main()
{ 
    vec3 result;

    /*float focus = 0.4f;
    float blurStrength = texture(frameTexture, texCoord).a * 5.0f - 4.0f;
    if (blurStrength < 0.05f) blurStrength = 0.05f;

    float kernel[9] = float[](
        1.0f * blurStrength / 16.0f, 2.0f * blurStrength / 16.0f, 1.0f * blurStrength / 16.0f,
        2.0f * blurStrength / 16.0f, (4.0f + 12.0f * (1.0f - blurStrength)) / 16.0f, 2.0f * blurStrength / 16.0f,
        1.0f * blurStrength / 16.0f, 2.0f * blurStrength / 16.0f, 1.0f * blurStrength / 16.0f
    );

    vec3 result = vec3(0.0f);
    vec3 colors[9];
    for (int i = 0; i < 9; i++) {
        colors[i] = vec3(texture(frameTexture, texCoord.st + offsets[i]));
    }
    for (int i = 0; i < 9; i++) {
        result += colors[i] * kernel[i];
    }*/
    
    if (floatEffect) {
        vec2 offset = vec2(0.0f);
        offset.x = 0.04f * sin(10.0f * texCoord.t + 4.0f * time);
        offset.y = 0.02f * sin(10.0f * texCoord.x + 4.0f * time);

        vec2 resultTexCoord = texCoord.st + offset;
        if (resultTexCoord.x < 0) resultTexCoord.x = 0;
        if (resultTexCoord.x > 1199) resultTexCoord.x = 1199;
        result = vec3(texture(frameTexture, texCoord.st + offset));
    }
    else {
        result = vec3(texture(frameTexture, texCoord.st));
    }


    color = vec4(result, result.z);
    //color = vec4(vec3(texture(frameTexture, texCoord.st)), 1.0f);
    //color = vec4(1.0f);
}