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
triple nulltrip = {-1, -1, -1};
struct sphere sphereArray[4] = {0};

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

triple unitVector(triple* t)
{
  double mag = 1/magnitude(*t);
  t -> x *= mag;
  t -> y *= mag;
  t -> z *= mag;
}

triple willCollide(sphere sph, triple pix)
{
  triple vector = {0};
  diff(&vector, pix, eye);
  unitVector(&vector);

  double b = 2*(vector.x*(pix.x-sph.x)+vector.y*(pix.y-sph.y)+vector.z*(pix.z-sph.z));
  double c = pow(pix.x-sph.x,2)+pow(pix.y-sph.y,2)+pow(pix.z-sph.z,2)-pow(sph.r,2);

  double discriminant = pow(b,2) - 4*c;

  if(discriminant >= 0)
  {
    double r1 = (-b - sqrt(pow(b,2)-4*c))/2.0;
    double r2 = (-b + sqrt(pow(b,2)-4*c))/2.0;
    triple ray = {0};
    if(r1 < r2 && r1 > 0)
    {
      ray.x = pix.x + r1*vector.x;
      ray.y = pix.y + r1*vector.y;
      ray.z = pix.z + r1*vector.z;
      return ray;
    }
    else if(r2 > 0)
    {
      ray.x = pix.x + r2*vector.x;
      ray.y = pix.y + r2*vector.y;
      ray.z = pix.z + r2*vector.z;
      return ray;
    }
    return nulltrip;
  }
  else
    return nulltrip;
}

triple shadeCollide(sphere sph, triple pix)
{
  triple vector = {0};
  diff(&vector, light, pix);
  unitVector(&vector);

  double b = 2*(vector.x*(pix.x-sph.x)+vector.y*(pix.y-sph.y)+vector.z*(pix.z-sph.z));
  double c = pow(pix.x-sph.x,2)+pow(pix.y-sph.y,2)+pow(pix.z-sph.z,2)-pow(sph.r,2);

  double discriminant = pow(b,2) - 4*c;

  if(discriminant >= 0)
  {
    double r1 = (-b - sqrt(pow(b,2)-4*c))/2.0;
    double r2 = (-b + sqrt(pow(b,2)-4*c))/2.0;
    triple ray = {0};
    if(r1 < r2 && r1 > 0)
    {
      ray.x = pix.x + r1*vector.x;
      ray.y = pix.y + r1*vector.y;
      ray.z = pix.z + r1*vector.z;
      return ray;
    }
    else if(r2 > 0)
    {
      ray.x = pix.x + r2*vector.x;
      ray.y = pix.y + r2*vector.y;
      ray.z = pix.z + r2*vector.z;
      return ray;
    }
    return nulltrip;
  }
  else
    return nulltrip;
}

double calcShading(triple pt, int current)
{
  //printf("%f, %f, %f\n", pt.x, pt.y, pt.z);
  int i;
  for(i = 0; i < 4; i++)
  {
    triple collision = shadeCollide(sphereArray[i], pt);
    if(i != current && !(collision.x == -1 && collision.y == -1 && collision.z == -1))
      return 0;
  }
  triple centerVector = {0};
  triple center = {0};
  center.x = sphereArray[i].x;
  center.y = sphereArray[i].y;
  center.z = sphereArray[i].z;
  diff(&centerVector, pt, center);

  pt.x += .000001 * centerVector.x;
  pt.y += .000001 * centerVector.y;
  pt.z += .000001 * centerVector.z;

  triple pointVector = {0};
  diff(&pointVector, light, pt);

  unitVector(&pointVector);
  unitVector(&centerVector);
  double result = dotProduct(pointVector, centerVector);
  if(result > 0)
  {
    return result;
  }
  return 0;
}

double min(double a, double b)
{
  if(a < b)
    return a;
  else
    return b;
}

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

   sphereArray[0] = a;
   sphereArray[1] = b;
   sphereArray[2] = c;
   sphereArray[3] = d;
}

void printSphere(sphere s)
{
  printf("x:\t%f\ny:\t%f\nz:\t%f\nrad:\t%f\nr:\t%d\ng:\t%d\nb:\t%d\n\n", s.x, s.y, s.z, s.r, s.c.r, s.c.g, s.c.b);
}

int main(int argc, int* argv)
{
  init();
  double render[XRES][YRES] = {0};
  color colorsphereArray[XRES][YRES] = {0};
  color blank = {0, 0, 0};

  int i;
  for(i = 0; i < 4; i++)
  {
    //printSphere(sphereArray[i]);
    int xpx = 0;
    int ypx = 0;
    double x, y;
    for(y = 0; y < 1.0; y+=1.0/YRES)
    {
      for(x = 0; x < 1.5; x+=1.5/XRES)
      {
        triple temp = {x, y, 0};
        //printf("%d\n", ypx);
        triple result = willCollide(sphereArray[i], temp);
        if(!(result.x==-1&&result.y==-1&&result.z==-1) && (render[xpx][ypx] == 0.0 || magnitude(result) < render[xpx][ypx]))
        {
          render[xpx][ypx] = magnitude(result);
          temp.x = eye.x + result.x;
          temp.y = eye.y + result.y;
          temp.z = eye.z + result.z;
          double luminosity = calcShading(temp, i);
          color tempColor = {0};
          tempColor.r = sphereArray[i].c.r*(.4 + (.6*luminosity));
          tempColor.g = sphereArray[i].c.g*(.4 + (.6*luminosity));
          tempColor.b = sphereArray[i].c.b*(.4 + (.6*luminosity));
          colorsphereArray[xpx][ypx] = tempColor;
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
         colorsphereArray[x][y].r , colorsphereArray[x][y].g , colorsphereArray[x][y].b ) ;
     }
  }
  close( fout ) ;
  //
  return 0 ;


}
