#!/bin/bash
FILE=$1
NAME=$2

rm $NAME*.ppm
rm $NAME*.png
rm movie.mpg

./sun_ray $FILE
mogrify -format png $NAME*.ppm
mogrify -resize 50x50% $NAME*.png
convert -delay 8 $NAME*.png m2v:movie.mpg

