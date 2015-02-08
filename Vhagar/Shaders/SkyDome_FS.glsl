// Supported in 3.30 and 3.00 ES

in lowp vec3 fTexCoord;

out lowp vec3 color;

uniform samplerCube uSkySampler;

void main() {
  color = texture(uSkySampler, fTexCoord).rgb;
}
