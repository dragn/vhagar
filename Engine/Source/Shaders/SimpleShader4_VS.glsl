// Supported in 3.30 and 3.00 ES

const int MAX_PLIGHTS = 10;

layout(location = 0) in vec4 iVertexPos;
layout(location = 1) in vec3 iVertexNormal;

layout(location = 2) in vec3 iAmbientColor;
layout(location = 3) in vec3 iDiffuseColor;
layout(location = 4) in vec3 iSpecularColor;

layout(location = 5) in vec2 iUV;

out vec3 fAmbientColor;
out vec3 fDiffuseColor;
out vec3 fSpecularColor;

out vec3 fNormal_cameraspace;
out vec3 fPLightDir_cameraspace[MAX_PLIGHTS];

out vec3 fEyeDirection_cameraspace;

out vec2 fUV;

uniform mat4 uMVP;
uniform mat4 uM;
uniform mat4 uV;

uniform int uPLightNum;                 // number of point lights
uniform vec3 uPLightPos[MAX_PLIGHTS];   // point lights positions
uniform float uPLightInt[MAX_PLIGHTS];  // point lights intencities

void main() {
  gl_Position = uMVP * iVertexPos;

  vec4 iVertexPos_cameraspace = uV * uM * iVertexPos;
  fEyeDirection_cameraspace = vec3(0) - iVertexPos_cameraspace.xyz;

  fNormal_cameraspace = (uV * uM * vec4(iVertexNormal, 0)).xyz;

  fAmbientColor = iAmbientColor;
  fDiffuseColor = iDiffuseColor;
  fSpecularColor = iSpecularColor;

  for (int i = 0; i < uPLightNum; i++)
  {
    fPLightDir_cameraspace[i] = (uV * vec4(uPLightPos[i], 1)).xyz + fEyeDirection_cameraspace;
  }

  fUV = iUV;
}
