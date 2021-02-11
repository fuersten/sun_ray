//
//  meta_class.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 08.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/script/function_registry.h>
#include <sun_ray/script/meta_class_fwd.h>

#include <map>


namespace sunray
{
  namespace script
  {
    template<typename T>
    class MetaClass
    : public IMetaClass
    , public std::enable_shared_from_this<MetaClass<T>>
    {
    public:
      virtual ~MetaClass() = default;

      static const T* get_class(const sunray::script::ClassPtr& c)
      {
        return dynamic_cast<const T*>(c.get());
      }

      static T* get_class(sunray::script::MutableClassPtr& c)
      {
        return dynamic_cast<T*>(c.get());
      }

    protected:
      MetaClass() = default;
    };

    template<typename T, typename M>
    const T& cast_object(const sunray::script::MutableClassPtr& object, const std::string& name)
    {
      auto t = std::dynamic_pointer_cast<T>(object);
      if (!t) {
        throw std::runtime_error{fmt::format("{0} add wrong parameter for {1}. Should be {0} but is {2}.", M{}.name(), name,
                                             object ? object->meta_class()->name() : "null")};
      }
      return *t;
    }

    template<typename T>
    const T& cast_object(const sunray::script::MutableClassPtr& object, const std::string& class_name, const std::string& name)
    {
      auto t = std::dynamic_pointer_cast<T>(object);
      if (!t) {
        throw std::runtime_error{fmt::format("{0} add wrong parameter for {1}. Should be {0} but is {2}.", class_name, name,
                                             object ? object->meta_class()->name() : "null")};
      }
      return *t;
    }

    class MetaClassRegistry
    {
    public:
      MetaClassRegistry(FunctionRegistry& registry)
      : registry_{registry}
      {
      }

      bool add_meta_class(MutableMetaClassPtr meta_class)
      {
        auto it = classes_.find(meta_class->name());
        if (it != classes_.end()) {
          return false;
        }

        meta_class->init(registry_);
        classes_.emplace(meta_class->name(), meta_class);
        return true;
      }

      MetaClassPtr find(const std::string& name) const
      {
        auto it = classes_.find(name);
        return it == classes_.end() ? MetaClassPtr{} : it->second;
      }

    private:
      std::map<std::string, MetaClassPtr> classes_;
      FunctionRegistry& registry_;
    };
  }
}
