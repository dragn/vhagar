#version 300 es
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColor;

uniform mat4 MVP;

out vec3 fragmentColor;

void main() {
  vec4 v = vec4(vertexPos, 1.0);
  gl_Position = MVP * v;
  fragmentColor = vertexColor;
}
