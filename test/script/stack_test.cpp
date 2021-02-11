//
//  stack_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 03.04.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/stack.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("basic stack operations", "[stack]")
{
  sunray::script::Stack<int, 10> stack;

  SECTION("empty")
  {
    CHECK(stack.empty());
    CHECK(0 == stack.count());
    stack.push(42);
    CHECK_FALSE(stack.empty());
  }
  SECTION("push")
  {
    stack.push(42);
    stack.push(815);
    CHECK(2 == stack.count());
    CHECK_FALSE(stack.empty());
  }
  SECTION("push overflow")
  {
    for (int n = 0; n < 10; ++n) {
      stack.push(n);
    }
    CHECK(10 == stack.count());
    CHECK_THROWS_AS(stack.push(10), std::exception);
    const int m = 10;
    CHECK_THROWS_AS(stack.push(m), std::exception);
  }
  SECTION("pop")
  {
    stack.push(42);
    stack.push(815);
    CHECK(2 == stack.count());
    stack.pop();
    CHECK(1 == stack.count());
    CHECK_FALSE(stack.empty());
    stack.pop();
    CHECK(stack.empty());
  }
  SECTION("pop empty")
  {
    CHECK_THROWS_AS(stack.pop(), std::exception);
  }
  SECTION("top")
  {
    stack.push(42);
    stack.push(815);
    CHECK(815 == stack.top());
    CHECK(815 == stack.top());
    stack.pop();
    CHECK(42 == stack.top());
    stack.top() = 4711;
    CHECK(4711 == stack.top());
  }
  SECTION("top empty")
  {
    CHECK_THROWS_AS(stack.top(), std::exception);
  }
  SECTION("next")
  {
    stack.push(42);
    stack.push(815);
    CHECK(815 == stack.next());
    CHECK(42 == stack.next());
    CHECK_THROWS_AS(stack.next(), std::exception);
  }
  SECTION("next empty")
  {
    CHECK_THROWS_AS(stack.next(), std::exception);
  }
}

TEST_CASE("top stack operations", "[stack]")
{
  sunray::script::Stack<int, 10> stack;

  SECTION("top minus n")
  {
    for (int n = 0; n < 5; ++n) {
      stack.push(n);
    }
    CHECK(4 == stack.top());
    CHECK(4 == stack.top(0));
    CHECK(3 == stack.top(1));
    CHECK(2 == stack.top(2));
    CHECK(1 == stack.top(3));
    CHECK(0 == stack.top(4));
    CHECK_THROWS_AS(stack.top(5), std::exception);
  }
}

TEST_CASE("pop stack operations", "[stack]")
{
  sunray::script::Stack<int, 10> stack;

  SECTION("pop n")
  {
    for (int n = 0; n < 5; ++n) {
      stack.push(n);
    }

    stack.pop(3);
    CHECK(1 == stack.top());
  }
}
