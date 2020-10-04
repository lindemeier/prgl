/**
 * @file ProjectionTest.cxx
 * @author thomas lindemeier
 *
 * @brief
 *
 * @date 2020-10-04
 *
 */

#include "gtest/gtest.h"
#include "prgl/Projection.hxx"

TEST(ProjectionTest, orthoTest) {
  const auto orthoPr =
    prgl::projection::ortho<double>(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);

  constexpr auto Eps = 0.00000001;
  EXPECT_NEAR(orthoPr[0U][0U], 0.02, Eps);
  EXPECT_NEAR(orthoPr[0U][1U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[0U][2U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[0U][3U], -1.0, Eps);
  EXPECT_NEAR(orthoPr[1U][0U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[1U][1U], 0.02, Eps);
  EXPECT_NEAR(orthoPr[1U][2U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[1U][3U], -1.0, Eps);
  EXPECT_NEAR(orthoPr[2U][0U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[2U][1U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[2U][2U], -1.0, Eps);
  EXPECT_NEAR(orthoPr[2U][3U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[3U][0U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[3U][1U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[3U][2U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[3U][3U], 1.0, Eps);
}
