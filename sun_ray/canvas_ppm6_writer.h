//
//  canvas_ppm6_writer.h
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
  class CanvasPPM6Writer : public CanvasWriter
  {
  public:
    explicit CanvasPPM6Writer()
    : CanvasWriter{ImageFormat::PPM6}
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
      stream << "P6\n";
      stream << canvas.width() << " " << canvas.height() << "\n";
      stream << "255\n";
    }

    static void write_body(const Canvas& canvas, std::ostream& stream)
    {
      std::vector<uint8_t> buffer;
      buffer.resize(canvas.height() * canvas.width() * 3);

      uint32_t n = 0;
      for (uint32_t y = 0; y < canvas.height(); ++y) {
        for (uint32_t x = 0; x < canvas.width(); ++x) {
          const auto pixel = canvas.pixel_at(x, y).normalize();
          buffer[n++] = static_cast<uint8_t>(round(pixel.red() * 255.0f));
          buffer[n++] = static_cast<uint8_t>(round(pixel.green() * 255.0f));
          buffer[n++] = static_cast<uint8_t>(round(pixel.blue() * 255.0f));
        }
      }
      stream.write(reinterpret_cast<const char*>(buffer.data()), static_cast<long>(buffer.size()));
    }
  };
}
