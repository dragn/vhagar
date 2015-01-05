#version 300 es
in lowp vec2 UV;
in lowp vec3 fragmentColor;
in lowp vec3 Normal_cameraspace;
in lowp vec3 LightDirection_cameraspace;
in lowp vec3 EyeDirection_cameraspace;
in lowp float LightDistance;

out lowp vec3 color;

uniform sampler2D textureSampler;

void main() {
  lowp vec3 n = normalize(Normal_cameraspace);
  lowp vec3 l = normalize(LightDirection_cameraspace);

  lowp vec3 E = normalize(EyeDirection_cameraspace);
  lowp vec3 R = reflect(- l, n);
  lowp float cosAlpha = clamp(dot(E, R), 0, 1);  

  lowp float cosTheta = clamp(dot(n, l), 0, 1);
  lowp float LightPower = 500f;

  if (LightDistance > 0f) {
    color = fragmentColor * 0.1 +
      fragmentColor * (cosTheta + pow(cosAlpha, 5)) * LightPower / (LightDistance * LightDistance);
  }
}
