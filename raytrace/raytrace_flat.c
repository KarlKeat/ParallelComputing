#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define XRES 720
#define YRES 480

typedef struct triple
{
  double x ;
  double y ;
  double z ;
}triple;

typedef struct color
{
  int r;
  int g;
  int b;
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

double willCollide(sphere sph, triple pix)
{
  triple unitVector = {0};
  diff(&unitVector, pix, eye);
  double mag = magnitude(unitVector);
  unitVector.x /= mag;
  unitVector.y /= mag;
  unitVector.z /= mag;

  double b = 2*(unitVector.x*(pix.x-sph.x)+unitVector.y*(pix.y-sph.y)+unitVector.z*(pix.z-sph.z));
  double c = pow(pix.x-sph.x,2)+pow(pix.y-sph.y,2)+pow(pix.z-sph.z,2)-pow(sph.r,2);

  double discriminant = pow(b,2) - 4*c;

  if(discriminant >= 0)
  {
    double r1 = (-b - sqrt(pow(b,2)-4*c))/2.0;
    double r2 = (-b + sqrt(pow(b,2)-4*c))/2.0;
    triple ray = {0};
    if(r1 < r2 && r1 > 0)
    {
      ray.x = pix.x + r1*unitVector.x;
      ray.y = pix.y + r1*unitVector.y;
      ray.z = pix.z + r1*unitVector.z;
      return magnitude(ray);
    }
    else if(r2 > 0)
    {
      ray.x = pix.x + r2*unitVector.x;
      ray.y = pix.y + r2*unitVector.y;
      ray.z = pix.z + r2*unitVector.z;
      return magnitude(ray);
    }
    return -1;
  }
  else
    return -1;
}

double min(double a, double b)
{
  if(a < b)
    return a;
  else
    return b;
}

struct sphere arr[4] = {0};

void init()
{
   sphere a = {0};
   a.x =      0.50 ;
   a.y = -20000.00 ; // the floor
   a.z =      0.50 ;
   a.r =  20000.25 ;
   a.c.r =    205  ; // color is Peru
   a.c.g =    133  ;
   a.c.b =     63  ;

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
  double render[XRES][YRES] = {0};
  color colorArr[XRES][YRES] = {0};
  color blank = {0, 0, 0};

  int i;
  for(i = 0; i < 4; i++)
  {
    //printSphere(arr[i]);
    int xpx = 0;
    int ypx = 0;
    double x, y;
    for(y = 0; y < 1.0; y+=1.0/YRES)
    {
      for(x = 0; x < 1.5; x+=1.5/XRES)
      {
        triple temp = {x, y, 0};
        //printf("%d\n", ypx);
        double result = willCollide(arr[i], temp);
        if(result != -1 && (render[xpx][ypx] == 0.0 || result < render[xpx][ypx]))
        {
          render[xpx][ypx] = result;
          colorArr[xpx][ypx] = arr[i].c;
        }
        xpx++;
      }
      xpx = 0;
      //printf("%d\n", xpx);
      ypx++;
    }

  }

  int x,y;
  FILE* fout = fopen( "img.ppm" , "w" ) ;
  //
  fprintf( fout , "P3\n" ) ;
  fprintf( fout , "%d %d\n" , XRES , YRES ) ;
  fprintf( fout , "255\n" ) ;
  //
  for( y = YRES-1 ; y >=0 ; y-- )
  {
     for( x = 0 ; x < XRES ; x++ )
     {
        fprintf( fout , "%d %d %d\n" ,
         colorArr[x][y].r , colorArr[x][y].g , colorArr[x][y].b ) ;
     }
  }
  close( fout ) ;
  //
  return 0 ;


}
