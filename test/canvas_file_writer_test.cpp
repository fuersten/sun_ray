//
//  canvas_file_writer_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 20.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/canvas_file_writer.h>

#include <catch2/catch.hpp>

#include "temporary_directory.h"


TEST_CASE("write file canvas", "[canvas file writer]")
{
  SECTION("write ppm3 file")
  {
    TemporaryDirectoryGuard guard;
    auto file = guard.temporary_directory_path() / "test.ppm";

    sunray::Canvas canvas{100, 100};
    sunray::CanvasFileWriter writer{sunray::ImageFormat::PPM3, file};
    writer.write(canvas);

    CHECK(std::filesystem::exists(file));
  }
  SECTION("write ppm6 file")
  {
    TemporaryDirectoryGuard guard;
    auto file = guard.temporary_directory_path() / "test.ppm";

    sunray::Canvas canvas{100, 100};
    sunray::CanvasFileWriter writer{sunray::ImageFormat::PPM6, file};
    writer.write(canvas);

    CHECK(std::filesystem::exists(file));
  }
  SECTION("write ppm3 file fail")
  {
    std::filesystem::path file{"/this/file/does/not/exist/test.ppm"};
    sunray::Canvas canvas{100, 100};
    sunray::CanvasFileWriter writer{sunray::ImageFormat::PPM3, file};
    CHECK_THROWS_AS(writer.write(canvas), std::exception);
  }
}
