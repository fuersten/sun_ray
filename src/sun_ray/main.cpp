//
//  main.cpp
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 21.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/init.h>

#include <iostream>

#include "application.h"


int main(int argc, const char* argv[])
{
  std::vector<std::string> args;
  for (int n = 0; n < argc; ++n) {
    args.emplace_back(argv[n]);
  }

  return sunray::Application{std::cout, std::cerr, args}.run();
}
