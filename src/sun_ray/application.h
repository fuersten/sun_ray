//
//  application.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 29.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/engine.h>
#include <sun_ray/version.h>

#include <fstream>
#include <iostream>

#include "options.h"


namespace sunray
{
  class Application
  {
  public:
    Application(std::ostream& stream, std::ostream& error_stream, const std::vector<std::string>& args)
    : stream_{stream}
    , error_stream_{error_stream}
    {
      print_copy_right();
      opts_ = Options::handle_options(error_stream, args);
    }

    int run()
    {
      if (!opts_.first) {
        return -1;
      }

      int ret{0};
      try {
        sunray::script::Engine executor{stream_, error_stream_, opts_.second.format_, opts_.second.dump_};

        for (const auto& file : opts_.second.files_) {
          std::ifstream input_source{file.string()};
          if (input_source.good()) {
            error_stream_ << "================================================================================\n";
            error_stream_ << file.string() << ":" << std::endl;
            error_stream_ << "================================================================================\n\n";

            if (!executor.process(input_source)) {
              error_stream_ << "\nfailure!\n\n";
              ret = -1;
            }
          } else {
            error_stream_ << "Could not open sunray source file " << file.string() << std::endl;
            ret = -1;
          }
        }
      } catch (const std::exception& ex) {
        error_stream_ << "Caught exception: " << ex.what() << std::endl;
        ret = -1;
      }

      return ret;
    }

    ~Application() = default;

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

  private:
    void print_copy_right()
    {
      error_stream_ << fmt::format("SunRay ray tracer {}\n{}\n\n", SUNRAY_VERSION_STRING, SUNRAY_COPYRIGHT);
    }

    std::pair<bool, Options> opts_;
    std::ostream& stream_;
    std::ostream& error_stream_;
  };
}
