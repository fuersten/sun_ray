//
//  main.cpp
//  clock
//
//  Created by Lars-Christian Fürstenberg on 04.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/canvas_file_writer.h>
#include <sun_ray/feature/transformation.h>
#include <sun_ray/init.h>

#include <iostream>


int main(int argc, const char* argv[])
{
  try {
    if (argc < 2) {
      std::cerr << "Usage: clock <pathname>" << std::endl;
      return -1;
    }

    sunray::Canvas canvas{250, 250};
    const auto orange = sunray::Color{1.0f, 0.647f, 0};
    const auto point = sunray::create_point(0, 1, 0);
    double deg = 0.0;

    for (uint8_t n = 1; n <= 12; ++n) {
      const auto trans =
        sunray::Transformation().scale(1, -120, 1).rotate_z(sunray::deg_to_rad(deg)).translate(125, 125, 0).matrix();
      const auto pixel = trans * point;
      canvas.pixel_at(static_cast<uint32_t>(pixel.x()), static_cast<uint32_t>(pixel.y()), orange);
      deg += 30.0;
    }

    sunray::CanvasFileWriter writer{sunray::ImageFormat::PNG, std::filesystem::path{argv[1]} / "clock.png"};
    writer.write(canvas);
  } catch (const std::exception& ex) {
    std::cerr << "exception caught: " << ex.what() << std::endl;
  }

  return 0;
}
