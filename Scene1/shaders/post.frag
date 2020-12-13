#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D frameTexture;
uniform float time;
uniform bool floatEffect;

const float xOffset = 1.0 / 300.0f;
const float yOffset = 1.0 / 200.0f;

const vec2 offsets[9] = vec2[](                // ¬екторы направлений во все стороны
        vec2(-xOffset,  yOffset),             
        vec2( 0.0f,    yOffset),             
        vec2( xOffset,  yOffset),            
        vec2(-xOffset,  0.0f),  
        vec2( 0.0f,    0.0f),  
        vec2( xOffset,  0.0f),  
        vec2(-xOffset, -yOffset),
        vec2( 0.0f,   -yOffset),
        vec2( xOffset, -yOffset)  
);

const float kernel[9] = float[](                    // ядро свертки пост эффекта
        -1.0f, -1.0f, -1.0f,
        -1.0f, 9.0f, -1.0f,
        -1.0f, -1.0f, -1.0f
);

void main()
{ 
    vec4 result;

    
    if (floatEffect) {
        
        vec4 result1 = vec4(0.0f);
        for (int i = 0; i < 9; i++) {
            result1 += texture(frameTexture, texCoord.xy + 2.0f * sin(time) * offsets[i]) * kernel[i];
        }

        vec2 offset = vec2(0.0f);
        offset.x = 0.04f * sin(10.0f * texCoord.t + 4.0f * time);
        offset.y = 0.02f * sin(10.0f * texCoord.x + 4.0f * time);

        vec2 resultTexCoord = texCoord.st + offset;
        if (resultTexCoord.x < 0) resultTexCoord.x = 0;
        if (resultTexCoord.x > 1199) resultTexCoord.x = 1199;
        result = texture(frameTexture, texCoord.st + offset);
        result = mix(result, result1, 0.2f);
    }
    else {
        result = texture(frameTexture, texCoord.st);
    }


    color = vec4(result);
}