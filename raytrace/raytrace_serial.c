#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

#define N 8

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

typedef struct box
{
  struct triple corner1;
  struct triple corner2;
}box;

const triple eye = {1.00, .50, -1.00};
const triple light = {0, 1.25, -.50};
const triple nulltrip = {-1, -1, -1};
const struct color sphereColor = {128, 0, 128};
int numspheres = 0;
char* filepath;
struct box boundingbox = {0};
sphere* sphereArray;

void countLines(char* filename)
{
  FILE* file = fopen(filename, "r");
  numspheres = 0;
  char temp = getc(file);
  while(temp != EOF)
  {
    if(temp == '\n')
    {
      numspheres++;
    }
    temp = getc(file);
  }
  printf("%s has %d lines.\n", filename, numspheres);
  close(file);
}

void readFile(char* filename)
{
  FILE* file = fopen(filename, "r");
  int i = 0;
  char* temp = malloc(36*sizeof(char));
  for(i = 0; i < numspheres; i++)
  {
    temp[35] = '\0';
    fscanf(file, "%lf", &sphereArray[i].x); //sphereArray[i].x =strtod(temp, &temp);
    fscanf(file, "%lf", &sphereArray[i].y);//sphereArray[i].y = strtod(temp, &temp);
    fscanf(file, "%lf", &sphereArray[i].z);//sphereArray[i].z = strtod(temp, &temp);
    fscanf(file, "%lf", &sphereArray[i].r);//sphereArray[i].r = strtod(temp, &temp);
    sphereArray[i].c = sphereColor;
    //printf("%d\n", i);
  }

  close(file);
  printf("Finished reading %s.\n", filename);
}

void makeBoundingBox(void)
{
  double xMin, yMin, zMin;
  xMin = yMin = zMin = 1000000;
  double xMax, yMax, zMax;
  xMax = yMax = zMax = -1000000;
  int i;
  for(i = 0; i < numspheres + 4; i++)
  {
    double sphX = sphereArray[i].x + sphereArray[i].r;
    if(sphX > xMax)
      xMax = sphX;
    sphX = sphereArray[i].x - sphereArray[i].r;
    if(sphX < xMin)
      xMin = sphX;

    double sphY = sphereArray[i].y + sphereArray[i].r;
    if(sphY > yMax)
      yMax = sphY;
    sphY = sphereArray[i].y - sphereArray[i].r;
    if(sphY < yMin)
      yMin = sphY;

    double sphZ = sphereArray[i].z + sphereArray[i].r;
    if(sphZ > zMax)
      zMax = sphZ;
    sphZ = sphereArray[i].z - sphereArray[i].r;
    if(sphX < xMin)
      zMin = sphZ;
  }
  boundingbox.corner1.x = xMax;
  boundingbox.corner1.y = yMax;
  boundingbox.corner1.z = zMax;
  boundingbox.corner2.x = xMin;
  boundingbox.corner2.y = yMin;
  boundingbox.corner2.z = zMin;
  printf("<%f,%f,%f>, <%f,%f,%f>\n", xMax, yMax, zMax, xMin, yMin, zMin);
}

int isNull(triple t)
{
  if(t.x != -1 || t.y != -1 || t.z != -1)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

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

  double b = 2*(vector.x*(eye.x-sph.x)+vector.y*(eye.y-sph.y)+vector.z*(eye.z-sph.z));
  double c = pow(eye.x-sph.x,2)+pow(eye.y-sph.y,2)+pow(eye.z-sph.z,2)-pow(sph.r,2);

  double discriminant = pow(b,2) - 4*c;

  if(discriminant >= 0)
  {
    double r1 = (-b - sqrt(pow(b,2)-4*c))/2.0;
    double r2 = (-b + sqrt(pow(b,2)-4*c))/2.0;
    triple ray = {0};
    if(r1 < r2 && r1 > 0)
    {
      ray.x = eye.x + r1*vector.x;
      ray.y = eye.y + r1*vector.y;
      ray.z = eye.z + r1*vector.z;
      return ray;
    }
    else if(r2 > 0)
    {
      ray.x = eye.x + r2*vector.x;
      ray.y = eye.y + r2*vector.y;
      ray.z = eye.z + r2*vector.z;
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

int boxCollide(sphere sph, triple pix)
{
  triple vector = {0};
  diff(&vector, light, pix);
  unitVector(&vector);

  double tFar = -1000000;
  double tNear = 1000000;


  return 0;
}


double calcShading(triple pt, int current)
{
  //printf("%f, %f, %f\n", pt.x, pt.y, pt.z);
  triple center = {0};
  center.x = sphereArray[current].x;
  center.y = sphereArray[current].y;
  center.z = sphereArray[current].z;
  triple centerVector = {0};
  diff(&centerVector, pt, center);
  unitVector(&centerVector);
  pt.x += .000001 * centerVector.x;
  pt.y += .000001 * centerVector.y;
  pt.z += .000001 * centerVector.z;

  triple pointVector = {0};
  diff(&pointVector, light, pt);

  int i;
  for(i = 0; i < numspheres + 4; i++)
  {
    triple collision = shadeCollide(sphereArray[i], pt);
    if(!isNull(collision))
    {
      return 0;
    }
  }
  unitVector(&pointVector);
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
   a.c.r =      0  ; // color is Peru
   a.c.g =    191  ;
   a.c.b =    255  ;

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

   sphereArray[numspheres + 0] = a;
   sphereArray[numspheres + 1] = b;
   sphereArray[numspheres + 2] = c;
   sphereArray[numspheres + 3] = d;
}

void printSphere(sphere s)
{
  printf("x:\t%f\ny:\t%f\nz:\t%f\nrad:\t%f\nr:\t%d\ng:\t%d\nb:\t%d\n\n", s.x, s.y, s.z, s.r, s.c.r, s.c.g, s.c.b);
}

int main(int argc, char* argv[])
{
  int tid, nthreads;

  numspheres = 0;
  if(argc == 2)
  {
    filepath = argv[1];
    countLines(filepath);
    sphereArray = calloc(numspheres + 4, sizeof(sphere));
    readFile(filepath);
  }
  else
    sphereArray = calloc(numspheres + 4, sizeof(sphere));
  init();
  //makeBoundingBox();
  double render[XRES][YRES] = {0};
  color colorArray[XRES][YRES] = {0};
  color blank = {0, 0, 0};

  int i;

  //omp_set_num_threads(N);
  #pragma omp parallel for private(tid)
  for(i = 0; i < numspheres + 4; i++)
  {
    int xpx,ypx;
    double y = 0;
    double x = 0;
    //#pragma omp parallel for private(tid, x, y, xpx, ypx)
    for(ypx = 0; ypx < YRES; ypx++)
    {
      for(xpx = 0; xpx < XRES; xpx++)
      {
        triple temp = {x, y, 0};
        triple result = willCollide(sphereArray[i], temp);
        if(!isNull(result) && (render[xpx][ypx] == 0.0 || magnitude(result) < render[xpx][ypx]))
        {
          render[xpx][ypx] = magnitude(result);
          double luminosity = calcShading(result, i);
          color reflectance = sphereArray[i].c;//calcReflectance(result, i);
          color tempColor = {0};
          if(i == numspheres)
          {
            if(((int)floor(result.x*5)%2==0&&(int)floor(result.z*5)%2!=0)||((int)floor(result.x*5)%2!=0&&(int)floor(result.z*5)%2==0))
            {
              tempColor.r = (.6*sphereArray[i].c.r+.4*reflectance.r)*(.4 + (.6*luminosity));
              tempColor.g = (.6*sphereArray[i].c.g+.4*reflectance.g)*(.4 + (.6*luminosity));
              tempColor.b = (.6*sphereArray[i].c.b+.4*reflectance.b)*(.4 + (.6*luminosity));
            }
            else
            {
              tempColor.r = (255-(.6*sphereArray[i].c.r+.4*reflectance.r))*(.4 + (.6*luminosity));
              tempColor.g = (255-(.6*sphereArray[i].c.g+.4*reflectance.g))*(.4 + (.6*luminosity));
              tempColor.b = (255-(.6*sphereArray[i].c.b+.4*reflectance.b))*(.4 + (.6*luminosity));
            }
          }
          else
          {
            tempColor.r = (.6*sphereArray[i].c.r+.4*reflectance.r)*(.4 + (.6*luminosity));
            tempColor.g = (.6*sphereArray[i].c.g+.4*reflectance.g)*(.4 + (.6*luminosity));
            tempColor.b = (.6*sphereArray[i].c.b+.4*reflectance.b)*(.4 + (.6*luminosity));
          }
          colorArray[xpx][ypx] = tempColor;
        }
        x+=1.5/XRES;
      }
      x=0;
      y+=1.0/YRES;
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
         colorArray[x][y].r , colorArray[x][y].g , colorArray[x][y].b ) ;
     }
  }
  close( fout ) ;
  //
  return 0 ;


}
