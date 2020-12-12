#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D frameTexture;

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

const float kernel[9] = float[](
        0, 0, 0,
        0,  1, 0,
        0, 0, 0
);

/*const float kernel[9] = float[](
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
);*/

void main()
{ 
    //float focus = 0.4f;
    //float blurStrength = texture(frameTexture, texCoord).a * 5.0f - 4.0f;
    //if (blurStrength < 0.05f) blurStrength = 0.05f;

    /*float kernel[9] = float[](
        1.0f * blurStrength / 16.0f, 2.0f * blurStrength / 16.0f, 1.0f * blurStrength / 16.0f,
        2.0f * blurStrength / 16.0f, (4.0f + 12.0f * (1.0f - blurStrength)) / 16.0f, 2.0f * blurStrength / 16.0f,
        1.0f * blurStrength / 16.0f, 2.0f * blurStrength / 16.0f, 1.0f * blurStrength / 16.0f
    );*/

    /*
    vec3 result = vec3(0.0f);
    vec3 colors[9];
    for (int i = 0; i < 9; i++) {
        colors[i] = vec3(texture(frameTexture, texCoord.st + offsets[i]));
    }
    for (int i = 0; i < 9; i++) {
        result += colors[i] * kernel[i];
    }
    */


    /*for (int i=0; i<offsetSize; i++) {
		vec4 highSample = texture2D(texture, vTexCoord+offsets[i]*currentSize); //делаем выборку
		vec4 lowSample = texture2D(lowTexture, vTexCoord+offsets[i]*currentSize);
		float sampleSize = abs(highSample.a);//вычисляем силу размытия выбранной точки

		highSample.rgb = mix(highSample.rgb, lowSample.rgb, sampleSize); //смешиваем цвет размытой и оригинальной текстуры исходя из силы размытия
		highSample.a = highSample.a >= currentSize ? 1.0 : highSample.a; //корректировка весов (вклад, окторый вносит текущая выборка в результирующий цвет)

		sampleSize = abs(highSample.a); 
		resultColor.rgb += highSample.rgb * sampleSize; //суммируем цвет
		resultColor.a += sampleSize; //увеличиваем общий вес
	}*/



    //color = vec4(result, 1.0f);
    //color = vec4(texture(frameTexture, texCoord.st), 1.0f);
    color = vec4(1.0f);
}