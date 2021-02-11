//
//  function_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 07.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/format_helper.h>
#include <sun_ray/script/function_traits.h>

#include <catch2/catch.hpp>


static std::string doit(int n, double f)
{
  return fmt::format("done it with {} and {}", n, f);
}

static void nothing_to_do()
{
}

TEST_CASE("function decomposition", "[function traits]")
{
  SECTION("simple function with two params")
  {
    doit(42, 47.11);
    typedef decltype(doit) function_type;
    using R = typename sunray::script::function_traits<function_type>::return_type;
    using T1 = typename sunray::script::function_traits<function_type>::template argument<0>::type;
    using T2 = typename sunray::script::function_traits<function_type>::template argument<1>::type;

    CHECK(std::is_same<R, std::string>());
    CHECK(sunray::script::function_traits<function_type>::arity == 2);
    CHECK(std::is_same<T1, int>());
    CHECK(std::is_same<T2, double>());
  }
  SECTION("simple void function")
  {
    nothing_to_do();
    typedef decltype(nothing_to_do) function_type;
    using R = typename sunray::script::function_traits<function_type>::return_type;

    CHECK(std::is_void<R>());
    CHECK(sunray::script::function_traits<function_type>::arity == 0);
  }
}
