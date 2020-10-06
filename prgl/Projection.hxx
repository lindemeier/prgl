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
  if (std::fabs(right - left) <
      (static_cast<T>(100.0) * std::numeric_limits<T>::epsilon())) {
    throw std::invalid_argument("left and right are not allowed to be equal");
  }
  if (std::fabs(top - bottom) <
      (static_cast<T>(100.0) * std::numeric_limits<T>::epsilon())) {
    throw std::invalid_argument("top and bottom are not allowed to be equal");
  }
  if (std::fabs(near - far) <
      (static_cast<T>(0.0) * std::numeric_limits<T>::epsilon())) {
    throw std::invalid_argument("near and far are not allowed to be equal");
  }
  constexpr auto Zero = static_cast<T>(0.0);
  constexpr auto One  = static_cast<T>(1.0);
  constexpr auto Two  = static_cast<T>(2.0);
  // clang-format off
  return {
       Two / (right - left),
       Zero,
       Zero,
       Zero,

       Zero,
       Two / (bottom - top),
       Zero,
       Zero,

       Zero,
       Zero,
       One / (near - far),
       Zero,

       -(right + left) / (right - left),
       -(bottom + top) / (bottom - top),
       near / (near - far),
       One
    };
  // clang-format off
  }
}  // namespace prgl::projection

#endif  // PRGL_PROJECTION_H
