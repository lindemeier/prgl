/**
 * @file Types.hxx
 *
 * @author Thomas Lindemeier
 *
 * @brief
 *
 * @date 2012-10-04
 *
 */
#ifndef PRGL_TYPES_H
#define PRGL_TYPES_H

#include <array>

namespace prgl {

using vec2f = std::array<float, 2U>;
using vec3f = std::array<float, 3U>;
using vec4f = std::array<float, 4U>;

using vec2d = std::array<double, 2U>;
using vec3d = std::array<double, 3U>;
using vec4d = std::array<double, 4U>;

using vec2i = std::array<int32_t, 2U>;
using vec3i = std::array<int32_t, 3U>;
using vec4i = std::array<int32_t, 4U>;

using vec2ui = std::array<uint32_t, 2U>;
using vec3ui = std::array<uint32_t, 3U>;
using vec4ui = std::array<uint32_t, 4U>;

template <class T>
using mat4x4 = std::array<T, 4U * 4U>;
template <class T>
using mat3x3 = std::array<T, 3U * 3U>;

}  // namespace prgl

#endif  // PRGL_TYPES_H
