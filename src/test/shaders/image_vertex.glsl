#version 130

in vec2 vPos;
out vec2 targetPos;

void main() {
  vec2 flipped = (vPos + vec2(1.0)) * 0.5;        //texture coordinates
  targetPos = vec2(flipped.x, -flipped.y);
  gl_Position = vec4(vPos.x, -vPos.y, 0.0, 1.0);
}