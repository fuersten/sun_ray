//
//  tuple_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 26.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/matrix.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("create 2x2 matrix", "[matrix 2x2]")
{
  SECTION("create matrix")
  {
    // clang-format off
    sunray::Matrix22 m{{ -3,  5,
                          1, -2 }};
    // clang-format on
    CHECK(m[0] == Approx(-3));
    CHECK(m[1] == Approx(5));
    CHECK(m[2] == Approx(1));
    CHECK(m[3] == Approx(-2));
  }
  SECTION("create matrix mat")
  {
    // clang-format off
    sunray::Matrix22::Mat m{ -3,  5,
                              1, -2 };
    // clang-format on
    sunray::Matrix22 matrix{m};
    CHECK(matrix[0] == Approx(-3));
    CHECK(matrix[1] == Approx(5));
    CHECK(matrix[2] == Approx(1));
    CHECK(matrix[3] == Approx(-2));
  }
}

TEST_CASE("access 2x2 matrix", "[matrix 2x2]")
{
  SECTION("access matrix out of range")
  {
    // clang-format off
    sunray::Matrix22 m{{ -3,  5,
                          1, -2 }};
    // clang-format on
    CHECK_THROWS_AS(m[4], std::out_of_range);
  }
}

TEST_CASE("compare 2x2 matrix", "[matrix 2x2]")
{
  SECTION("equal matrix")
  {
    // clang-format off
    sunray::Matrix22::Mat m{ 1, 2,
                             3, 4 };
    // clang-format on
    sunray::Matrix22 a{m};
    sunray::Matrix22 b{m};

    CHECK(a == b);
    CHECK(b == a);
  }
  SECTION("not equal matrix")
  {
    // clang-format off
    sunray::Matrix22::Mat m1{ 1, 2,
                              3, 4 };
    sunray::Matrix22::Mat m2{ 2, 3,
                              4, 5 };
    sunray::Matrix22::Mat m3{ 5, 6,
                              7, 8 };
    // clang-format on
    sunray::Matrix22 a{m1};
    sunray::Matrix22 b{m2};
    sunray::Matrix22 c{m3};

    CHECK(a != b);
    CHECK(b != a);
    CHECK(a != c);
  }
}

TEST_CASE("stream 2x2 matrix", "[matrix 2x2]")
{
  SECTION("stream matrix")
  {
    // clang-format off
    sunray::Matrix22 a{{ 1, 2,
                         3, 4 }};
    // clang-format on
    std::stringstream ss;
    ss << a;
    CHECK(ss.str() ==
          // clang-format off
R"(| 1 | 2 |
| 3 | 4 |
)");
    // clang-format on
  }
}

TEST_CASE("determinant 2x2 matrix", "[matrix 2x2]")
{
  SECTION("matrix determinant")
  {
    // clang-format off
    sunray::Matrix22 m{{ 1, 5,
                        -3, 2 }};
    // clang-format on
    CHECK(m.determinant() == Approx(17));
  }
}

TEST_CASE("create 3x3 matrix", "[matrix 3x3]")
{
  SECTION("create matrix")
  {
    // clang-format off
    sunray::Matrix33 matrix{{ -3, 5, 0,
                               1,-2,-7,
                               0, 1, 1 }};
    // clang-format on
    CHECK(matrix[0] == Approx(-3));
    CHECK(matrix[1] == Approx(5));
    CHECK(matrix[2] == Approx(0));
    CHECK(matrix[3] == Approx(1));
    CHECK(matrix[4] == Approx(-2));
    CHECK(matrix[5] == Approx(-7));
    CHECK(matrix[6] == Approx(0));
    CHECK(matrix[7] == Approx(1));
    CHECK(matrix[8] == Approx(1));
  }
  SECTION("create matrix mat")
  {
    // clang-format off
    sunray::Matrix33::Mat m{ -3, 5, 0,
                              1,-2,-7,
                              0, 1, 1 };
    // clang-format on
    sunray::Matrix33 matrix{m};
    CHECK(matrix[0] == Approx(-3));
    CHECK(matrix[1] == Approx(5));
    CHECK(matrix[2] == Approx(0));
    CHECK(matrix[3] == Approx(1));
    CHECK(matrix[4] == Approx(-2));
    CHECK(matrix[5] == Approx(-7));
    CHECK(matrix[6] == Approx(0));
    CHECK(matrix[7] == Approx(1));
    CHECK(matrix[8] == Approx(1));
  }
}

TEST_CASE("access 3x3 matrix", "[matrix 3x3]")
{
  SECTION("access matrix out of range")
  {
    // clang-format off
    sunray::Matrix33 m{{ -3, 5, 0,
                          1,-2,-7,
                          0, 1, 1 }};
    // clang-format on
    CHECK_THROWS_AS(m[9], std::out_of_range);
  }
}

TEST_CASE("compare 3x3 matrix", "[matrix 3x3]")
{
  SECTION("equal matrix")
  {
    // clang-format off
    sunray::Matrix33::Mat m{ -3, 5, 0,
                              1,-2,-7,
                              0, 1, 1 };
    // clang-format on
    sunray::Matrix33 a{m};
    sunray::Matrix33 b{m};

    CHECK(a == b);
    CHECK(b == a);
  }
  SECTION("not equal matrix")
  {
    // clang-format off
    sunray::Matrix33::Mat m1{ -3, 5, 0,
                               1,-2,-7,
                               0, 1, 1 };
    sunray::Matrix33::Mat m2{  2, 3, 4,
                               5, 6, 7,
                               8, 9, 1 };
    sunray::Matrix33::Mat m3{  2, 3, 4,
                               5, 6, 7,
                               8, 9, 3 };
    // clang-format on
    sunray::Matrix33 a{m1};
    sunray::Matrix33 b{m2};
    sunray::Matrix33 c{m3};

    CHECK(a != b);
    CHECK(b != a);
    CHECK(b != c);
  }
}

TEST_CASE("submatrix 3x3 matrix", "[matrix 3x3]")
{
  SECTION("submatrix")
  {
    // clang-format off
    sunray::Matrix33 a{{  1, 5,  0,
                         -3, 2,  7,
                          0, 6, -3 }};
    CHECK(a.submatrix(0, 2) == sunray::Matrix22{{ -3, 2,
                                                   0, 6
    }});
    CHECK(a.submatrix(0, 0) == sunray::Matrix22{{ 2,  7,
                                                  6, -3
    }});
    CHECK(a.submatrix(0, 1) == sunray::Matrix22{{ -3,  7,
                                                  0, -3
    }});
    CHECK(a.submatrix(1, 1) == sunray::Matrix22{{ 1,  0,
                                                  0, -3
    }});
    CHECK(a.submatrix(2, 2) == sunray::Matrix22{{  1, 5,
                                                  -3, 2
    }});
    // clang-format on
  }
  SECTION("submatrix out of range")
  {
    // clang-format off
    sunray::Matrix33 a{{  1, 5,  0,
                         -3, 2,  7,
                          0, 6, -3 }};
    // clang-format on
    CHECK_THROWS_AS(a.submatrix(3, 2), std::out_of_range);
    CHECK_THROWS_AS(a.submatrix(2, 3), std::out_of_range);
  }
}

TEST_CASE("minor 3x3 matrix", "[matrix 3x3]")
{
  SECTION("minor")
  {
    // clang-format off
    sunray::Matrix33 a{{ 3, 5, 0,
                         2,-1,-7,
                         6,-1, 5 }};

    // clang-format on
    CHECK(a.submatrix(1, 0).determinant() == Approx(25));
    CHECK(a.minor(1, 0) == Approx(25));
  }
}

TEST_CASE("cofactor 3x3 matrix", "[matrix 3x3]")
{
  SECTION("cofactor")
  {
    // clang-format off
    sunray::Matrix33 a{{ 3, 5, 0,
                         2,-1,-7,
                         6,-1, 5 }};

    // clang-format on
    CHECK(a.minor(0, 0) == Approx(-12));
    CHECK(a.cofactor(0, 0) == Approx(-12));
    CHECK(a.minor(1, 0) == Approx(25));
    CHECK(a.cofactor(1, 0) == Approx(-25));

    // clang-format off
    sunray::Matrix33 b{{ 1, 2, 6,
                        -5, 8,-4,
                         2, 6, 4 }};
    // clang-format on
    CHECK(b.cofactor(0, 0) == Approx(56));
    CHECK(b.cofactor(0, 1) == Approx(12));
    CHECK(b.cofactor(0, 2) == Approx(-46));
  }
}

TEST_CASE("determinant 3x3 matrix", "[matrix 3x3]")
{
  SECTION("determinant")
  {
    // clang-format off
    sunray::Matrix33 a{{ 1, 2, 6,
                        -5, 8,-4,
                         2, 6, 4 }};
    // clang-format on
    CHECK(a.cofactor(0, 0) == Approx(56));
    CHECK(a.cofactor(0, 1) == Approx(12));
    CHECK(a.cofactor(0, 2) == Approx(-46));
    CHECK(a.determinant() == Approx(-196));
  }
}

TEST_CASE("stream 3x3 matrix", "[matrix 3x3]")
{
  SECTION("stream matrix")
  {
    // clang-format off
    sunray::Matrix33 a{{ -3, 5, 0,
                          1,-2,-7,
                          0, 1, 1 }};
    // clang-format on
    std::stringstream ss;
    ss << a;
    CHECK(ss.str() ==
          // clang-format off
R"(| -3 | 5 | 0 |
| 1 | -2 | -7 |
| 0 | 1 | 1 |
)");
    // clang-format on
  }
}

TEST_CASE("create 4x4 matrix", "[matrix 4x4]")
{
  SECTION("create matrix")
  {
    // clang-format off
    sunray::Matrix44 matrix{{   1,    2,    3,    4,
                              5.5,  6.5,  7.5,  8.5,
                                9,   10,   11,   12,
                             13.5, 14.5, 15.5, 16.5 }};
    // clang-format on
    CHECK(matrix[{0, 0}] == Approx(1));
    CHECK(matrix[{0, 1}] == Approx(2));
    CHECK(matrix[{0, 2}] == Approx(3));
    CHECK(matrix[{0, 3}] == Approx(4));
    CHECK(matrix[{1, 0}] == Approx(5.5));
    CHECK(matrix[{1, 1}] == Approx(6.5));
    CHECK(matrix[{1, 2}] == Approx(7.5));
    CHECK(matrix[{1, 3}] == Approx(8.5));
    CHECK(matrix[{2, 0}] == Approx(9));
    CHECK(matrix[{2, 1}] == Approx(10));
    CHECK(matrix[{2, 2}] == Approx(11));
    CHECK(matrix[{2, 3}] == Approx(12));
    CHECK(matrix[{3, 0}] == Approx(13.5));
    CHECK(matrix[{3, 1}] == Approx(14.5));
    CHECK(matrix[{3, 2}] == Approx(15.5));
    CHECK(matrix[{3, 3}] == Approx(16.5));
  }
  SECTION("create matrix mat")
  {
    // clang-format off
    sunray::Matrix44::Mat m{    1,    2,    3,    4,
                              5.5,  6.5,  7.5,  8.5,
                                9,   10,   11,   12,
                             13.5, 14.5, 15.5, 16.5};
    // clang-format on
    sunray::Matrix44 matrix{m};
    CHECK(matrix[{0, 0}] == Approx(1));
    CHECK(matrix[{0, 1}] == Approx(2));
    CHECK(matrix[{0, 2}] == Approx(3));
    CHECK(matrix[{0, 3}] == Approx(4));
    CHECK(matrix[{1, 0}] == Approx(5.5));
    CHECK(matrix[{1, 1}] == Approx(6.5));
    CHECK(matrix[{1, 2}] == Approx(7.5));
    CHECK(matrix[{1, 3}] == Approx(8.5));
    CHECK(matrix[{2, 0}] == Approx(9));
    CHECK(matrix[{2, 1}] == Approx(10));
    CHECK(matrix[{2, 2}] == Approx(11));
    CHECK(matrix[{2, 3}] == Approx(12));
    CHECK(matrix[{3, 0}] == Approx(13.5));
    CHECK(matrix[{3, 1}] == Approx(14.5));
    CHECK(matrix[{3, 2}] == Approx(15.5));
    CHECK(matrix[{3, 3}] == Approx(16.5));
  }
}

TEST_CASE("access 4x4 matrix", "[matrix 4x4]")
{
  SECTION("access matrix")
  {
    // clang-format off
    sunray::Matrix44 matrix{{   1,    2,    3,    4,
                              5.5,  6.5,  7.5,  8.5,
                                9,   10,   11,   12,
                             13.5, 14.5, 15.5, 16.5 }};
    // clang-format on
    CHECK_THROWS_AS(matrix[16], std::out_of_range);
  }
}

TEST_CASE("compare 4x4 matrix", "[matrix 4x4]")
{
  SECTION("equal matrix")
  {
    // clang-format off
    sunray::Matrix44::Mat m{ 1, 2, 3, 4,
                             5, 6, 7, 8,
                             9, 8, 7, 6,
                             5, 4, 3, 2 };
    // clang-format on
    sunray::Matrix44 a{m};
    sunray::Matrix44 b{m};

    CHECK(a == b);
    CHECK(b == a);
  }
  SECTION("not equal matrix")
  {
    // clang-format off
    sunray::Matrix44::Mat m1{ 1, 2, 3, 4,
                              5, 6, 7, 8,
                              9, 8, 7, 6,
                              5, 4, 3, 2 };
    sunray::Matrix44::Mat m2{ 2, 3, 4, 5,
                              6, 7, 8, 9,
                              8, 7, 6, 5,
                              4, 3, 2, 1 };
    sunray::Matrix44::Mat m3{ 1, 2, 3, 4,
                              5, 6, 7, 8,
                              9, 8, 7, 6,
                              5, 4, 3, 1 };
    // clang-format on
    sunray::Matrix44 a{m1};
    sunray::Matrix44 b{m2};
    sunray::Matrix44 c{m3};

    CHECK(a != b);
    CHECK(b != a);
    CHECK(a != c);
  }
}

TEST_CASE("copy 4x4 matrix", "[matrix 4x4]")
{
  SECTION("copy matrix")
  {
    // clang-format off
    sunray::Matrix44::Mat m{ 1, 2, 3, 4,
                             5, 6, 7, 8,
                             9, 8, 7, 6,
                             5, 4, 3, 2 };
    // clang-format on
    sunray::Matrix44 a{m};
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    sunray::Matrix44 b{a};

    CHECK(a == b);
    CHECK(b == a);
  }
  SECTION("move matrix")
  {
    // clang-format off
    sunray::Matrix44::Mat m{ 1, 2, 3, 4,
                             5, 6, 7, 8,
                             9, 8, 7, 6,
                             5, 4, 3, 2 };
    // clang-format on
    sunray::Matrix44 a{m};

    // clang-format off
    sunray::Matrix44 b{{ 1, 2, 3, 4,
                         5, 6, 7, 8,
                         9, 8, 7, 6,
                         5, 4, 3, 2 }};
    // clang-format on
    sunray::Matrix44 c{std::move(b)};

    CHECK(a == c);
    CHECK(c == a);
  }
}

TEST_CASE("stream 4x4 matrix", "[matrix 4x4]")
{
  SECTION("stream matrix")
  {
    // clang-format off
    sunray::Matrix44 a{{ 1, 2, 3, 4,
                         5, 6, 7, 8,
                         9, 8, 7, 6,
                         5, 4, 3, 2 }};
    // clang-format on
    std::stringstream ss;
    ss << a;
    // clang-format off
    CHECK(ss.str() ==
R"(| 1 | 2 | 3 | 4 |
| 5 | 6 | 7 | 8 |
| 9 | 8 | 7 | 6 |
| 5 | 4 | 3 | 2 |
)");
    // clang-format on
  }
}

TEST_CASE("multiply 4x4 matrix", "[matrix 4x4]")
{
  SECTION("multiply matrix with matrix")
  {
    // clang-format off
    sunray::Matrix44 a{{ 1, 2, 3, 4,
                         5, 6, 7, 8,
                         9, 8, 7, 6,
                         5, 4, 3, 2 }};
    sunray::Matrix44 b{{ -2, 1, 2, 3,
                          3, 2, 1, -1,
                          4, 3, 6, 5,
                          1, 2, 7, 8 }};
    // clang-format on
    auto c = a * b;
    // clang-format off
    CHECK(c == sunray::Matrix44{{ 20, 22,  50,  48,
                                  44, 54, 114, 108,
                                  40, 58, 110, 102,
                                  16, 26,  46,  42 }});
    // clang-format on
  }
  SECTION("multiply matrix with point")
  {
    // clang-format off
    sunray::Matrix44 a{{ 1, 2, 3, 4,
                         2, 4, 4, 2,
                         8, 6, 4, 1,
                         0, 0, 0, 1 }};
    // clang-format on
    auto b = sunray::create_point(1, 2, 3);
    auto c = a * b;
    CHECK(c == sunray::create_point(18, 24, 33));
  }
  SECTION("multiply matrix with vector")
  {
    // clang-format off
    sunray::Matrix44 a{{ 1, 2, 3, 4,
                         2, 4, 4, 2,
                         8, 6, 4, 1,
                         0, 0, 0, 1 }};
    // clang-format on
    auto b = sunray::create_vector(1, 2, 3);
    auto c = a * b;
    CHECK(c == sunray::create_vector(14, 22, 32));
  }
}

TEST_CASE("4x4 identity matrix", "[matrix 4x4]")
{
  SECTION("multiply matrix with identity")
  {
    // clang-format off
    sunray::Matrix44 a{{ 0, 1,  2,  4,
                         1, 2,  4,  8,
                         2, 4,  8, 16,
                         4, 8, 16, 32 }};
    // clang-format on
    auto c = a * sunray::Matrix44::identity();
    // clang-format off
    CHECK(c == sunray::Matrix44{{ 0, 1,  2,  4,
                                  1, 2,  4,  8,
                                  2, 4,  8, 16,
                                  4, 8, 16, 32 }});
    // clang-format on
  }
  SECTION("transpose identity matrix")
  {
    CHECK(sunray::Matrix44::identity().transpose() == sunray::Matrix44::identity());
  }
}

TEST_CASE("transpose 4x4 matrix", "[matrix 4x4]")
{
  SECTION("transpose matrix")
  {
    // clang-format off
    sunray::Matrix44 a{{ 0, 9, 3, 0,
                         9, 8, 0, 8,
                         1, 8, 5, 3,
                         0, 0, 5, 8 }};
    CHECK(a.transpose() == sunray::Matrix44{{ 0, 9, 1, 0,
                                              9, 8, 8, 0,
                                              3, 0, 5, 5,
                                              0, 8, 3, 8 }});
    sunray::Matrix44 b{{  0,  1,  2,  3,
                          4,  5,  6,  7,
                          8,  9, 10, 11,
                         12, 13, 14, 15 }};
    CHECK(b.transpose() == sunray::Matrix44{{ 0, 4,  8, 12,
                                              1, 5,  9, 13,
                                              2, 6, 10, 14,
                                              3, 7, 11, 15 }});
    // clang-format on
  }
}

TEST_CASE("submatrix 4x4 matrix", "[matrix 4x4]")
{
  SECTION("submatrix")
  {
    // clang-format off
    sunray::Matrix44 a{{ -6, 1,  1, 6,
                         -8, 5,  8, 6,
                         -1, 0,  8, 2,
                         -7, 1, -1, 1 }};
    CHECK(a.submatrix(2, 1) == sunray::Matrix33{{ -6,  1, 6,
                                                  -8,  8, 6,
                                                  -7, -1, 1
    }});
    CHECK(a.submatrix(0, 0) == sunray::Matrix33{{ 5, 8, 6,
                                                  0, 8, 2,
                                                  1,-1, 1
    }});
    CHECK(a.submatrix(2, 3) == sunray::Matrix33{{-6, 1, 1,
                                                 -8, 5, 8,
                                                 -7, 1,-1
    }});
    CHECK(a.submatrix(0, 2) == sunray::Matrix33{{-8, 5, 6,
                                                 -1, 0, 2,
                                                 -7, 1, 1
    }});
    CHECK(a.submatrix(3, 2) == sunray::Matrix33{{-6, 1, 6,
                                                 -8, 5, 6,
                                                 -1, 0, 2
    }});
    // clang-format on
  }
  SECTION("submatrix out of range")
  {
    // clang-format off
    sunray::Matrix44 a{{ -6, 1,  1, 6,
                         -8, 5,  8, 6,
                         -1, 0,  8, 2,
                         -7, 1, -1, 1 }};
    // clang-format on
    CHECK_THROWS_AS(a.submatrix(4, 3), std::out_of_range);
    CHECK_THROWS_AS(a.submatrix(3, 4), std::out_of_range);
  }
}

TEST_CASE("determinant 4x4 matrix", "[matrix 4x4]")
{
  SECTION("determinant")
  {
    // clang-format off
    sunray::Matrix44 a{{ -2,-8, 3, 5,
                         -3, 1, 7, 3,
                          1, 2,-9, 6,
                         -6, 7, 7,-9 }};
    // clang-format on
    CHECK(a.cofactor(0, 0) == Approx(690));
    CHECK(a.cofactor(0, 1) == Approx(447));
    CHECK(a.cofactor(0, 2) == Approx(210));
    CHECK(a.cofactor(0, 3) == Approx(51));
    CHECK(a.determinant() == Approx(-4071));
  }
}

TEST_CASE("inversion 4x4 matrix", "[matrix 4x4]")
{
  SECTION("is invertible")
  {
    // clang-format off
    sunray::Matrix44 a{{ 6, 4, 4, 4,
                         5, 5, 7, 6,
                         4,-9, 3,-7,
                         9, 1, 7,-6 }};
    // clang-format on
    CHECK(a.is_invertible());

    // clang-format off
    sunray::Matrix44 b{{-4, 2,-2,-3,
                         9, 6, 2, 6,
                         0,-5, 1,-5,
                         0, 0, 0, 0 }};
    // clang-format on
    CHECK_FALSE(b.is_invertible());
  }
  SECTION("inverse")
  {
    // clang-format off
    sunray::Matrix44 a{{-5, 2, 6,-8,
                         1,-5, 1, 8,
                         7, 7,-6,-7,
                         1,-3, 7, 4 }};
    // clang-format on
    CHECK(a.determinant() == Approx(532.0));
    CHECK(a.cofactor(2, 3) == Approx(-160.0));
    CHECK(a.cofactor(3, 2) == Approx(105.0));

    auto b = a.inverse();
    CHECK(b[{3, 2}] == Approx(-160.0 / 532.0));
    CHECK(b[{2, 3}] == Approx(105.0 / 532.0));
    // clang-format off
    sunray::Matrix44 bb{{ 0.21805, 0.45113, 0.24060,-0.04511,
                         -0.80827,-1.45677,-0.44361, 0.52068,
                         -0.07895,-0.22368,-0.05263, 0.19737,
                         -0.52256,-0.81391,-0.30075, 0.30639 }};
    // clang-format on
    CHECK(b == bb);

    // clang-format off
    sunray::Matrix44 c{{ 8,-5, 9, 2,
                         7, 5, 6, 1,
                        -6, 0, 9, 6,
                        -3, 0,-9,-4 }};
    CHECK(c.inverse() ==
          sunray::Matrix44{{-0.15385,-0.15385,-0.28205,-0.53846,
                            -0.07692, 0.12308, 0.02564, 0.03077,
                             0.35897, 0.35897, 0.43590, 0.92308,
                            -0.69231,-0.69231,-0.76923,-1.92308 }});
    sunray::Matrix44 d{{ 9, 3, 0, 9,
                        -5,-2,-6,-3,
                        -4, 9, 6, 4,
                        -7, 6, 6, 2 }};
    CHECK(d.inverse() ==
          sunray::Matrix44{{-0.04074,-0.07778, 0.14444,-0.22222,
                            -0.07778, 0.03333, 0.36667,-0.33333,
                            -0.02901,-0.14630,-0.10926, 0.12963,
                             0.17778, 0.06667,-0.26667, 0.33333 }});
    // clang-format on
  }
  SECTION("not invertible")
  {
    // clang-format off
    sunray::Matrix44 b{{-4, 2,-2,-3,
                         9, 6, 2, 6,
                         0,-5, 1,-5,
                         0, 0, 0, 0 }};
    // clang-format on
    CHECK_THROWS_AS(b.inverse(), std::runtime_error);
  }
  SECTION("inverse check")
  {
    // clang-format off
    sunray::Matrix44 a{{ 4,-9, 7, 3,
                         3,-8, 2,-9,
                        -4, 4, 4, 1,
                        -6, 5,-1, 1 }};
    sunray::Matrix44 b{{ 8, 2, 2, 2,
                         3,-1, 7, 0,
                         7, 0, 5, 4,
                         6,-2, 0, 5 }};
    // clang-format on
    auto c = a * b;
    CHECK(c * b.inverse() == a);
  }
}

TEST_CASE("translation 4x4 matrix", "[matrix 4x4]")
{
  SECTION("translate point")
  {
    auto transform = sunray::Matrix44::translation(5, -3, 2);
    auto point = sunray::create_point(-3, 4, 5);
    CHECK(transform * point == sunray::create_point(2, 1, 7));
  }
  SECTION("translate point inverse")
  {
    auto transform = sunray::Matrix44::translation(5, -3, 2).inverse();
    auto point = sunray::create_point(2, 1, 7);
    CHECK(transform * point == sunray::create_point(-3, 4, 5));
  }
  SECTION("translate vector")
  {
    auto transform = sunray::Matrix44::translation(5, -3, 2);
    auto vector = sunray::create_vector(-3, 4, 5);
    CHECK(transform * vector == sunray::create_vector(-3, 4, 5));
  }
}

TEST_CASE("scaling 4x4 matrix", "[matrix 4x4]")
{
  SECTION("scale point")
  {
    auto transform = sunray::Matrix44::scaling(2, 3, 4);
    auto point = sunray::create_point(-4, 6, 8);
    CHECK(transform * point == sunray::create_point(-8, 18, 32));
  }
  SECTION("scale point inverse")
  {
    auto transform = sunray::Matrix44::scaling(2, 3, 4).inverse();
    auto point = sunray::create_point(-8, 18, 32);
    CHECK(transform * point == sunray::create_point(-4, 6, 8));
  }
  SECTION("reflect point")
  {
    auto transform = sunray::Matrix44::scaling(-1, 1, 1);
    auto point = sunray::create_point(2, 3, 4);
    CHECK(transform * point == sunray::create_point(-2, 3, 4));
  }
  SECTION("scale vector")
  {
    auto transform = sunray::Matrix44::scaling(2, 3, 4);
    auto vector = sunray::create_vector(-4, 6, 8);
    CHECK(transform * vector == sunray::create_vector(-8, 18, 32));
  }
  SECTION("scale vector inverse")
  {
    auto transform = sunray::Matrix44::scaling(2, 3, 4).inverse();
    auto vector = sunray::create_vector(-4, 6, 8);
    CHECK(transform * vector == sunray::create_vector(-2, 2, 2));
  }
}

TEST_CASE("rotation x 4x4 matrix", "[matrix 4x4]")
{
  SECTION("rotate point half quarter")
  {
    auto rotate = sunray::Matrix44::rotation_x(sunray::PI / 4);
    auto point = sunray::create_point(0, 1, 0);
    CHECK(rotate * point == sunray::create_point(0, sunray::sqrt(2) / 2.0, sunray::sqrt(2) / 2.0));
  }
  SECTION("rotate point full quarter")
  {
    auto rotate = sunray::Matrix44::rotation_x(sunray::PI / 2);
    auto point = sunray::create_point(0, 1, 0);
    CHECK(rotate * point == sunray::create_point(0, 0, 1));
  }
  SECTION("rotate point half quarter inverse")
  {
    auto rotate = sunray::Matrix44::rotation_x(sunray::PI / 4).inverse();
    auto point = sunray::create_point(0, 1, 0);
    CHECK(rotate * point == sunray::create_point(0, sunray::sqrt(2) / 2.0, -sunray::sqrt(2) / 2.0));
  }
}

TEST_CASE("rotation y 4x4 matrix", "[matrix 4x4]")
{
  SECTION("rotate point half quarter")
  {
    auto rotate = sunray::Matrix44::rotation_y(sunray::PI / 4);
    auto point = sunray::create_point(0, 0, 1);
    CHECK(rotate * point == sunray::create_point(sunray::sqrt(2) / 2.0, 0, sunray::sqrt(2) / 2.0));
  }
  SECTION("rotate point full quarter")
  {
    auto rotate = sunray::Matrix44::rotation_y(sunray::PI / 2);
    auto point = sunray::create_point(0, 0, 1);
    CHECK(rotate * point == sunray::create_point(1, 0, 0));
  }
}

TEST_CASE("rotation z 4x4 matrix", "[matrix 4x4]")
{
  SECTION("rotate point half quarter")
  {
    auto rotate = sunray::Matrix44::rotation_z(sunray::PI / 4);
    auto point = sunray::create_point(0, 1, 0);
    CHECK(rotate * point == sunray::create_point(-sunray::sqrt(2) / 2.0, sunray::sqrt(2) / 2.0, 0));
  }
  SECTION("rotate point full quarter")
  {
    auto rotate = sunray::Matrix44::rotation_z(sunray::PI / 2);
    auto point = sunray::create_point(0, 1, 0);
    CHECK(rotate * point == sunray::create_point(-1, 0, 0));
  }
}

TEST_CASE("shearing 4x4 matrix", "[matrix 4x4]")
{
  SECTION("shear point xy")
  {
    auto shear = sunray::Matrix44::shearing(1, 0, 0, 0, 0, 0);
    auto point = sunray::create_point(2, 3, 4);
    CHECK(shear * point == sunray::create_point(5, 3, 4));
  }
  SECTION("shear point xz")
  {
    auto shear = sunray::Matrix44::shearing(0, 1, 0, 0, 0, 0);
    auto point = sunray::create_point(2, 3, 4);
    CHECK(shear * point == sunray::create_point(6, 3, 4));
  }
  SECTION("shear point yx")
  {
    auto shear = sunray::Matrix44::shearing(0, 0, 1, 0, 0, 0);
    auto point = sunray::create_point(2, 3, 4);
    CHECK(shear * point == sunray::create_point(2, 5, 4));
  }
  SECTION("shear point yz")
  {
    auto shear = sunray::Matrix44::shearing(0, 0, 0, 1, 0, 0);
    auto point = sunray::create_point(2, 3, 4);
    CHECK(shear * point == sunray::create_point(2, 7, 4));
  }
  SECTION("shear point zx")
  {
    auto shear = sunray::Matrix44::shearing(0, 0, 0, 0, 1, 0);
    auto point = sunray::create_point(2, 3, 4);
    CHECK(shear * point == sunray::create_point(2, 3, 6));
  }
  SECTION("shear point zy")
  {
    auto shear = sunray::Matrix44::shearing(0, 0, 0, 0, 0, 1);
    auto point = sunray::create_point(2, 3, 4);
    CHECK(shear * point == sunray::create_point(2, 3, 7));
  }
}
