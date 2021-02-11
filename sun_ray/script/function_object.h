//
//  function_object.h
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

#include <sun_ray/script/format_helper.h>
#include <sun_ray/script/function_traits.h>
#include <sun_ray/script/types.h>

#include <vector>


namespace sunray
{
  namespace script
  {
    class FunctionObject;
    using Functions = std::vector<std::pair<std::string, sunray::script::FunctionObject*>>;

    template<typename T, typename P>
    void check_parameter(const P& param, uint8_t num)
    {
      using TT = typename std::remove_reference<T>::type;
      if (!unwrap<T>::is_type(param)) {
        Variant dummy = TT{};
        throw std::runtime_error{fmt::format("Called with wrong parameter type for parameter {}. Should be {}, but is {}.", num,
                                             type_from_variant(dummy)->as_string(), type_from_variant(param)->as_string())};
      }
    }

    class FunctionObject
    {
    public:
      virtual ~FunctionObject() = default;

      FunctionObject(const FunctionObject&) = delete;
      FunctionObject(FunctionObject&&) = delete;
      FunctionObject& operator=(const FunctionObject&) = delete;
      FunctionObject& operator=(FunctionObject&&) = delete;

      Variant call(const std::vector<Variant>& parameter)
      {
        return do_call(parameter);
      }

    protected:
      virtual Variant do_call(const std::vector<Variant>& parameter) = 0;

      FunctionObject() = default;
    };


    template<typename F, typename R>
    class FunctionObjectImpl0 : public FunctionObject
    {
    public:
      FunctionObjectImpl0(F f)
      : f_(f)
      {
      }

    private:
      Variant do_call(const std::vector<Variant>& parameter) override
      {
        if (!parameter.empty()) {
          throw std::runtime_error{
            fmt::format("Called with wrong parameter count. Should be {}, but is {}.", 0, parameter.size())};
        }
        return Variant{f_()};
      }

      F f_;
    };

    template<typename F, typename R, typename T0>
    class FunctionObjectImpl1 : public FunctionObject
    {
    public:
      FunctionObjectImpl1(F f)
      : f_(f)
      {
      }

    private:
      Variant do_call(const std::vector<Variant>& parameter) override
      {
        if (parameter.size() != 1) {
          throw std::runtime_error{
            fmt::format("Called with wrong parameter count. Should be {}, but is {}.", 1, parameter.size())};
        }
        check_parameter<T0>(parameter[0], 0);
        auto t0 = unwrap<T0>::as_type(parameter[0]);
        return Variant{f_(t0)};
      }

      F f_;
    };

    template<typename F, typename R, typename T0, typename T1>
    class FunctionObjectImpl2 : public FunctionObject
    {
    public:
      FunctionObjectImpl2(F f)
      : f_(f)
      {
      }

    private:
      Variant do_call(const std::vector<Variant>& parameter) override
      {
        if (parameter.size() != 2) {
          throw std::runtime_error{
            fmt::format("Called with wrong parameter count. Should be {}, but is {}.", 2, parameter.size())};
        }
        check_parameter<T0>(parameter[0], 0);
        check_parameter<T1>(parameter[1], 1);
        auto t0 = unwrap<T0>::as_type(parameter[0]);
        auto t1 = unwrap<T1>::as_type(parameter[1]);
        return Variant{f_(t0, t1)};
      }

      F f_;
    };

    template<typename F, typename R, typename T0, typename T1, typename T2>
    class FunctionObjectImpl3 : public FunctionObject
    {
    public:
      FunctionObjectImpl3(F f)
      : f_(f)
      {
      }

    private:
      Variant do_call(const std::vector<Variant>& parameter) override
      {
        if (parameter.size() != 3) {
          throw std::runtime_error{
            fmt::format("Called with wrong parameter count. Should be {}, but is {}.", 3, parameter.size())};
        }
        check_parameter<T0>(parameter[0], 0);
        check_parameter<T1>(parameter[1], 1);
        check_parameter<T2>(parameter[2], 2);
        auto t0 = unwrap<T0>::as_type(parameter[0]);
        auto t1 = unwrap<T1>::as_type(parameter[1]);
        auto t2 = unwrap<T2>::as_type(parameter[2]);
        return Variant{f_(t0, t1, t2)};
      }

      F f_;
    };

    template<typename F, typename R, typename T0, typename T1, typename T2, typename T3>
    class FunctionObjectImpl4 : public FunctionObject
    {
    public:
      FunctionObjectImpl4(F f)
      : f_(f)
      {
      }

    private:
      Variant do_call(const std::vector<Variant>& parameter) override
      {
        if (parameter.size() != 4) {
          throw std::runtime_error{
            fmt::format("Called with wrong parameter count. Should be {}, but is {}.", 4, parameter.size())};
        }
        check_parameter<T0>(parameter[0], 0);
        check_parameter<T1>(parameter[1], 1);
        check_parameter<T2>(parameter[2], 2);
        check_parameter<T3>(parameter[3], 3);
        auto t0 = unwrap<T0>::as_type(parameter[0]);
        auto t1 = unwrap<T1>::as_type(parameter[1]);
        auto t2 = unwrap<T2>::as_type(parameter[2]);
        auto t3 = unwrap<T3>::as_type(parameter[3]);
        return Variant{f_(t0, t1, t2, t3)};
      }

      F f_;
    };

    template<typename F, typename R, typename T0, typename T1, typename T2, typename T3, typename T4>
    class FunctionObjectImpl5 : public FunctionObject
    {
    public:
      FunctionObjectImpl5(F f)
      : f_(f)
      {
      }

    private:
      Variant do_call(const std::vector<Variant>& parameter) override
      {
        if (parameter.size() != 5) {
          throw std::runtime_error{
            fmt::format("Called with wrong parameter count. Should be {}, but is {}.", 5, parameter.size())};
        }
        check_parameter<T0>(parameter[0], 0);
        check_parameter<T1>(parameter[1], 1);
        check_parameter<T2>(parameter[2], 2);
        check_parameter<T3>(parameter[3], 3);
        check_parameter<T4>(parameter[4], 4);
        auto t0 = unwrap<T0>::as_type(parameter[0]);
        auto t1 = unwrap<T1>::as_type(parameter[1]);
        auto t2 = unwrap<T2>::as_type(parameter[2]);
        auto t3 = unwrap<T3>::as_type(parameter[3]);
        auto t4 = unwrap<T4>::as_type(parameter[4]);
        return Variant{f_(t0, t1, t2, t3, t4)};
      }

      F f_;
    };

    template<typename F, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    class FunctionObjectImpl6 : public FunctionObject
    {
    public:
      FunctionObjectImpl6(F f)
      : f_(f)
      {
      }

    private:
      Variant do_call(const std::vector<Variant>& parameter) override
      {
        if (parameter.size() != 6) {
          throw std::runtime_error{
            fmt::format("Called with wrong parameter count. Should be {}, but is {}.", 6, parameter.size())};
        }
        check_parameter<T0>(parameter[0], 0);
        check_parameter<T1>(parameter[1], 1);
        check_parameter<T2>(parameter[2], 2);
        check_parameter<T3>(parameter[3], 3);
        check_parameter<T4>(parameter[4], 4);
        check_parameter<T5>(parameter[5], 5);
        auto t0 = unwrap<T0>::as_type(parameter[0]);
        auto t1 = unwrap<T1>::as_type(parameter[1]);
        auto t2 = unwrap<T2>::as_type(parameter[2]);
        auto t3 = unwrap<T3>::as_type(parameter[3]);
        auto t4 = unwrap<T4>::as_type(parameter[4]);
        auto t5 = unwrap<T5>::as_type(parameter[5]);
        return Variant{f_(t0, t1, t2, t3, t4, t5)};
      }

      F f_;
    };

    template<typename F, typename R, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    class FunctionObjectImpl7 : public FunctionObject
    {
    public:
      FunctionObjectImpl7(F f)
      : f_(f)
      {
      }

    private:
      Variant do_call(const std::vector<Variant>& parameter) override
      {
        if (parameter.size() != 7) {
          throw std::runtime_error{
            fmt::format("Called with wrong parameter count. Should be {}, but is {}.", 7, parameter.size())};
        }
        check_parameter<T0>(parameter[0], 0);
        check_parameter<T1>(parameter[1], 1);
        check_parameter<T2>(parameter[2], 2);
        check_parameter<T3>(parameter[3], 3);
        check_parameter<T4>(parameter[4], 4);
        check_parameter<T5>(parameter[5], 5);
        check_parameter<T6>(parameter[6], 6);
        auto t0 = unwrap<T0>::as_type(parameter[0]);
        auto t1 = unwrap<T1>::as_type(parameter[1]);
        auto t2 = unwrap<T2>::as_type(parameter[2]);
        auto t3 = unwrap<T3>::as_type(parameter[3]);
        auto t4 = unwrap<T4>::as_type(parameter[4]);
        auto t5 = unwrap<T5>::as_type(parameter[5]);
        auto t6 = unwrap<T6>::as_type(parameter[6]);
        return Variant{f_(t0, t1, t2, t3, t4, t5, t6)};
      }

      F f_;
    };


    class FunctionObjectImpl : public FunctionObject
    {
    public:
      FunctionObjectImpl(std::function<Variant(const std::vector<Variant>&)> f)
      : f_{std::move(f)}
      {
      }

    private:
      Variant do_call(const std::vector<Variant>& parameter) override
      {
        return Variant{f_(parameter)};
      }

      std::function<Variant(const std::vector<Variant>&)> f_;
    };


    template<typename F>
    void registerFunctionN(Functions& funcs, std::string name, F f, int2type<0>)
    {
      typedef decltype(f) function_type;

      using R = typename function_traits<function_type>::return_type;
      typedef FunctionObjectImpl0<F, R> F0;

      FunctionObject* func = new F0(f);
      funcs.emplace_back(std::make_pair(std::move(name), func));
    }

    template<typename F>
    void registerFunctionN(Functions& funcs, std::string name, F f, int2type<1>)
    {
      typedef decltype(f) function_type;

      using R = typename function_traits<function_type>::return_type;
      using T0 = typename function_traits<function_type>::template argument<0>::type;
      typedef FunctionObjectImpl1<F, R, T0> F1;

      FunctionObject* func = new F1(f);
      funcs.emplace_back(std::make_pair(std::move(name), func));
    }

    template<typename F>
    void registerFunctionN(Functions& funcs, std::string name, F f, int2type<2>)
    {
      typedef decltype(f) function_type;

      using R = typename function_traits<function_type>::return_type;
      using T0 = typename function_traits<function_type>::template argument<0>::type;
      using T1 = typename function_traits<function_type>::template argument<1>::type;
      typedef FunctionObjectImpl2<F, R, T0, T1> F2;

      FunctionObject* func = new F2(f);
      funcs.emplace_back(std::make_pair(std::move(name), func));
    }

    template<typename F>
    void registerFunctionN(Functions& funcs, std::string name, F f, int2type<3>)
    {
      typedef decltype(f) function_type;

      using R = typename function_traits<function_type>::return_type;
      using T0 = typename function_traits<function_type>::template argument<0>::type;
      using T1 = typename function_traits<function_type>::template argument<1>::type;
      using T2 = typename function_traits<function_type>::template argument<2>::type;
      typedef FunctionObjectImpl3<F, R, T0, T1, T2> F3;

      FunctionObject* func = new F3(f);
      funcs.emplace_back(std::make_pair(std::move(name), func));
    }

    template<typename F>
    void registerFunctionN(Functions& funcs, std::string name, F f, int2type<4>)
    {
      typedef decltype(f) function_type;

      using R = typename function_traits<function_type>::return_type;
      using T0 = typename function_traits<function_type>::template argument<0>::type;
      using T1 = typename function_traits<function_type>::template argument<1>::type;
      using T2 = typename function_traits<function_type>::template argument<2>::type;
      using T3 = typename function_traits<function_type>::template argument<3>::type;
      typedef FunctionObjectImpl4<F, R, T0, T1, T2, T3> F4;

      FunctionObject* func = new F4(f);
      funcs.emplace_back(std::make_pair(std::move(name), func));
    }

    template<typename F>
    void registerFunctionN(Functions& funcs, std::string name, F f, int2type<5>)
    {
      typedef decltype(f) function_type;

      using R = typename function_traits<function_type>::return_type;
      using T0 = typename function_traits<function_type>::template argument<0>::type;
      using T1 = typename function_traits<function_type>::template argument<1>::type;
      using T2 = typename function_traits<function_type>::template argument<2>::type;
      using T3 = typename function_traits<function_type>::template argument<3>::type;
      using T4 = typename function_traits<function_type>::template argument<4>::type;
      typedef FunctionObjectImpl5<F, R, T0, T1, T2, T3, T4> F5;

      FunctionObject* func = new F5(f);
      funcs.emplace_back(std::make_pair(std::move(name), func));
    }

    template<typename F>
    void registerFunctionN(Functions& funcs, std::string name, F f, int2type<6>)
    {
      typedef decltype(f) function_type;

      using R = typename function_traits<function_type>::return_type;
      using T0 = typename function_traits<function_type>::template argument<0>::type;
      using T1 = typename function_traits<function_type>::template argument<1>::type;
      using T2 = typename function_traits<function_type>::template argument<2>::type;
      using T3 = typename function_traits<function_type>::template argument<3>::type;
      using T4 = typename function_traits<function_type>::template argument<4>::type;
      using T5 = typename function_traits<function_type>::template argument<5>::type;
      typedef FunctionObjectImpl6<F, R, T0, T1, T2, T3, T4, T5> F6;

      FunctionObject* func = new F6(f);
      funcs.emplace_back(std::make_pair(std::move(name), func));
    }

    template<typename F>
    void registerFunctionN(Functions& funcs, std::string name, F f, int2type<7>)
    {
      typedef decltype(f) function_type;

      using R = typename function_traits<function_type>::return_type;
      using T0 = typename function_traits<function_type>::template argument<0>::type;
      using T1 = typename function_traits<function_type>::template argument<1>::type;
      using T2 = typename function_traits<function_type>::template argument<2>::type;
      using T3 = typename function_traits<function_type>::template argument<3>::type;
      using T4 = typename function_traits<function_type>::template argument<4>::type;
      using T5 = typename function_traits<function_type>::template argument<5>::type;
      using T6 = typename function_traits<function_type>::template argument<6>::type;
      typedef FunctionObjectImpl7<F, R, T0, T1, T2, T3, T4, T5, T6> F7;

      FunctionObject* func = new F7(f);
      funcs.emplace_back(std::make_pair(std::move(name), func));
    }

    template<typename F>
    void registerFunction(Functions& funcs, std::string name, F f)
    {
      registerFunctionN(funcs, std::move(name), f, int2type<function_traits<decltype(f)>::arity>());
    }

    template<typename Function>
    void registerVariadicFunction(Functions& funcs, std::string name, Function&& f)
    {
      FunctionObject* func = new FunctionObjectImpl(std::move(f));
      funcs.emplace_back(std::make_pair(std::move(name), func));
    }
  }
}
