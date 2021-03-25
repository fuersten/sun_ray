//
//  main.cpp
//  bullet
//
//  Created by Lars-Christian Fürstenberg on 24.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/canvas_file_writer.h>
#include <sun_ray/feature/tuple.h>
#include <sun_ray/init.h>

#include <iostream>


class Projectile
{
public:
  Projectile(sunray::Point position, sunray::Vector velocity)
  : position_{std::move(position)}
  , velocity_{std::move(velocity)}
  {
  }

  Projectile move(const sunray::Vector& environment) const
  {
    return Projectile{position_ + velocity_, velocity_ + environment};
  }

  const sunray::Point& position() const
  {
    return position_;
  }

  friend std::ostream& operator<<(std::ostream& stream, const Projectile& projectile)
  {
    stream << projectile.position_;
    return stream;
  }

private:
  sunray::Point position_;
  sunray::Vector velocity_;
};


static bool is_on_ground(const Projectile& projectile)
{
  std::cout << "Position " << projectile.position() << std::endl;
  return projectile.position().y() <= 0;
}

static Projectile move(const Projectile& projectile, const sunray::Vector& environment)
{
  return projectile.move(environment);
}


int main(int argc, const char* argv[])
{
  try {
    if (argc < 2) {
      std::cerr << "Usage: bullet <pathname>" << std::endl;
      return -1;
    }

    sunray::Canvas canvas{1300, 550};

    const sunray::Vector environment{sunray::create_vector(0, -0.1, 0) + sunray::create_vector(0.01, 0, 0)};

    Projectile projectile{sunray::create_point(0, 1, 0), sunray::create_vector(1, 1.8, 0).normalize() * 11.25};

    const auto orange = sunray::Color{1.0f, 0.647f, 0};
    while (true) {
      try {
        canvas.pixel_at(sunray::round(projectile.position().x()), canvas.height() - sunray::round(projectile.position().y()),
                        orange);
      } catch (const std::exception&) {
      }
      projectile = move(projectile, environment);
      if (is_on_ground(projectile)) {
        std::cout << "Hit the ground at " << projectile << std::endl;
        break;
      }
    }

    sunray::CanvasFileWriter writer{sunray::ImageFormat::PNG, std::filesystem::path{argv[1]} / "bullet.png"};
    writer.write(canvas);
  } catch (const std::exception& ex) {
    std::cerr << "Exception: " << ex.what() << std::endl;
  }

  return 0;
}
