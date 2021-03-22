# Building and installation

## Mac OS X 10.15 and newer

- You will need the following software packages
  - cmake-3.16 or higher
  - git
  - current XCode version

- Create a build directory in the source directory of sun_ray and change to it
- Call `cmake -DCMAKE_BUILD_TYPE=Release ..`
- Call `make`
- If you want to use XCode instead of make call `cmake -GXcode -DCMAKE_BUILD_TYPE=Release ..`

## Linux

- You will need the following software packages
  - cmake-3.16 or higher
  - git
  - clang++ 9 or higher compiler or g++ 9 or higher
  - libc++ 9 or higher

- Create a build directory in the source directory of sun_ray and change to it

- Call `cmake -DCMAKE_BUILD_TYPE=Release ..`
- Call `make`

## Windows

- You will need the following software packages
  - cmake-3.16 or higher
  - git
  - Visual Studio 2019 or newer

- CMake can be downloaded from [http://www.cmake.org/download](http://www.cmake.org/download)
- Create a build directory in the source directory of sun_ray and change to it
- Call `cmake -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Release ..`
- Use the project file to build with visual studio

