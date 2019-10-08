// Supported in 3.30 and 3.00 ES

const int MAX_PLIGHTS = 10;

in lowp vec2 fUV;

in lowp vec3 fAmbientColor;
in lowp vec3 fDiffuseColor;
in lowp vec3 fSpecularColor;

in lowp vec3 fNormal_cameraspace;
in lowp vec3 fPLightDir_cameraspace[MAX_PLIGHTS];
in lowp float fPLightDist[MAX_PLIGHTS];

in lowp vec3 fEyeDirection_cameraspace;
in lowp float fLightDistance;

out vec4 color;

uniform int uPLightNum;                 // number of point lights
uniform float uPLightInt[MAX_PLIGHTS];  // point lights intencities
uniform sampler2D textureSampler;

void main() {
  lowp vec3 n = normalize(fNormal_cameraspace);
  lowp vec3 E = normalize(fEyeDirection_cameraspace);

  vec3 diffuse = vec3(0);
  vec3 specular = vec3(0);

  for (int i = 0; i < uPLightNum; i++)
  {
    lowp vec3 l = normalize(fPLightDir_cameraspace[i]);
    lowp vec3 R = reflect(- l, n);

    lowp float cosAlpha = clamp(dot(E, R), 0, 1);
    lowp float cosTheta = clamp(dot(n, l), 0, 1);

    diffuse += (fDiffuseColor + texture(textureSampler, fUV).rgb) * cosTheta * uPLightInt[i] / (1.0f + fPLightDist[i]);

    // TODO take shininess from material properties
    specular += (fSpecularColor + texture(textureSampler, fUV).rgb) * pow(cosAlpha, 5) * uPLightInt[i] / (1.0f + fPLightDist[i]);
  }

  // TODO replace 0.1 with ambient color amount
  vec3 ambient = (fAmbientColor + texture(textureSampler, fUV).rgb) * 0.1; 

  // TODO apply gamma correction
  color = vec4(ambient + diffuse + specular, 1.0);
}
