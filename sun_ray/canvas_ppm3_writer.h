//
//  canvas_ppm3_writer.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 05.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/canvas_writer.h>

#include <sstream>


namespace sunray
{
  class CanvasPPM3Writer : public CanvasWriter
  {
  public:
    explicit CanvasPPM3Writer()
    : CanvasWriter{ImageFormat::PPM3}
    {
    }

  private:
    const std::string& do_extension() const override
    {
      static const std::string extension = ".ppm";
      return extension;
    }

    void do_write(const Canvas& canvas, std::ostream& stream) const override
    {
      write_header(canvas, stream);
      write_body(canvas, stream);
    }

    static void write_header(const Canvas& canvas, std::ostream& stream)
    {
      stream << "P3\n";
      stream << canvas.width() << " " << canvas.height() << "\n";
      stream << "255\n";
    }

    static void write_body(const Canvas& canvas, std::ostream& stream)
    {
      for (uint32_t y = 0; y < canvas.height(); ++y) {
        std::stringstream ss;
        auto position = ss.tellp();
        for (uint32_t x = 0; x < canvas.width(); ++x) {
          const auto pixel = canvas.pixel_at(x, y).normalize();
          ss << round(pixel.red() * 255.0f) << " " << round(pixel.green() * 255.0f) << " " << round(pixel.blue() * 255.0f) << " ";
          if ((ss.tellp() - position) >= 60 && x < (canvas.width() - 1)) {
            ss << "\n";
            stream << ss.str();
            ss.clear();
            ss.str("");
            position = ss.tellp();
          }
        }
        ss << "\n";
        stream << ss.str();
      }
    }
  };
}
