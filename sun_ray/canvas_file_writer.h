//
//  canvas_file_writer.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 20.01.20.
//  Copyright © 20290 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/canvas_ppm3_writer.h>
#include <sun_ray/canvas_ppm6_writer.h>
#include <sun_ray/script/format_helper.h>

#include <filesystem>
#include <fstream>


namespace sunray
{
  class CanvasFileWriter
  {
  public:
    CanvasFileWriter(ImageFormat format, std::filesystem::path path)
    : format_{format}
    , path_{std::move(path)}
    {
    }

    ~CanvasFileWriter() = default;

    CanvasFileWriter(const CanvasFileWriter&) = delete;
    CanvasFileWriter(CanvasFileWriter&&) = delete;
    CanvasFileWriter& operator=(const CanvasFileWriter&) = delete;
    CanvasFileWriter& operator=(CanvasFileWriter&&) = delete;

    void write(const Canvas& canvas) const
    {
      std::ofstream of(path_);
      if (!of.good()) {
        throw std::runtime_error{fmt::format("Cannot write canvas to file ''", path_.string())};
      }
      std::unique_ptr<sunray::CanvasWriter> writer;
      switch (format_) {
        case ImageFormat::PPM3:
          writer.reset(new sunray::CanvasPPM3Writer);
          break;
        case ImageFormat::PPM6:
          writer.reset(new sunray::CanvasPPM6Writer);
      }
      writer->write(canvas, of);
      of.close();
    }

  private:
    ImageFormat format_;
    std::filesystem::path path_;
  };
}
