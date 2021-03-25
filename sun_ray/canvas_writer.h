//
//  canvas_writer.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 25.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/canvas.h>

#include <ostream>


namespace sunray
{
  enum class ImageFormat { PPM3, PPM6, PNG };

  class CanvasWriter
  {
  public:
    virtual ~CanvasWriter()
    {
    }

    CanvasWriter(const CanvasWriter&) = delete;
    CanvasWriter(CanvasWriter&&) = delete;
    CanvasWriter& operator=(const CanvasWriter&) = delete;
    CanvasWriter& operator=(CanvasWriter&&) = delete;

    void write(const Canvas& canvas, std::ostream& stream) const
    {
      do_write(canvas, stream);
    }

    ImageFormat format() const
    {
      return format_;
    }

    const std::string& extension() const
    {
      return do_extension();
    }

  protected:
    explicit CanvasWriter(ImageFormat format)
    : format_{format}
    {
    }

  private:
    virtual void do_write(const Canvas& canvas, std::ostream& stream) const = 0;
    virtual const std::string& do_extension() const = 0;

    ImageFormat format_;
  };
}
