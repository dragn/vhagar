// Supported in 3.30 and 3.00 ES

layout(location = 0) in vec3 vertexPos;

out vec3 texCoord;

uniform mat4 MVP;

void main() {
  gl_Position = MVP * vec4(vertexPos, 1.0);
  //gl_Position.xyz = vertexPos;
  texCoord = vertexPos;
}
