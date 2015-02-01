// Supported in 3.30 and 3.00 ES

in lowp vec3 texCoord;

out lowp vec3 color;

uniform samplerCube skySampler;

void main() {
  color = texture(skySampler, texCoord).rgb;
}
