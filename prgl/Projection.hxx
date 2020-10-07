/**
 * @file Projection.hxx
 *
 * @author Thomas Lindemeier
 *
 * @brief
 *
 * @date 2012-10-04
 *
 */
#ifndef PRGL_PROJECTION_H
#define PRGL_PROJECTION_H

#include <cmath>
#include <limits>
#include <stdexcept>

#include "prgl/Types.hxx"

namespace prgl::projection {
/**
 * @brief
 *
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 *
 * @return mat4x4<T> in opengl format already
 */
template <class T>
auto ortho(const T left, const T right, const T bottom, const T top,
           const T near, const T far) -> mat4x4<T> {
  const auto width  = (right - left);
  const auto height = (top - bottom);
  const auto zclip  = (far - near);
  if (std::fabs(width) <
      (static_cast<T>(100.0) * std::numeric_limits<T>::epsilon())) {
    throw std::invalid_argument("left and right are not allowed to be equal");
  }
  if (std::fabs(height) <
      (static_cast<T>(100.0) * std::numeric_limits<T>::epsilon())) {
    throw std::invalid_argument("top and bottom are not allowed to be equal");
  }
  if (std::fabs(zclip) <
      (static_cast<T>(100.0) * std::numeric_limits<T>::epsilon())) {
    throw std::invalid_argument("near and far are not allowed to be equal");
  }
  constexpr auto Zero = static_cast<T>(0.0);
  constexpr auto Two  = static_cast<T>(2.0);

  return {Two / width, Zero,         Zero,         -(left + right) / width,
          Zero,        Two / height, Zero,         -(top + bottom) / height,
          Zero,        Zero,         -Two / zclip, -(near + far) / zclip,
          Zero,        Zero,         Zero,         static_cast<T>(1.0)};
}
}  // namespace prgl::projection

#endif  // PRGL_PROJECTION_H
