#version 130

uniform sampler2D inputImage;

in vec2 targetPos;

void main() {
  vec4 v = texture2D(inputImage, targetPos);
  gl_FragColor = v;
}