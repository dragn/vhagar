// Supported in 3.30 and 3.00 ES

uniform vec3 uColor;

out lowp vec4 color;

void main() {
  color = vec4(uColor, 1.0);
}
