#!/bin/bash
FILE=$1
NAME=$2

rm $NAME*.png
rm $NAME.mpg

./sun_ray $FILE
mogrify -resize 50x50% $NAME*.png
convert -delay 8 $NAME*.png m2v:$NAME.mpg

