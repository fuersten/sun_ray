//
//  canvas_writer_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 25.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/canvas_png_writer.h>
#include <sun_ray/canvas_ppm3_writer.h>
#include <sun_ray/canvas_ppm6_writer.h>

#include <fstream>
#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("write ppm3 canvas", "[canvas writer]")
{
  SECTION("extension")
  {
    sunray::CanvasPPM3Writer cw;
    CHECK(cw.extension() == ".ppm");
  }
  SECTION("check header")
  {
    sunray::Canvas canvas{10, 20};

    std::stringstream ss;
    sunray::CanvasPPM3Writer cw;
    cw.write(canvas, ss);

    std::istringstream iss(ss.str());
    std::string line;
    uint32_t count{0};
    while (std::getline(iss, line)) {
      switch (count) {
        case 0:
          CHECK(line == "P3");
          break;
        case 1:
          CHECK(line == "10 20");
          break;
        case 2:
          CHECK(line == "255");
          break;
      }
      ++count;
    }
  }
  SECTION("check body")
  {
    sunray::Canvas canvas{10, 5};

    auto red = sunray::Color{1.0f, 0, 0};
    auto green = sunray::Color{0, 1.0f, 0};
    auto blue = sunray::Color{0, 0, 1.0f};
    auto orange = sunray::Color{1.0f, 0.647f, 0};

    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 0, red);
    }
    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 1, green);
    }
    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 3, blue);
    }
    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 4, orange);
    }

    std::stringstream ss;
    sunray::CanvasPPM3Writer cw;
    cw.write(canvas, ss);

    std::istringstream iss(ss.str());
    std::string line;
    uint32_t count{0};
    while (std::getline(iss, line)) {
      switch (count) {
        case 0:
          CHECK(line == "P3");
          break;
        case 1:
          CHECK(line == "10 5");
          break;
        case 2:
          CHECK(line == "255");
          break;
        case 3:
          CHECK(line == "255 0 0 255 0 0 255 0 0 255 0 0 255 0 0 255 0 0 255 0 0 255 0 0 ");
          break;
        case 4:
          CHECK(line == "255 0 0 255 0 0 ");
          break;
        case 5:
          CHECK(line == "0 255 0 0 255 0 0 255 0 0 255 0 0 255 0 0 255 0 0 255 0 0 255 0 ");
          break;
        case 6:
          CHECK(line == "0 255 0 0 255 0 ");
          break;
        case 7:
          CHECK(line == "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
          break;
        case 8:
          CHECK(line == "0 0 255 0 0 255 0 0 255 0 0 255 0 0 255 0 0 255 0 0 255 0 0 255 ");
          break;
        case 9:
          CHECK(line == "0 0 255 0 0 255 ");
          break;
        case 10:
          CHECK(line == "255 165 0 255 165 0 255 165 0 255 165 0 255 165 0 255 165 0 ");
          break;
        case 11:
          CHECK(line == "255 165 0 255 165 0 255 165 0 255 165 0 ");
          break;
      }
      ++count;
    }
    CHECK(count == 12);
  }
  SECTION("check body with negativ colors")
  {
    sunray::Canvas canvas{5, 3};

    auto c1 = sunray::Color{1.5f, 0, 0};
    auto c2 = sunray::Color{0, 0.5f, 0};
    auto c3 = sunray::Color{-0.5f, 0, 1};

    canvas.pixel_at(0, 0, c1);
    canvas.pixel_at(2, 1, c2);
    canvas.pixel_at(4, 2, c3);

    std::stringstream ss;
    sunray::CanvasPPM3Writer cw;
    cw.write(canvas, ss);

    std::istringstream iss(ss.str());
    std::string line;
    uint32_t count{0};
    while (std::getline(iss, line)) {
      switch (count) {
        case 0:
          CHECK(line == "P3");
          break;
        case 1:
          CHECK(line == "5 3");
          break;
        case 2:
          CHECK(line == "255");
          break;
        case 3:
          CHECK(line == "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ");
          break;
        case 4:
          CHECK(line == "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0 ");
          break;
        case 5:
          CHECK(line == "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255 ");
          break;
      }
      ++count;
    }
    CHECK(count == 6);
  }
  SECTION("check body with long lines")
  {
    sunray::Canvas canvas{10, 2};

    auto c = sunray::Color{1, 0.8f, 0.6f};

    for (uint32_t y = 0; y < 2; ++y) {
      for (uint32_t x = 0; x < 10; ++x) {
        canvas.pixel_at(x, y, c);
      }
    }

    std::stringstream ss;
    sunray::CanvasPPM3Writer cw;
    cw.write(canvas, ss);

    std::istringstream iss(ss.str());
    std::string line;
    uint32_t count{0};
    while (std::getline(iss, line)) {
      switch (count) {
        case 0:
          CHECK(line == "P3");
          break;
        case 1:
          CHECK(line == "10 2");
          break;
        case 2:
          CHECK(line == "255");
          break;
        case 3:
          CHECK(line == "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 ");
          break;
        case 4:
          CHECK(line == "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 ");
          break;
        case 5:
          CHECK(line == "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 ");
          break;
        case 6:
          CHECK(line == "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 ");
          break;
      }
      ++count;
    }
    CHECK(count == 7);
  }
  SECTION("check newline at end")
  {
    sunray::Canvas canvas{10, 5};
    std::stringstream ss;
    sunray::CanvasPPM3Writer cw;
    cw.write(canvas, ss);
    CHECK(*(ss.str().rbegin()) == '\n');
    CHECK(*(ss.str().rbegin()++) == '\n');
  }
}

TEST_CASE("write ppm6 canvas", "[canvas writer]")
{
  SECTION("extension")
  {
    sunray::CanvasPPM6Writer cw;
    CHECK(cw.extension() == ".ppm");
  }
  SECTION("check header")
  {
    sunray::Canvas canvas{10, 20};

    std::stringstream ss;
    sunray::CanvasPPM6Writer cw;
    cw.write(canvas, ss);

    std::istringstream iss(ss.str());
    std::string line;
    uint32_t count{0};
    while (std::getline(iss, line)) {
      switch (count) {
        case 0:
          CHECK(line == "P6");
          break;
        case 1:
          CHECK(line == "10 20");
          break;
        case 2:
          CHECK(line == "255");
          break;
      }
      ++count;
    }
  }
  SECTION("check body")
  {
    sunray::Canvas canvas{10, 5};

    auto red = sunray::Color{1.0f, 0, 0};
    auto green = sunray::Color{0, 1.0f, 0};
    auto blue = sunray::Color{0, 0, 1.0f};
    auto orange = sunray::Color{1.0f, 0.647f, 0};

    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 0, red);
    }
    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 1, green);
    }
    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 3, blue);
    }
    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 4, orange);
    }

    std::stringstream ss;
    sunray::CanvasPPM6Writer cw;
    cw.write(canvas, ss);

    std::istringstream iss(ss.str());
    std::string line;
    std::getline(iss, line);
    CHECK(line == "P6");
    std::getline(iss, line);
    CHECK(line == "10 5");
    std::getline(iss, line);
    CHECK(line == "255");
    std::vector<uint8_t> buffer;
    buffer.resize(10 * 5 * 3);
    iss.read(reinterpret_cast<char*>(buffer.data()), 10 * 5 * 3);
    CHECK(buffer.size() == 150);
    CHECK(buffer[0] == 255);
    CHECK(buffer[1] == 0);
    CHECK(buffer[2] == 0);
    CHECK(buffer[30] == 0);
    CHECK(buffer[31] == 255);
    CHECK(buffer[32] == 0);
    CHECK(buffer[147] == 255);
    CHECK(buffer[148] == 165);
    CHECK(buffer[149] == 0);
  }
}

TEST_CASE("write png canvas", "[canvas writer]")
{
  SECTION("extension")
  {
    sunray::CanvasPNGWriter cw;
    CHECK(cw.extension() == ".png");
  }
  SECTION("write png")
  {
    sunray::Canvas canvas{10, 5};

    auto red = sunray::Color{1.0f, 0, 0};
    auto green = sunray::Color{0, 1.0f, 0};
    auto blue = sunray::Color{0, 0, 1.0f};
    auto orange = sunray::Color{1.0f, 0.647f, 0};

    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 0, red);
    }
    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 1, green);
    }
    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 3, blue);
    }
    for (uint32_t x = 0; x < 10; ++x) {
      canvas.pixel_at(x, 4, orange);
    }

    std::stringstream ss;
    sunray::CanvasPNGWriter cw;
    cw.write(canvas, ss);
    auto s = ss.str();
    REQUIRE(s.size() == 85);
    CHECK(static_cast<uint8_t>(s[0]) == 0x89);
    CHECK(s[1] == 'P');
    CHECK(s[2] == 'N');
    CHECK(s[3] == 'G');
    CHECK(s[4] == '\r');
    CHECK(s[5] == '\n');
    CHECK(s[6] == 0x1A);
    CHECK(s[7] == '\n');
  }
}
