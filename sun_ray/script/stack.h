//
//  stack.h
//  sun_ray
//
//  BSD 3-Clause License
//  Copyright (c) 2020, Lars-Christian Fürstenberg
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//  this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//  contributors may be used to endorse or promote products derived from this
//  software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//

#pragma once

#include <stdexcept>

namespace sunray
{
  namespace script
  {
    template<typename T, size_t N>
    class Stack
    {
    public:
      Stack()
      : top_(-1)
      {
      }

      bool empty() const
      {
        return top_ < 0;
      }

      const T& top(size_t n = 0) const
      {
        auto idx = top_ - static_cast<int64_t>(n);
        if (idx >= 0) {
          return stack_[idx];
        } else {
          throw std::runtime_error("stack is empty");
        }
      }

      T& top(size_t n = 0)
      {
        auto idx = top_ - static_cast<int64_t>(n);
        if (idx >= 0) {
          return stack_[idx];
        } else {
          throw std::runtime_error("stack is empty");
        }
      }

      T next()
      {
        if (top_ >= 0) {
          return std::move(stack_[top_--]);
        } else {
          throw std::runtime_error("stack is empty");
        }
      }

      void pop(size_t n = 1)
      {
        while (n--) {
          if (top_ >= 0) {
            top_--;
          } else {
            throw std::runtime_error("stack is empty");
          }
        }
      }

      void push(T&& t)
      {
        if (top_ == -1 || top_ < static_cast<int64_t>(N - 1)) {
          ++top_;
          stack_[top_] = std::move(t);
        } else {
          throw std::runtime_error("stack is full");
        }
      }

      void push(const T& t)
      {
        if (top_ == -1 || top_ < static_cast<int64_t>(N - 1)) {
          ++top_;
          stack_[top_] = t;
        } else {
          throw std::runtime_error("stack is full");
        }
      }

      size_t count() const
      {
        if (top_ < 0) {
          return 0;
        }
        return static_cast<size_t>(top_ + 1);
      }

    private:
      T stack_[N];
      int64_t top_;
    };
  }
}
