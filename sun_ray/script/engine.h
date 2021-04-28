//
//  executor.h
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

#include <sun_ray/script/build_in_functions.h>
#include <sun_ray/script/parser.h>
#include <sun_ray/script/shapes/camera.h>
#include <sun_ray/script/shapes/canvas.h>
#include <sun_ray/script/shapes/checker_pattern.h>
#include <sun_ray/script/shapes/color.h>
#include <sun_ray/script/shapes/cone.h>
#include <sun_ray/script/shapes/cube.h>
#include <sun_ray/script/shapes/cylinder.h>
#include <sun_ray/script/shapes/disk.h>
#include <sun_ray/script/shapes/gradient_pattern.h>
#include <sun_ray/script/shapes/light.h>
#include <sun_ray/script/shapes/material.h>
#include <sun_ray/script/shapes/measurement.h>
#include <sun_ray/script/shapes/plane.h>
#include <sun_ray/script/shapes/point.h>
#include <sun_ray/script/shapes/ring_pattern.h>
#include <sun_ray/script/shapes/sphere.h>
#include <sun_ray/script/shapes/stripe_pattern.h>
#include <sun_ray/script/shapes/triangle.h>
#include <sun_ray/script/shapes/vector.h>
#include <sun_ray/script/shapes/world.h>
#include <sun_ray/script/visitor/dump_visitor.h>
#include <sun_ray/script/visitor/stack_machine_visitor.h>
#include <sun_ray/script/visitor/validation_visitor.h>


namespace sunray
{
  namespace script
  {
    class Engine
    {
    public:
      Engine(std::ostream& output, std::ostream& diagnostic_output, bool dump_script, bool dump_instructions)
      : output_{output}
      , diagnostic_output_{diagnostic_output}
      , dump_script_{dump_script}
      , dump_instructions_{dump_instructions}
      {
        meta_class_registry_.add_meta_class(std::make_shared<CameraMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<CanvasMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<CheckerPatternMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<ColorMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<ConeMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<CubeMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<CylinderMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<DiskMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<GradientPatternMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<LightMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<MaterialMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<MeasurementMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<PlaneMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<PointMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<RingPatternMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<SphereMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<StripePatternMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<TriangleMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<VectorMetaClass>());
        meta_class_registry_.add_meta_class(std::make_shared<WorldMetaClass>());
      }

      bool process(std::istream& input_stream)
      {
        DiagnosticMessageHandler diagnostic_messages;
        Parser parser{diagnostic_messages};
        const auto statements = parser.parse(input_stream);

        {
          SymbolTable sym_table;
          ValidationVisitor visitor{sym_table, diagnostic_messages};
          visitor.visit(statements);
        }

        if (diagnostic_messages.has_error()) {
          diagnostic_messages.output_messages(diagnostic_output_);
          return false;
        }

        if (statements.size()) {
          if (dump_script_) {
            DumpVisitor visitor{diagnostic_output_};
            visitor.visit(statements);
            diagnostic_output_ << "\n================================================================================\n\n";
          }

          {
            StackMachineVisitor visitor{function_registry_, meta_class_registry_};
            visitor.visit(statements);
            auto sm = visitor.machine();

            if (dump_instructions_) {
              sm.dump_instructions(diagnostic_output_);
              diagnostic_output_ << "\n================================================================================\n\n";
            }

            sm.run();
          }
        }

        return true;
      }

    private:
      std::ostream& output_;
      std::ostream& diagnostic_output_;
      bool dump_script_{false};
      bool dump_instructions_{false};
      FunctionRegistry function_registry_;
      BuildInFunctions buildin_functions_{function_registry_, output_};
      MetaClassRegistry meta_class_registry_{function_registry_};
    };
  }
}
