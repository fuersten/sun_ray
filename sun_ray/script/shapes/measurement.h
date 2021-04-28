//
//  measurement.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 02.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/script/class.h>
#include <sun_ray/script/meta_class.h>

#include <chrono>


namespace sunray
{
  namespace script
  {
    class Measurement : public Class
    {
    public:
      Measurement(MetaClassPtr meta_class)
      : Class(meta_class)
      , tp_{std::chrono::steady_clock::now()}
      {
      }

      std::chrono::steady_clock::rep elapsed_time()
      {
        auto end = std::chrono::steady_clock::now();
        auto res = std::chrono::duration_cast<std::chrono::milliseconds>(end - tp_).count();
        tp_ = end;
        return res;
      }

      std::string to_string() const override
      {
        return fmt::format("Measurement");
      }

    private:
      std::chrono::steady_clock::time_point tp_;
    };


    class MeasurementMetaClass : public sunray::script::MetaClass<Measurement>
    {
    public:
      MeasurementMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Measurement";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<MeasurementMetaClass>(shared_from_this());
        registry.add_variadic_function("Measurement_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.empty()) {
            return self->construct();
          }
          throw std::runtime_error{
            fmt::format("Measurement constructor called with wrong parameter count. Should be 0, but is {}.", parameter.size())};
        });
        registry.add_function("Measurement_elapsed_time", elapsed_time);
      }

      std::shared_ptr<Measurement> construct() const
      {
        return std::make_shared<Measurement>(shared_from_this());
      }

    private:
      static double elapsed_time(sunray::script::MutableClassPtr& c)
      {
        return static_cast<double>(get_class(c)->elapsed_time());
      }
    };
  }
}
