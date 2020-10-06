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
  EXPECT_NEAR(orthoPr[0U], 0.02, Eps);
  EXPECT_NEAR(orthoPr[1U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[2U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[3U], -1.0, Eps);
  EXPECT_NEAR(orthoPr[4U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[5U], 0.02, Eps);
  EXPECT_NEAR(orthoPr[6U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[7U], -1.0, Eps);
  EXPECT_NEAR(orthoPr[8U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[9U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[10U], -1.0, Eps);
  EXPECT_NEAR(orthoPr[11U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[12U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[13U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[14U], 0.0, Eps);
  EXPECT_NEAR(orthoPr[15U], 1.0, Eps);
}
