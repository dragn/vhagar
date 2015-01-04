#version 300 es

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;

out vec3 fragmentColor;
out vec2 UV;

uniform mat4 MVP;

void main() {
  gl_Position = MVP * vec4(vertexPos, 1.0);
  fragmentColor = vertexColor;
  UV = vertexUV;
}
