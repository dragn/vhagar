// Supported in 3.30 and 3.00 ES

in lowp vec2 fUV;

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

  // TODO replace 0.1 with ambient color amount
  vec3 ambient = fAmbientColor * 0.1; 

  // TODO apply multiple light sources
  vec3 diffuse = (fDiffuseColor + texture(textureSampler, fUV).rgb) * cosTheta * fLightIntensity;

  // TODO apply multiple lights, take shininess from material properties
  vec3 specular = fSpecularColor * pow(cosAlpha, 5) * fLightIntensity;

  // TODO apply gamma correction
  color = ambient + diffuse + specular;
}
