// Supported in 3.30 and 3.00 ES

in lowp vec3 fColor;

out lowp vec4 color;

void main() {
  color = vec4(fColor, 1.0);
}
