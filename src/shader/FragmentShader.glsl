#version 300 es
in lowp vec3 fragmentColor;

out lowp vec3 color;

void main() {
  color = fragmentColor;
}
