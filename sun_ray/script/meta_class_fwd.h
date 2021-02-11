//
//  meta_class_fwd.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 09.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <memory>
#include <string>


namespace sunray
{
  namespace script
  {
    class FunctionRegistry;

    class IMetaClass;
    using MetaClassPtr = std::shared_ptr<const IMetaClass>;
    using MutableMetaClassPtr = std::shared_ptr<IMetaClass>;

    class IMetaClass
    {
    public:
      virtual ~IMetaClass() = default;

      virtual const std::string& name() const = 0;
      virtual void init(FunctionRegistry& registry) = 0;
    };


    class IClass;
    using ClassPtr = std::shared_ptr<const IClass>;
    using MutableClassPtr = std::shared_ptr<IClass>;

    class IClass
    {
    public:
      virtual ~IClass() = default;

      virtual std::string to_string() const = 0;
      virtual const MetaClassPtr& meta_class() const = 0;
    };

  }
}
