#!/bin/sh

set -xe

format=clang-format

apps_files=`find ./apps -name "*.h" -o -name "*.cpp" |xargs`
src_files=`find ./src -name "*.cpp"  -o -name "*.h" |xargs`
hdr_files=`find ./sun_ray -name "*.h" -o -name "*.ipp" |xargs`
sunray_files=`find ./sun_ray -name "*.cpp" |xargs`
test_files=`find ./test -name "*.cpp" |xargs`

$format -i -sort-includes $apps_files $hdr_files $src_files $sunray_files $test_files

