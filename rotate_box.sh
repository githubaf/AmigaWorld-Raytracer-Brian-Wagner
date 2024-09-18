#!/bin/bash

rm -f amiga_rotation

for DEG in $(seq 0 12 360 );do
   DEGREES=$(printf %03d "$DEG")

~/Desktop/SelcoGit/af_iff_structure/iff_structure_linux ~/Desktop/lwtoy/lwtoy_demo/lwobj/Cow | \
awk  -v DEGREES="$DEGREES" 'BEGIN{
   xmin=1000000;xmax=-1000000;ymin=xmin;ymax=xmax;zmin=xmin;zmax=xmax;
} 

/^ +[0-9]+)/{
   x=$2*1200; y=$3*1200; z=$4*1200; 
   if(x<xmin){xmin=x;}else if(x>xmax){xmax=x;} 
   if(y<ymin){ymin=y;}else if(y>ymax){ymax=y;}
   if(z<zmin){zmin=z;}else if(z>zmax){zmax=z;}
   next;
} 

END{ 
   /* printf("xmin=%d xmax=%d\n",xmin,xmax);printf("ymin=%d ymax=%d\n",ymin,ymax);printf("zmin=%d zmax=%d\n",zmin,zmax); printf("\nBounding Box\n------------\n"); */
   /* Rotate box by x times 12 Degrees */
   printf("3DG1\n8\n");
   cmd=sprintf("echo %f %f %f 0 %f 0 | ~/Desktop/SelcoGit/af_rotatepoints/Debug/af_rotatepoints", xmin, ymin, zmax, DEGREES); system (cmd); close(cmd);
   cmd=sprintf("echo %f %f %f 0 %f 0 | ~/Desktop/SelcoGit/af_rotatepoints/Debug/af_rotatepoints", xmax, ymin, zmax, DEGREES); system (cmd); close(cmd);
   cmd=sprintf("echo %f %f %f 0 %f 0 | ~/Desktop/SelcoGit/af_rotatepoints/Debug/af_rotatepoints", xmax, ymax, zmax, DEGREES); system (cmd); close(cmd);
   cmd=sprintf("echo %f %f %f 0 %f 0 | ~/Desktop/SelcoGit/af_rotatepoints/Debug/af_rotatepoints", xmin, ymax, zmax, DEGREES); system (cmd); close(cmd);
   cmd=sprintf("echo %f %f %f 0 %f 0 | ~/Desktop/SelcoGit/af_rotatepoints/Debug/af_rotatepoints", xmin, ymin, zmin, DEGREES); system (cmd); close(cmd);
   cmd=sprintf("echo %f %f %f 0 %f 0 | ~/Desktop/SelcoGit/af_rotatepoints/Debug/af_rotatepoints", xmax, ymin, zmin, DEGREES); system (cmd); close(cmd);
   cmd=sprintf("echo %f %f %f 0 %f 0 | ~/Desktop/SelcoGit/af_rotatepoints/Debug/af_rotatepoints", xmax, ymax, zmin, DEGREES); system (cmd); close(cmd);
   cmd=sprintf("echo %f %f %f 0 %f 0 | ~/Desktop/SelcoGit/af_rotatepoints/Debug/af_rotatepoints", xmin, ymax, zmin, DEGREES); system (cmd); close(cmd);
   printf("4 %u %u %u %u %u\n",0,1,2,3,7);
   printf("4 %u %u %u %u %u\n",4,5,6,7,7);
   printf("4 %u %u %u %u %u\n",0,1,5,4,7);
   printf("4 %u %u %u %u %u\n",3,2,6,7,7);
   printf("4 %u %u %u %u %u\n",0,4,7,3,4);
   printf("4 %u %u %u %u %u\n",1,5,6,2,7);}' > "cowbox_${DEGREES}"

   # create Amiga script, call with "timeit execute amiga_rotation" on Amiga
   echo "af_tracer cowbox_$DEGREES cowopts 4" >>amiga_rotation


done
