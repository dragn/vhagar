// Supported in 3.30 and 3.00 ES

in lowp vec3 fTexCoord;

out vec4 color;

uniform samplerCube uSkySampler;

void main() {
  color = texture(uSkySampler, fTexCoord);
}
