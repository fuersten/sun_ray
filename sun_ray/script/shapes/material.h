//
//  material.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 16.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/material.h>
#include <sun_ray/script/class.h>
#include <sun_ray/script/meta_class.h>
#include <sun_ray/script/shapes/color.h>
#include <sun_ray/script/shapes/pattern.h>


namespace sunray
{
  namespace script
  {
    class Material : public Class
    {
    public:
      Material(MetaClassPtr meta_class)
      : Class(meta_class)
      , material_{new sunray::Material}
      {
      }

      Material(MetaClassPtr meta_class, const Material& material)
      : Class(meta_class)
      , material_{new sunray::Material{*material.material_}}
      {
      }

      void color(const Color& color)
      {
        auto pattern = material_->pattern();
        material_.reset(new sunray::Material{color.color(), material_->ambient(), material_->diffuse(), material_->specular(),
                                             material_->shininess(), material_->reflective(), material_->transparency(),
                                             material_->refractive_index()});
        material_->pattern(pattern);
      }

      void ambient(float ambient)
      {
        auto pattern = material_->pattern();
        material_.reset(new sunray::Material{material_->color(), ambient, material_->diffuse(), material_->specular(),
                                             material_->shininess(), material_->reflective(), material_->transparency(),
                                             material_->refractive_index()});
        material_->pattern(pattern);
      }

      void diffuse(float diffuse)
      {
        auto pattern = material_->pattern();
        material_.reset(new sunray::Material{material_->color(), material_->ambient(), diffuse, material_->specular(),
                                             material_->shininess(), material_->reflective(), material_->transparency(),
                                             material_->refractive_index()});
        material_->pattern(pattern);
      }

      void specular(float specular)
      {
        auto pattern = material_->pattern();
        material_.reset(new sunray::Material{material_->color(), material_->ambient(), material_->diffuse(), specular,
                                             material_->shininess(), material_->reflective(), material_->transparency(),
                                             material_->refractive_index()});
        material_->pattern(pattern);
      }

      void shininess(float shininess)
      {
        auto pattern = material_->pattern();
        material_.reset(new sunray::Material{material_->color(), material_->ambient(), material_->diffuse(),
                                             material_->specular(), shininess, material_->reflective(), material_->transparency(),
                                             material_->refractive_index()});
        material_->pattern(pattern);
      }

      void reflective(float reflective)
      {
        auto pattern = material_->pattern();
        material_.reset(new sunray::Material{material_->color(), material_->ambient(), material_->diffuse(),
                                             material_->specular(), material_->shininess(), reflective, material_->transparency(),
                                             material_->refractive_index()});
        material_->pattern(pattern);
      }

      void transparency(float transparency)
      {
        auto pattern = material_->pattern();
        material_.reset(new sunray::Material{material_->color(), material_->ambient(), material_->diffuse(),
                                             material_->specular(), material_->shininess(), material_->reflective(), transparency,
                                             material_->refractive_index()});
        material_->pattern(pattern);
      }

      void refractive_index(float refractive_index)
      {
        auto pattern = material_->pattern();
        material_.reset(new sunray::Material{material_->color(), material_->ambient(), material_->diffuse(),
                                             material_->specular(), material_->shininess(), material_->reflective(),
                                             material_->transparency(), refractive_index});
        material_->pattern(pattern);
      }

      const sunray::Color& color() const
      {
        return material_->color();
      }

      double ambient() const
      {
        return material_->ambient();
      }

      double diffuse() const
      {
        return material_->diffuse();
      }

      double specular() const
      {
        return material_->specular();
      }

      double shininess() const
      {
        return material_->shininess();
      }

      double reflective() const
      {
        return material_->reflective();
      }

      double transparency() const
      {
        return material_->transparency();
      }

      double refractive_index() const
      {
        return material_->refractive_index();
      }

      void pattern(const Pattern& pattern)
      {
        material_->pattern(pattern.pattern());
      }

      bool has_pattern() const
      {
        return material_->pattern() ? true : false;
      }

      std::string to_string() const override
      {
        std::stringstream ss_col;
        ss_col << material_->color();

        return fmt::format("Material c: {{{}}} a: {} d: {} s: {} sh: {} r: {} t: {} ri: {}", ss_col.str(), material_->ambient(),
                           material_->diffuse(), material_->specular(), material_->shininess(), material_->reflective(),
                           material_->transparency(), material_->refractive_index());
      }

      const sunray::Material& material() const
      {
        return *material_;
      }

    private:
      std::unique_ptr<sunray::Material> material_;
    };


    class MaterialMetaClass : public sunray::script::MetaClass<Material>
    {
    public:
      MaterialMetaClass() = default;

      const std::string& name() const override
      {
        static const std::string name = "Material";
        return name;
      }

      void init(sunray::script::FunctionRegistry& registry) override
      {
        auto self = std::dynamic_pointer_cast<MaterialMetaClass>(shared_from_this());
        registry.add_variadic_function("Material_constructor", [self](const std::vector<Variant>& parameter) {
          if (parameter.empty()) {
            return self->construct();
          } else if (parameter.size() != 1) {
            throw std::runtime_error{fmt::format(
              "Material constructor called with wrong parameter count. Should be 0 or 1, but is {}.", parameter.size())};
          }
          return self->construct(as_class(parameter[0]));
        });
        registry.add_function("Material_set_color", color);
        registry.add_function("Material_set_ambient", ambient);
        registry.add_function("Material_set_diffuse", diffuse);
        registry.add_function("Material_set_specular", specular);
        registry.add_function("Material_set_shininess", shininess);
        registry.add_function("Material_set_reflective", reflective);
        registry.add_function("Material_set_transparency", transparency);
        registry.add_function("Material_set_refractive_index", refractive_index);
        registry.add_function("Material_set_pattern", pattern);
      }

      std::shared_ptr<Material> construct() const
      {
        return std::make_shared<Material>(shared_from_this());
      }

      std::shared_ptr<Material> construct(const sunray::script::MutableClassPtr& material) const
      {
        return std::make_shared<Material>(shared_from_this(), cast_object<Material, MaterialMetaClass>(material, "material"));
      }

    private:
      static double color(sunray::script::MutableClassPtr& c, const sunray::script::MutableClassPtr& color)
      {
        get_class(c)->color(cast_object<Color, ColorMetaClass>(color, "color"));
        return 0.0;
      }
      static double pattern(sunray::script::MutableClassPtr& c, const sunray::script::MutableClassPtr& pattern)
      {
        get_class(c)->pattern(cast_object<Pattern>(pattern, "Material", "pattern"));
        return 0.0;
      }
      static double ambient(sunray::script::MutableClassPtr& c, double ambient)
      {
        get_class(c)->ambient(static_cast<float>(ambient));
        return 0.0;
      }
      static double diffuse(sunray::script::MutableClassPtr& c, double diffuse)
      {
        get_class(c)->diffuse(static_cast<float>(diffuse));
        return 0.0;
      }
      static double specular(sunray::script::MutableClassPtr& c, double specular)
      {
        get_class(c)->specular(static_cast<float>(specular));
        return 0.0;
      }
      static double shininess(sunray::script::MutableClassPtr& c, double shininess)
      {
        get_class(c)->shininess(static_cast<float>(shininess));
        return 0.0;
      }
      static double reflective(sunray::script::MutableClassPtr& c, double reflective)
      {
        get_class(c)->reflective(static_cast<float>(reflective));
        return 0.0;
      }
      static double transparency(sunray::script::MutableClassPtr& c, double transparency)
      {
        get_class(c)->transparency(static_cast<float>(transparency));
        return 0.0;
      }
      static double refractive_index(sunray::script::MutableClassPtr& c, double refractive_index)
      {
        get_class(c)->refractive_index(static_cast<float>(refractive_index));
        return 0.0;
      }
    };
  }
}
