// Supported in 3.30 and 3.00 ES

in lowp vec2 fTexCoord;

out lowp vec4 color;

uniform sampler2D uSampler;

void main() {
  color = texture(uSampler, fTexCoord);
}
