// Supported in 3.30 and 3.00 ES

in lowp vec2 fTexCoord;
in lowp float fTime;

out lowp vec4 color;

uniform sampler2D uSampler;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float rand(float f)
{
    return f * 43758.5453;
}

float drop(float f, float time)
{
    float shift = fract(time);

    float r1 = fract(trunc(time + 15234.0) * 0.9412);
    float r2 = fract(trunc(time + 6134.0) * 0.7162);

    float dist = length(fTexCoord - vec2(r1, r2));

    if (dist < 0.2 * shift)
    {
        return 0.9 + 0.2 * sin(shift) * f;
    }

    return f;
}

void main() {
    float time = fTime * 5.0f;

    float a = mix(
        0.5f + 0.25f * sin(time * 0.5f),
        0.5f + 0.25f * sin(time * 0.25f),
        length(fTexCoord - vec2(0.5, 0.5))
    );

    float b = mix(
        0.5f + 0.25f * sin(time * 0.5f),
        0.5f + 0.25f * sin(time),
        length(fTexCoord - vec2(0.5, 0.5))
    );

    float c = mix(
        0.5f + 0.25f * sin(time * 0.5f),
        0.5f + 0.25f * sin(time * 0.7f),
        length(fTexCoord - vec2(0.5, 0.5))
    );

    float mult = step(
        length(fTexCoord - vec2(0.5, 0.5)),
        0.5);

/*
    mult = drop(mult, fTime);
    mult = drop(mult, fTime + 68134.44);
    mult = drop(mult, fTime + 6814.79);
    mult = drop(mult, fTime + 41214.12);
    mult = drop(mult, fTime + 5162.50);
*/

    color = mult * vec4(
        a + 0.1f * rand(fTexCoord),
        b + 0.1f * rand(fTexCoord),
        c + 0.1f * rand(fTexCoord),
    1.0f);
}
