// Supported in 3.30 and 3.00 ES

in lowp vec2 UV;

in lowp vec3 fAmbientColor;
in lowp vec3 fDiffuseColor;
in lowp vec3 fSpecularColor;

in lowp vec3 fNormal_cameraspace;
in lowp vec3 fLightDirection_cameraspace;
in lowp vec3 fEyeDirection_cameraspace;
in lowp float fLightDistance;

in lowp float fLightIntensity;

out lowp vec3 color;

uniform sampler2D textureSampler;

void main() {
  lowp vec3 n = normalize(fNormal_cameraspace);
  lowp vec3 l = normalize(fLightDirection_cameraspace);

  lowp vec3 E = normalize(fEyeDirection_cameraspace);
  lowp vec3 R = reflect(- l, n);
  lowp float cosAlpha = clamp(dot(E, R), 0, 1);

  lowp float cosTheta = clamp(dot(n, l), 0, 1);

  lowp vec3 temp;

  if (fLightDistance > 0.0) {
    color = fAmbientColor * 0.1 +
      fDiffuseColor * cosTheta * fLightIntensity +
      fSpecularColor * pow(cosAlpha, 5) * fLightIntensity;
  }
}
