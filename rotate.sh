#!/bin/bash

# rotates the cow

rm amiga_rotation

for DEG in $(seq 6 12 360 );do
   DEGREES=$(printf %03d "$DEG")

   ~/Desktop/SelcoGit/af_iff_structure/iff_structure_linux ~/Desktop/lwtoy/lwtoy_demo/lwobj/Cow | awk 'BEGIN{print "3DG1"; print "3001"}' > cow_${DEGREES}
   ~/Desktop/SelcoGit/af_iff_structure/iff_structure_linux ~/Desktop/lwtoy/lwtoy_demo/lwobj/Cow | awk -v DEGREES=$DEGREES '/^ +[0-9]+)/{print $2*1200, $3*1200, $4*1200, 0, DEGREES, 0; next;}' | ~/Desktop/SelcoGit/af_rotatepoints/Debug/af_rotatepoints >> cow_${DEGREES}
   ~/Desktop/SelcoGit/af_iff_structure/iff_structure_linux ~/Desktop/lwtoy/lwtoy_demo/lwobj/Cow | awk '/^pols_[0-9]+/{gsub(/^pols_[0-9]+/,"",$0); print $0;}' >>cow_${DEGREES}

   # create Amiga script, call with "timeit execute amiga_rotation" on Amiga
   echo "af_tracer cow_$DEGREES cowopts 4" >>amiga_rotation

done

