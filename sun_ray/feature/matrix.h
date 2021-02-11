//
//  matrix.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 26.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/tuple.h>


namespace sunray
{
  class Index
  {
  public:
    Index(uint8_t r, uint8_t c)
    : r_(r)
    , c_(c)
    {
    }

    ~Index() = default;

    Index(const Index&) = delete;
    Index(Index&&) = delete;
    Index& operator=(const Index&) = delete;
    Index& operator=(Index&&) = delete;

    inline uint8_t row() const
    {
      return r_;
    }

    inline uint8_t column() const
    {
      return c_;
    }

  private:
    uint8_t r_;
    uint8_t c_;
  };


  class Matrix22
  {
  public:
    static constexpr uint8_t elements = 4;
    using Mat = std::array<double, elements>;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit Matrix22(const Mat& m)
    {
      std::copy(m.begin(), m.end(), m_.begin());
    }

    explicit Matrix22(Mat&& m) noexcept
    : m_{m}
    {
    }

    Matrix22(const Matrix22& m) = delete;
    Matrix22(Matrix22&& m) = delete;

    ~Matrix22() = default;

    Matrix22& operator=(const Matrix22&) = delete;
    Matrix22& operator=(Matrix22&&) = delete;

    bool operator==(const Matrix22& rhs) const
    {
      return Approx(m_[0]) == rhs.m_[0] && Approx(m_[1]) == rhs.m_[1] && Approx(m_[2]) == rhs.m_[2] && Approx(m_[3]) == rhs.m_[3];
    }

    bool operator!=(const Matrix22& rhs) const
    {
      return Approx(m_[0]) != rhs.m_[0] || Approx(m_[1]) != rhs.m_[1] || Approx(m_[2]) != rhs.m_[2] || Approx(m_[3]) != rhs.m_[3];
    }

    double operator[](uint8_t n) const
    {
      return m_.at(n);
    }

    double determinant() const
    {
      return m_[0] * m_[3] - m_[1] * m_[2];
    }

    friend std::ostream& operator<<(std::ostream& stream, const Matrix22& matrix)
    {
      stream << "| " << matrix[0] << " | " << matrix[1] << " |\n";
      stream << "| " << matrix[2] << " | " << matrix[3] << " |\n";
      return stream;
    }

  private:
    Mat m_;
  };


  class Matrix33
  {
  public:
    static constexpr uint8_t elements = 9;
    using Mat = std::array<double, elements>;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit Matrix33(const Mat& m)
    {
      std::copy(m.begin(), m.end(), m_.begin());
    }

    explicit Matrix33(Mat&& m) noexcept
    : m_{m}
    {
    }

    Matrix33(const Matrix33& m) = delete;
    Matrix33(Matrix33&& m) = delete;

    ~Matrix33() = default;

    Matrix33& operator=(const Matrix33&) = delete;
    Matrix33& operator=(Matrix33&&) = delete;

    bool operator==(const Matrix33& rhs) const
    {
      return Approx(m_[0]) == rhs.m_[0] && Approx(m_[1]) == rhs.m_[1] && Approx(m_[2]) == rhs.m_[2] &&
             Approx(m_[3]) == rhs.m_[3] && Approx(m_[4]) == rhs.m_[4] && Approx(m_[5]) == rhs.m_[5] &&
             Approx(m_[6]) == rhs.m_[6] && Approx(m_[7]) == rhs.m_[7] && Approx(m_[8]) == rhs.m_[8];
    }

    bool operator!=(const Matrix33& rhs) const
    {
      return Approx(m_[0]) != rhs.m_[0] || Approx(m_[1]) != rhs.m_[1] || Approx(m_[2]) != rhs.m_[2] ||
             Approx(m_[3]) != rhs.m_[3] || Approx(m_[4]) != rhs.m_[4] || Approx(m_[5]) != rhs.m_[5] ||
             Approx(m_[6]) != rhs.m_[6] || Approx(m_[7]) != rhs.m_[7] || Approx(m_[8]) != rhs.m_[8];
    }

    double operator[](uint8_t n) const
    {
      return m_.at(n);
    }

    Matrix22 submatrix(uint8_t row, uint8_t column) const
    {
      if (row > 2 || column > 2) {
        throw std::out_of_range{"row/column out of range"};
      }

      Matrix22::Mat m;
      uint8_t count{0};
      for (uint8_t r = 0; r < 3; ++r) {
        if (r == row) {
          continue;
        }
        for (uint8_t c = 0; c < 3; ++c) {
          if (c == column) {
            continue;
          }
          m[count++] = m_[r * 3 + c];
        }
      }

      return Matrix22{m};
    }

    double minor(uint8_t row, uint8_t column) const
    {
      return submatrix(row, column).determinant();
    }

    double cofactor(uint8_t row, uint8_t column) const
    {
      static Mat factors = {1, -1, 1, -1, 1, -1, 1, -1, 1};
      return minor(row, column) * factors[row * 3 + column];
    }

    double determinant() const
    {
      return m_[0] * cofactor(0, 0) + m_[1] * cofactor(0, 1) + m_[2] * cofactor(0, 2);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Matrix33& matrix)
    {
      stream << "| " << matrix[0] << " | " << matrix[1] << " | " << matrix[2] << " |\n";
      stream << "| " << matrix[3] << " | " << matrix[4] << " | " << matrix[5] << " |\n";
      stream << "| " << matrix[6] << " | " << matrix[7] << " | " << matrix[8] << " |\n";
      return stream;
    }

  private:
    Mat m_;
  };


  class Matrix44
  {
  public:
    static constexpr uint8_t elements = 16;
    using Mat = std::array<double, elements>;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit Matrix44(const Mat& m)
    {
      std::copy(m.begin(), m.end(), m_.begin());
    }

    explicit Matrix44(Mat&& m) noexcept
    : m_{m}
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    Matrix44(const Matrix44& m)
    {
      std::copy(m.m_.begin(), m.m_.end(), m_.begin());
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    Matrix44(Matrix44&& m) noexcept
    : m_{m.m_}
    {
    }

    ~Matrix44() = default;

    Matrix44& operator=(const Matrix44&) = delete;
    Matrix44& operator=(Matrix44&&) = delete;

    bool operator==(const Matrix44& rhs) const
    {
      return Approx(m_[0]) == rhs.m_[0] && Approx(m_[1]) == rhs.m_[1] && Approx(m_[2]) == rhs.m_[2] &&
             Approx(m_[3]) == rhs.m_[3] && Approx(m_[4]) == rhs.m_[4] && Approx(m_[5]) == rhs.m_[5] &&
             Approx(m_[6]) == rhs.m_[6] && Approx(m_[7]) == rhs.m_[7] && Approx(m_[8]) == rhs.m_[8] &&
             Approx(m_[9]) == rhs.m_[9] && Approx(m_[10]) == rhs.m_[10] && Approx(m_[11]) == rhs.m_[11] &&
             Approx(m_[12]) == rhs.m_[12] && Approx(m_[13]) == rhs.m_[13] && Approx(m_[14]) == rhs.m_[14] &&
             Approx(m_[15]) == rhs.m_[15];
    }

    bool operator!=(const Matrix44& rhs) const
    {
      return Approx(m_[0]) != rhs.m_[0] || Approx(m_[1]) != rhs.m_[1] || Approx(m_[2]) != rhs.m_[2] ||
             Approx(m_[3]) != rhs.m_[3] || Approx(m_[4]) != rhs.m_[4] || Approx(m_[5]) != rhs.m_[5] ||
             Approx(m_[6]) != rhs.m_[6] || Approx(m_[7]) != rhs.m_[7] || Approx(m_[8]) != rhs.m_[8] ||
             Approx(m_[9]) != rhs.m_[9] || Approx(m_[10]) != rhs.m_[10] || Approx(m_[11]) != rhs.m_[11] ||
             Approx(m_[12]) != rhs.m_[12] || Approx(m_[13]) != rhs.m_[13] || Approx(m_[14]) != rhs.m_[14] ||
             Approx(m_[15]) != rhs.m_[15];
    }

    double operator[](uint8_t n) const
    {
      return m_.at(n);
    }

    double operator[](Index i) const
    {
      return m_[(i.row() * 4) + i.column()];
    }

    friend Matrix44 operator*(const Matrix44& lhs, const Matrix44& rhs)
    {
      Mat m;
      m[0] = lhs[0] * rhs[0] + lhs[1] * rhs[4] + lhs[2] * rhs[8] + lhs[3] * rhs[12];
      m[1] = lhs[0] * rhs[1] + lhs[1] * rhs[5] + lhs[2] * rhs[9] + lhs[3] * rhs[13];
      m[2] = lhs[0] * rhs[2] + lhs[1] * rhs[6] + lhs[2] * rhs[10] + lhs[3] * rhs[14];
      m[3] = lhs[0] * rhs[3] + lhs[1] * rhs[7] + lhs[2] * rhs[11] + lhs[3] * rhs[15];

      m[4] = lhs[4] * rhs[0] + lhs[5] * rhs[4] + lhs[6] * rhs[8] + lhs[7] * rhs[12];
      m[5] = lhs[4] * rhs[1] + lhs[5] * rhs[5] + lhs[6] * rhs[9] + lhs[7] * rhs[13];
      m[6] = lhs[4] * rhs[2] + lhs[5] * rhs[6] + lhs[6] * rhs[10] + lhs[7] * rhs[14];
      m[7] = lhs[4] * rhs[3] + lhs[5] * rhs[7] + lhs[6] * rhs[11] + lhs[7] * rhs[15];

      m[8] = lhs[8] * rhs[0] + lhs[9] * rhs[4] + lhs[10] * rhs[8] + lhs[11] * rhs[12];
      m[9] = lhs[8] * rhs[1] + lhs[9] * rhs[5] + lhs[10] * rhs[9] + lhs[11] * rhs[13];
      m[10] = lhs[8] * rhs[2] + lhs[9] * rhs[6] + lhs[10] * rhs[10] + lhs[11] * rhs[14];
      m[11] = lhs[8] * rhs[3] + lhs[9] * rhs[7] + lhs[10] * rhs[11] + lhs[11] * rhs[15];

      m[12] = lhs[12] * rhs[0] + lhs[13] * rhs[4] + lhs[14] * rhs[8] + lhs[15] * rhs[12];
      m[13] = lhs[12] * rhs[1] + lhs[13] * rhs[5] + lhs[14] * rhs[9] + lhs[15] * rhs[13];
      m[14] = lhs[12] * rhs[2] + lhs[13] * rhs[6] + lhs[14] * rhs[10] + lhs[15] * rhs[14];
      m[15] = lhs[12] * rhs[3] + lhs[13] * rhs[7] + lhs[14] * rhs[11] + lhs[15] * rhs[15];
      return Matrix44{m};
    }

    friend Tuple operator*(const Matrix44& lhs, const Tuple& rhs)
    {
      Tuple::Vec v;
      v[0] = lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + lhs[3] * rhs[3];
      v[1] = lhs[4] * rhs[0] + lhs[5] * rhs[1] + lhs[6] * rhs[2] + lhs[7] * rhs[3];
      v[2] = lhs[8] * rhs[0] + lhs[9] * rhs[1] + lhs[10] * rhs[2] + lhs[11] * rhs[3];
      v[3] = lhs[12] * rhs[0] + lhs[13] * rhs[1] + lhs[14] * rhs[2] + lhs[15] * rhs[3];
      return Tuple{v};
    }

    Matrix44 transpose() const
    {
      // clang-format off
      return Matrix44{{ m_[0], m_[4], m_[8], m_[12],
                        m_[1], m_[5], m_[9], m_[13],
                        m_[2], m_[6], m_[10], m_[14],
                        m_[3], m_[7], m_[11], m_[15] }};
      // clang-format on
    }

    Matrix33 submatrix(uint8_t row, uint8_t column) const
    {
      if (row > 3 || column > 3) {
        throw std::out_of_range{"row/column out of range"};
      }

      Matrix33::Mat m;
      uint8_t count{0};
      for (uint8_t r = 0; r < 4; ++r) {
        if (r == row) {
          continue;
        }
        for (uint8_t c = 0; c < 4; ++c) {
          if (c == column) {
            continue;
          }
          m[count++] = m_[r * 4 + c];
        }
      }

      return Matrix33{m};
    }

    double minor(uint8_t row, uint8_t column) const
    {
      return submatrix(row, column).determinant();
    }

    double cofactor(uint8_t row, uint8_t column) const
    {
      // clang-format off
      static Mat factors = {1,-1, 1,-1,
                           -1, 1,-1, 1,
                            1,-1, 1,-1,
                           -1, 1,-1, 1};
      // clang-format on
      return minor(row, column) * factors[row * 4 + column];
    }

    double determinant() const
    {
      return m_[0] * cofactor(0, 0) + m_[1] * cofactor(0, 1) + m_[2] * cofactor(0, 2) + m_[3] * cofactor(0, 3);
    }

    bool is_invertible() const
    {
      return determinant() != Approx(0.0);
    }

    Matrix44 inverse() const
    {
      const auto det = determinant();
      if (det == Approx(0.0)) {
        throw std::runtime_error{"matrix is not inversible"};
      }
      Matrix44::Mat mat;

      for (uint8_t r = 0; r < 4; ++r) {
        for (uint8_t c = 0; c < 4; ++c) {
          auto co = cofactor(r, c);
          mat[c * 4 + r] = co / det;
        }
      }

      return Matrix44{mat};
    }

    static Matrix44 translation(double x, double y, double z)
    {
      // clang-format off
      return Matrix44{{ 1, 0, 0, x,
                        0, 1, 0, y,
                        0, 0, 1, z,
                        0, 0, 0, 1 }};
      // clang-format on
    }

    static Matrix44 scaling(double x, double y, double z)
    {
      // clang-format off
      return Matrix44{{ x, 0, 0, 0,
                        0, y, 0, 0,
                        0, 0, z, 0,
                        0, 0, 0, 1 }};
      // clang-format on
    }

    static Matrix44 rotation_x(double radians)
    {
      // clang-format off
      return Matrix44{{ 1,            0,            0, 0,
                        0, cos(radians),-sin(radians), 0,
                        0, sin(radians), cos(radians), 0,
                        0,            0,            0, 1 }};
      // clang-format on
    }

    static Matrix44 rotation_y(double radians)
    {
      // clang-format off
      return Matrix44{{ cos(radians), 0, sin(radians), 0,
                                   0, 1,            0, 0,
                       -sin(radians), 0, cos(radians), 0,
                                   0, 0,            0, 1 }};
      // clang-format on
    }

    static Matrix44 rotation_z(double radians)
    {
      // clang-format off
      return Matrix44{{ cos(radians),-sin(radians), 0, 0,
                        sin(radians), cos(radians), 0, 0,
                                   0,            0, 1, 0,
                                   0,            0, 0, 1 }};
      // clang-format on
    }

    static Matrix44 shearing(double xy, double xz, double yx, double yz, double zx, double zy)
    {
      // clang-format off
      return Matrix44{{  1, xy, xz, 0,
                        yx,  1, yz, 0,
                        zx, zy,  1, 0,
                         0,  0,  0, 1 }};
      // clang-format on
    }

    static const Matrix44& identity()
    {
      // clang-format off
      static Matrix44 identity{{ 1, 0, 0, 0,
                                 0, 1, 0, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 1 }};
      // clang-format on
      return identity;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Matrix44& matrix)
    {
      stream << "| " << matrix[0] << " | " << matrix[1] << " | " << matrix[2] << " | " << matrix[3] << " |\n";
      stream << "| " << matrix[4] << " | " << matrix[5] << " | " << matrix[6] << " | " << matrix[7] << " |\n";
      stream << "| " << matrix[8] << " | " << matrix[9] << " | " << matrix[10] << " | " << matrix[11] << " |\n";
      stream << "| " << matrix[12] << " | " << matrix[13] << " | " << matrix[14] << " | " << matrix[15] << " |\n";
      return stream;
    }

  private:
    Mat m_;
  };
}
