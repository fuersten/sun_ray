//
//  class.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 08.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/script/meta_class_fwd.h>


namespace sunray
{
  namespace script
  {
    class Class : public IClass
    {
    public:
      const MetaClassPtr& meta_class() const
      {
        return meta_class_;
      }

    protected:
      Class(MetaClassPtr meta_class)
      : meta_class_{std::move(meta_class)}
      {
      }

    private:
      MetaClassPtr meta_class_;
    };
  }
}
