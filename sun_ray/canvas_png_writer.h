//
//  canvas_png_writer.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 25.03.21.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/canvas_writer.h>
#include <sun_ray/image.h>

#include <iostream>
#include <sstream>


namespace sunray
{
  class CanvasPNGWriter : public CanvasWriter
  {
  public:
    explicit CanvasPNGWriter()
    : CanvasWriter{ImageFormat::PNG}
    {
    }

  private:
    const std::string& do_extension() const override
    {
      static const std::string extension = ".png";
      return extension;
    }

    static void stbi_write_func(void* context, void* data, int size)
    {
      std::ostream& stream = *reinterpret_cast<std::ostream*>(context);
      stream.write(reinterpret_cast<const char*>(data), size);
    }

    void do_write(const Canvas& canvas, std::ostream& stream) const override
    {
      constexpr uint8_t channels = 3;
      std::vector<uint8_t> buffer;
      buffer.resize(canvas.height() * canvas.width() * channels);

      uint32_t n = 0;
      for (uint32_t y = 0; y < canvas.height(); ++y) {
        for (uint32_t x = 0; x < canvas.width(); ++x) {
          const auto pixel = canvas.pixel_at(x, y).normalize();
          buffer[n++] = static_cast<uint8_t>(round(pixel.red() * 255.0f));
          buffer[n++] = static_cast<uint8_t>(round(pixel.green() * 255.0f));
          buffer[n++] = static_cast<uint8_t>(round(pixel.blue() * 255.0f));
        }
      }

      auto ret =
        stbi_write_png_to_func(stbi_write_func, &stream, static_cast<int>(canvas.width()), static_cast<int>(canvas.height()),
                               channels, buffer.data(), static_cast<int>(canvas.width() * channels));

      if (ret != 1) {
        throw std::runtime_error{"Could not store image as PNG"};
      }
    }
  };
}
