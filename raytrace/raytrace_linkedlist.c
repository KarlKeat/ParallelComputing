#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define XRES 720
#define YRES 480
#define NUMSPHERES 4
#define FILENAME "wire.txt"

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

typedef struct ListNode
{
	struct sphere* sph;
	struct ListNode* next;
}ListNode;

triple eye = {.50, .50, -1.00};
triple light = {0, 1.25, -.50};
triple nulltrip = {-1, -1, -1};
struct color sphereColor = {128, 0, 128};
struct ListNode* root = NULL;

void readFile(char* filename)
{
  FILE* file = fopen(filename, "r");
  int numbytes = 1;
  char* temp = malloc(36*sizeof(char));
  double sphereValues[4];
  int i = 0;
  struct ListNode* last = NULL;

  numbytes = fread(temp, sizeof(char), 36, file);

  //while(numbytes != 0)
  for(i = 0; i < 10; i++)
  {
    temp[36] = '\0';
    //printf("%s\n", temp);
    sphereValues[0] = strtod(temp, &temp);
    sphereValues[1] = strtod(temp, &temp);
    sphereValues[2] = strtod(temp, &temp);
    sphereValues[3] = strtod(temp, &temp);
    printf("%f\n", sphereValues[2]);
    sphere* tempSphere = (sphere*)malloc(sizeof(struct sphere));
    tempSphere -> x = sphereValues[0];
    tempSphere -> y = sphereValues[1];
    tempSphere -> z = sphereValues[2];
    tempSphere -> r = sphereValues[3];
    tempSphere -> c = sphereColor;

    if(last == NULL)
    {
      root = (ListNode*)malloc(sizeof(struct ListNode));
      root -> sph = tempSphere;
      root -> next = NULL;;
      last = root;
      printf("%s\n", "newhead");
    }
    else
    {
      last -> next = (ListNode*)malloc(sizeof(struct ListNode));
      last = last -> next;
      last -> sph = tempSphere;
      last -> next = NULL;
      //printf("%s\n", "newnode");
    }
    printf("%f\n", root -> sph -> x);
    numbytes = fread(temp, sizeof(char), 36, file);
  }
  close(file);
  //printf("%s\n", "done");
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

double calcShading(triple pt, sphere current)
{
  //printf("%f, %f, %f\n", pt.x, pt.y, pt.z);
  triple center = {0};
  center.x = current.x;
  center.y = current.y;
  center.z = current.z;
  triple centerVector = {0};
  diff(&centerVector, pt, center);
  unitVector(&centerVector);
  pt.x += .000001 * centerVector.x;
  pt.y += .000001 * centerVector.y;
  pt.z += .000001 * centerVector.z;

  triple pointVector = {0};
  diff(&pointVector, light, pt);


  int i;
  ListNode* tempNode = root;
  while(tempNode != NULL)
  {
    triple collision = shadeCollide(current, pt);
    if(!isNull(collision))
      return 0;
    tempNode = tempNode -> next;
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

   //sphereArray[0] = a;
   //sphereArray[1] = b;
   //sphereArray[2] = c;
   //sphereArray[3] = d;
}

void printSphere(sphere* s)
{
  printf("x:\t%f\ny:\t%f\nz:\t%f\nrad:\t%f\nr:\t%d\ng:\t%d\nb:\t%d\n\n", s->x, s->y, s->z, s->r, s->c.r, s->c.g, s->c.b);
}

void printList()
{
  ListNode* temp = root;
  while(temp != NULL)
  {
    puts("1");
    printSphere(temp -> sph);
    temp = temp -> next;
  }
}

int main(int argc, int* argv)
{
  //init();
  readFile(FILENAME);
  printList();
  printf("%f\n", root -> sph->x);
  printf("%f\n", root -> next -> sph->x);
  double render[XRES][YRES] = {0};
  color colorArray[XRES][YRES] = {0};
  color blank = {0, 0, 0};

  int i;
  struct ListNode* tempNode = root;
  while(tempNode != NULL)
  {
    int xpx = 0;
    int ypx = 0;
    double x, y;
    sphere* current = tempNode -> sph;
    printf("%f\n", current->x);
    for(y = 0; y < 1.0; y+=1.0/YRES)
    {
      for(x = 0; x < 1.5; x+=1.5/XRES)
      {
        triple temp = {x, y, 0};
        triple result = willCollide(*current, temp);
        if(!isNull(result) && (render[xpx][ypx] == 0.0 || magnitude(result) < render[xpx][ypx]))
        {
          render[xpx][ypx] = magnitude(result);
          double luminosity = calcShading(result, *current);
          color tempColor = {0};
          tempColor.r = current->c.r*(.4 + (.6*luminosity));
          tempColor.g = current->c.g*(.4 + (.6*luminosity));
          tempColor.b = current->c.b*(.4 + (.6*luminosity));
          colorArray[xpx][ypx] = tempColor;
        }
        xpx++;
      }
      xpx = 0;
      ypx++;
    }
    tempNode = tempNode -> next;
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
