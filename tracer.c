#include <stdlib.h>

#ifdef WINDOWED_UI
#include <SDL2/SDL.h>
#endif // WINDOWED_UI

#include "tracer.h"
#include "math.h"
#include "free.h"
#include "load.h"
#include "image.h"
#include "write.h"
#include <time.h>


struct ViewOpts vopts;

struct Polygon *polys = NULL;

struct Vertex *verts = NULL;

unsigned char *red = NULL, *grn = NULL, *blu = NULL;

int npoly, nvert;

int widtharg, heightarg, scrw, scrh;

float gnx, gny, gnz;

#ifdef WINDOWED_UI
int SDL_main(int argc, char *argv[])
{
    SDL_Renderer *rp = NULL;
    SDL_Window *wp = NULL;
#else // !WINDOWED_UI
void main(argc, argv)
int argc;
char **argv;
{
#endif // !WINDOWED_UI

   int err = 0;
   int returnStatus = 0; // exit valud from main
#ifdef WINDOWED_UI
      if(SDL_Init(SDL_INIT_VIDEO)) // SDL_Init returns non-zero for failure
      {
      puts("Failed to SDL_Init");
      returnStatus = 1; // failure 
      exit(returnStatus); // exit immediately if SDL doesn't initialize
      }
#endif // WINDOWED_UI

   /* Check for correct number of arguments. */

   if (argc != 5) {
      puts("Usage: tracer objectfile viewoptsfile width height");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      returnStatus = 1; // failure 
      return returnStatus;
   }

   /* Fetch pixel dimensions. */

   widtharg = atoi(argv[3]);
   heightarg = atoi(argv[4]);

   if (widtharg <= 0 || heightarg <= 0) {
      puts("Illegal dimensions specified");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      returnStatus = 1; // failure 
      return returnStatus;
   }


   /* Allocate object storage buffers. */
   polys = calloc(MAXPOLYS, sizeof(struct Polygon));

   if (polys == NULL) {

      puts("Unable to allocate 'polys' buffer");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      returnStatus = 1; // failure 
      return returnStatus;
   }

   verts = calloc(MAXVERTS, sizeof(struct Vertex));

   if (verts == NULL) {
      free(polys);

      puts("Unable to allocate 'verts' buffer");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      returnStatus = 1; // failure 
      return returnStatus;
   }

   /* Assign screen dimensions based on screen mode. */

   scrw = widtharg;
   scrh = heightarg;

   /* Allocate RGB buffers. */

   red = calloc(1, scrw * scrh);
   grn = calloc(1, scrw * scrh);
   blu = calloc(1, scrw * scrh);

   if (red == NULL || grn == NULL || blu == NULL) {
      if (red) free(red);
      if (grn) free(grn);
      if (blu) free(blu);

      free(verts);

      free(polys);

      puts("Unable to allocate RGB buffers");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      returnStatus = 1; // failure 
      return returnStatus;
   }

#ifdef WINDOWED_UI
   /* Open window. */

   SDL_CreateWindowAndRenderer(scrw, scrh, 0, &wp, &rp); // returns -1 on fail and wp=NULL

   if (wp == NULL) {

      free(red);
      free(grn);
      free(blu);

      free(verts);

      free(polys);

      puts("Unable to open window");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      returnStatus = 1; // failure 
      return returnStatus;
   }
#endif // WINDOWED_UI

   /* Initialize that no object exists. */

   npoly = nvert = 0;

   /* Initialize the ground's normal to point straight "up". */

   gnx = 0.0;
   gny = 1.0;
   gnz = 0.0;

   /* Load object to be displayed. */

   err = loadobject(argv[1]);

   if (err) {
      freevtxarrays();

#ifdef WINDOWED_UI
    SDL_DestroyRenderer(rp);
    SDL_DestroyWindow(wp);
#endif // WINDOWED_UI

      free(red);
      free(grn);
      free(blu);

      free(verts);

      free(polys);

      if (err == 1) puts("Error loading object");
      else
      if (err == 2) puts("Not a valid GEO object file");
      else
      if (err == 3) puts("Maximum vertex count exceeded");
      else
      if (err == 4) puts("Not enough memory for vertex arrays");
      else
      if (err == 5) puts("Maximum polygon count exceeded");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI
      returnStatus = 1; // failure 
      return returnStatus;
   }

   /* Load viewing options. */

   err = loadvopts(argv[2]);

   if (err) {
      freevtxarrays();

#ifdef WINDOWED_UI
    SDL_DestroyRenderer(rp);
    SDL_DestroyWindow(wp);
#endif // WINDOWED_UI

      free(red);
      free(grn);
      free(blu);

      free(verts);

      free(polys);

#ifdef WINDOWED_UI

#endif // WINDOWED_UI

      puts("DOS error loading view file");

#ifdef WINDOWED_UI
      SDL_Quit();
#endif // WINDOWED_UI

      returnStatus = 1; // failure 
      return returnStatus;
   }

   /* start timing computation */
   clock_t start_t, end_t;
   double total_t;
   start_t = clock();

   /* Call each function in the display process. */

   transform();

   calcnormals();

   traceimage(
#ifdef WINDOWED_UI
      rp
#endif / WINDOWED_UI
      );

   end_t = clock();
   total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
   printf("Total time taken by CPU: %f\n", total_t  );
   
   saveImageToFile(argv[1]);

   /* Free all allocations & close all openings. */

   freevtxarrays();

#ifdef WINDOWED_UI
    SDL_DestroyRenderer(rp);
    SDL_DestroyWindow(wp);
#endif // WINDOWED_UI

   free(red);
   free(grn);
   free(blu);

   free(verts);

   free(polys);

#ifdef WINDOWED_UI
   SDL_Quit();
#endif // WINDOWED_UI

   return returnStatus;
}

