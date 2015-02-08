// Supported in 3.30 and 3.00 ES

layout(location = 0) in vec3 iVertexPos;

out vec3 fTexCoord;

uniform mat4 uMVP;

void main() {
  gl_Position = uMVP * vec4(iVertexPos, 1.0);
  fTexCoord = iVertexPos;
}
