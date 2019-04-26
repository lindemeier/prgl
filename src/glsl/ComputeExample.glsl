#version 430

// Set the number of invocations in the work group.
// In this case, we operate on the image in 16x16 pixel tiles.
layout (local_size_x = 16, local_size_y = 16) in;

// Declare the texture inputs
layout (binding = 0, rgba32f) readonly uniform image2D fromTex;
layout (binding = 1, rgba32f) writeonly uniform image2D toTex;

uniform ivec2 offset;

void main() {
  ivec2 iSize = imageSize(fromTex);
  ivec2 texelCoords = ivec2(offset.x + gl_GlobalInvocationID.x,
                            iSize.y - (offset.y + gl_GlobalInvocationID.y)); // need to flip y since OpenGL is an asshole

  // Read the pixel from the first texture.
  vec4 pixel = imageLoad(fromTex, texelCoords);

  imageStore(toTex, texelCoords, vec4(pixel.x, pixel.y, 0.0, 1.0));
}