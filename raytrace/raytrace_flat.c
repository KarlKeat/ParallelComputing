#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define XRES 640
#define YRES 480
#define PIXSCALE 320

typedef struct triple
{
  double x ;
  double y ;
  double z ;
} triple;

typedef struct color
{
  char r;
  char g;
  char b;
}color;

typedef struct sphere
{
  double x;
  double y;
  double z;
  double r;
  struct color c;
}sphere;

triple eye = {.50, .50, -1.00};
triple light = {0, 1.25, -.50};

double dotProduct(triple a, triple b)
{
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

void diff(triple* a, triple b, triple c)
{
  a -> x = b.x - c.x;
  a -> y = b.y - c.y;
  a -> z = b.z - c.z;
}

double magnitude(triple t)
{
  return sqrt(pow(t.x,2) + pow(t.y,2) + pow(t.z,2));
}

void willCollide(sphere s, triple pix)
{
  triple t = {0};
  diff(&t, pix, eye);
  double mag = magnitude(t);
  t.x /= mag;
  t.y /= mag;
  t.z /= mag;

}

struct sphere arr[4] = {0};

void init()
{
   sphere a = {0};
   a.x =      0.50 ;
   a.y = -20000.00 ; // the floor
   a.z =      0.50 ;
   a.r =  20000.25 ;
   a.c.r =    205    ; // color is Peru
   a.c.g =    133    ;
   a.c.b =     63    ;

   //
   sphere b = {0};
   b.x  =     0.50 ;
   b.y  =     0.50 ;
   b.z  =     0.50 ;
   b.r  =     0.25 ;
   b.c.r =    0    ; // color is Blue
   b.c.g =    0    ;
   b.c.b =    255  ;
   //
   sphere c = {0};
   c.x =     1.00 ;
   c.y =     0.50 ;
   c.z =     1.00 ;
   c.r =     0.25 ;
   c.c.r =   0    ; // color is Green
   c.c.g =   255  ;
   c.c.b =   0    ;
   //
   sphere d = {0};
   d.x =      0.00 ;
   d.y =      0.75 ;
   d.z =      1.25 ;
   d.r =      0.50 ;
   d.c.r =    255  ; // color is Red
   d.c.g =    0    ;
   d.c.b =    0    ;

   arr[0] = a;
   arr[1] = b;
   arr[2] = c;
   arr[3] = d;
}

void printSphere(sphere s)
{
  printf("x:\t%f\ny:\t%f\nz:\t%f\nrad:\t%f\nr:\t%d\ng:\t%d\nb:\t%d\n\n", s.x, s.y, s.z, s.r, s.c.r, s.c.g, s.c.b);
}

int main(int argc, int* argv)
{
  init();
  int i;
  for(i = 0; i < 4; i++)
  {
    printSphere(arr[i]);
  }
}
