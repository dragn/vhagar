// Supported in 3.30 and 3.00 ES

layout(location = 0) in vec2 iVertexPos;

uniform vec4 uBounds; // x, y, width, height

out vec2 fTexCoord;

void main() {
  gl_Position.xy = iVertexPos;
  gl_Position.z = 0.1;
  gl_Position.w = 1.0;
  fTexCoord = vec2(
    (iVertexPos.x - uBounds.x) / uBounds.z,
    1 - (iVertexPos.y - 1 + uBounds.y) / uBounds.w
  );
  //fTexCoord = iVertexPos;
}
