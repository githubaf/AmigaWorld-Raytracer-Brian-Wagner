#!/bin/bash

#for DEG in $(seq 0 12 360 );do 
#   DEGREES=$(printf "%03d" $DEG);
#   convert -depth 8 -size 640x400 gray:cowbox_$DEGREES.red gray:cowbox_$DEGREES.grn gray:cowbox_$DEGREES.blu -combine cowbox_$DEGREES.jpg
#done

# 

# usage
# ./make_anim.sh cow
# creates cow_000.jpg from cow_000.red cow_000.grn cow_000.blu
#         cow_012.jpg from cow_012.red cow_012.grn cow_012.blu
#         ...
# and finally a gif anim called cow.gif

for IMAGE in $(ls $1_[0-9][0-9][0-9].red | awk '{gsub(/.red/,"",$0); print}'); do
    convert -depth 8 -size 640x400 gray:$IMAGE.red gray:$IMAGE.grn gray:$IMAGE.blu -combine $IMAGE.jpg  
done

convert -delay 6.66 -loop 0 *.jpg $1.gif

eog $1.gif
