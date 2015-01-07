#version 300 es

layout(location = 0) in vec3 vertexPos_modelspace;
//layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal_modelspace;

out vec3 fragmentColor;
out vec3 Normal_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 EyeDirection_cameraspace;
out float LightDistance;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform vec3 LightPosition_worldspace;

void main() {
  gl_Position = MVP * vec4(vertexPos_modelspace, 1);

  vec3 Position_worldspace = (M * vec4(vertexPos_modelspace, 1)).xyz;

  LightDistance = distance(Position_worldspace, LightPosition_worldspace);

  vec3 vertexPos_cameraspace = (V * M * vec4(vertexPos_modelspace, 1)).xyz;
  EyeDirection_cameraspace = vec3(0) - vertexPos_cameraspace;

  vec3 LightPosition_cameraspace = (V * vec4(LightPosition_worldspace, 1)).xyz;
  LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

  Normal_cameraspace = (V * M * vec4(vertexNormal_modelspace, 0)).xyz;

  fragmentColor = vec3(0.5, 0.5, 0.5);
}
