//
//  options.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 01.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/format_helper.h>

#include <filesystem>
#include <ostream>
#include <vector>


namespace sunray
{
  struct Options
  {
    static void print_usage(std::ostream& stream)
    {
      auto help = R"(Usage: sun_ray [ --help ] | [ [-df] <FILE> [<FILE>]... ]
help:
  --help                              display this help and exit

processing options:
  -d, --dump                          dump instructions
  -f, --format                        format the program
  <FILE>                              script to execute
)";
      stream << help << std::endl;
    }

    static std::pair<bool, Options> handle_options(std::ostream& stream, const std::vector<std::string>& args)
    {
      Options opts;

      if (args.size() < 1) {
        print_usage(stream);
        return std::make_pair(false, Options{});
      }

      bool error{false};
      bool start_untagged_options{false};
      for (size_t n = 1; n < args.size(); ++n) {
        const auto& option = args[n];
        if (option == "-f" || option == "--format") {
          if (start_untagged_options) {
            error = true;
          }
          opts.format_ = true;
        } else if (option == "-d" || option == "--dump") {
          if (start_untagged_options) {
            error = true;
          }
          opts.dump_ = true;
        } else if (option == "-h" || option == "--help") {
          error = true;
        } else if (option[0] == '-') {
          stream << fmt::format("unkown option '{}'", option) << std::endl;
          error = true;
        } else {
          start_untagged_options = true;
          opts.files_.emplace_back(option);
        }
      }

      if (error) {
        print_usage(stream);
        return std::make_pair(false, Options{});
      }

      return std::make_pair(true, opts);
    }

    bool dump_{false};
    bool format_{false};
    std::vector<std::filesystem::path> files_;
  };
}
