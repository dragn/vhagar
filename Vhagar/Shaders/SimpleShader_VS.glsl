// Supported in 3.30 and 3.00 ES

layout(location = 0) in vec3 iVertexPos;
layout(location = 1) in vec3 iVertexNormal;

layout(location = 2) in vec3 iAmbientColor;
layout(location = 3) in vec3 iDiffuseColor;
layout(location = 4) in vec3 iSpecularColor;

out vec3 fAmbientColor;
out vec3 fDiffuseColor;
out vec3 fSpecularColor;

out vec3 fNormal_cameraspace;
out vec3 fLightDirection_cameraspace;
out vec3 fEyeDirection_cameraspace;

out float fLightDistance;
out float fLightIntensity;

uniform mat4 uMVP;
uniform mat4 uM;
uniform mat4 uV;

uniform vec3 uLightPosition;
uniform float uLightIntensity;

void main() {
  gl_Position = uMVP * vec4(iVertexPos, 1);

  vec3 position_worldspace = (uM * vec4(iVertexPos, 1)).xyz;

  fLightDistance = distance(position_worldspace, uLightPosition);

  vec3 iVertexPos_cameraspace = (uV * uM * vec4(iVertexPos, 1)).xyz;
  fEyeDirection_cameraspace = vec3(0) - iVertexPos_cameraspace;

  vec3 lightPosition_cameraspace = (uV * vec4(uLightPosition, 1)).xyz;
  fLightDirection_cameraspace = lightPosition_cameraspace + fEyeDirection_cameraspace;

  fNormal_cameraspace = (uV * uM * vec4(iVertexNormal, 0)).xyz;

  fAmbientColor = iAmbientColor;
  fDiffuseColor = iDiffuseColor;
  fSpecularColor = iSpecularColor;

  fLightIntensity = uLightIntensity;
}
