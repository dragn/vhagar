#version 300 es

in lowp vec3 texCoord;

out lowp vec3 color;

uniform samplerCube skySampler;

void main() {
  color = texture(skySampler, texCoord).rgb;
}
