#version 300 es
in lowp vec2 UV;
in lowp vec3 fragmentColor;

out lowp vec3 color;

uniform sampler2D textureSampler;

void main() {
  color = fragmentColor;
}
