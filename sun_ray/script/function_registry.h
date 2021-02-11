//
//  function_registry.h
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

#include <sun_ray/script/function_object.h>
#include <sun_ray/script/name_mangler.h>


namespace sunray
{
  namespace script
  {
    class FunctionRegistry
    {
    public:
      FunctionRegistry() = default;

      ~FunctionRegistry() = default;

      FunctionRegistry(const FunctionRegistry&) = delete;
      FunctionRegistry(FunctionRegistry&&) = delete;
      FunctionRegistry& operator=(const FunctionRegistry&) = delete;
      FunctionRegistry& operator=(FunctionRegistry&&) = delete;

      template<typename F>
      int64_t add_function(const std::string& name, F f)
      {
        typedef decltype(f) function_type;
        auto signature = NameMangler::mangle(name, function_traits<function_type>::arity);
        auto it = std::find_if(functions_.begin(), functions_.end(), [&signature](const auto& func) {
          return func.first == signature;
        });
        if (it != functions_.end()) {
          return -1;
        }

        registerFunction(functions_, std::move(signature), f);
        return static_cast<int64_t>(functions_.size() - 1);
      }

      template<typename F>
      int64_t add_variadic_function(const std::string& name, F&& f)
      {
        auto signature = NameMangler::mangle(name, 1);
        auto it = std::find_if(functions_.begin(), functions_.end(), [&signature](const auto& func) {
          return func.first == signature;
        });
        if (it != functions_.end()) {
          return -1;
        }

        registerVariadicFunction(functions_, std::move(signature), std::move(f));
        return static_cast<int64_t>(functions_.size() - 1);
      }

      Variant call_function(size_t index, const std::vector<Variant>& parameter) const
      {
        if (index > functions_.size()) {
          throw std::out_of_range{fmt::format("Index {} for function out of range", index)};
        }

        try {
          return functions_[index].second->call(parameter);
        } catch (const std::exception& ex) {
          throw std::runtime_error{
            fmt::format("Function '{}' error: {}", NameMangler::demangle(functions_[index].first), ex.what())};
        }
      }

      int64_t index_for_function(const std::string& signature) const
      {
        size_t index{0};
        for (const auto& [sig, func] : functions_) {
          if (sig == signature) {
            return static_cast<int64_t>(index);
          }
          ++index;
        }

        return -1;
      }

    private:
      Functions functions_;
    };
  }
}
