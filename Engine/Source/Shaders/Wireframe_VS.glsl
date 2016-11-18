// Supported in 3.30 and 3.00 ES

layout(location = 0) in vec3 iVertexPos;

uniform vec3 uColor;

uniform mat4 uMVP;

out vec3 fColor;

void main() {
  gl_Position = uMVP * vec4(iVertexPos, 1);
  gl_Position.z = 1.0;
  fColor = uColor;
}
