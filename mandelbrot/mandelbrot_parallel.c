// 
// to compile type... make 
// uses file named... Makefile
// 
// a.out: fireGL.c
//  gcc -lGL -lGLU -lglut fireGL.c
// 
// tab character '\t' before gcc
// 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>
#include <math.h>
#include <GL/glut.h>
#include "mpi.h"
// 
int width  = 600;
int height = 600;
// 
int tempwidth = 600;
int tempheight = 600;

double **t;

// 
int threshold = 128;
int thresholdCounter = 1;
double centerX = 0;
double centerY = 0;
double scaleFactor = 2;


// MPI vars
int rank;
int size;
MPI_Status status;
int tag = 0;
// End MPI vars



int mandelbrot(double complex z, double complex c,  int n)
{
  if(cabs(z) >= 2 || n>=threshold)
          return n;
  double complex z2 = cpow(z, 2) + c;
  return mandelbrot(z2, c, n+1);
}

void init()
{
  
  int x, y;
  int nodeNum = 1;
  for(x = 0; x < width; x++)
  {
    MPI_Send(&width, 1, MPI_INT, nodeNum, tag, MPI_COMM_WORLD);
    MPI_Send(&height, 1, MPI_INT, nodeNum, tag, MPI_COMM_WORLD);
    MPI_Send(&x, 1, MPI_INT, nodeNum, tag, MPI_COMM_WORLD);
    MPI_Send(&centerX, 1, MPI_DOUBLE, nodeNum, tag, MPI_COMM_WORLD);
    MPI_Send(&centerY, 1, MPI_DOUBLE, nodeNum, tag, MPI_COMM_WORLD);
    MPI_Send(&scaleFactor, 1, MPI_DOUBLE, nodeNum, tag, MPI_COMM_WORLD);
    MPI_Send(&threshold, 1, MPI_INT, nodeNum, tag, MPI_COMM_WORLD);
    MPI_Send(&thresholdCounter, 1, MPI_INT, nodeNum, tag, MPI_COMM_WORLD);
    nodeNum++;
    if(nodeNum >= size)
      nodeNum = 1;
  }
  for(x = 0; x < width; x++)
  {
    int index;
    double column[height];
    MPI_Recv(column, height, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
    MPI_Recv(&index, 1, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);
    int y;
    for(y = 0; y < height; y++)
    {
      t[index][y] = column[y];
    }
  }
  double max = 0;
  double min = 100;
  for(x = 0; x < width; x++)
  {
    for(y = 0; y < height; y++)
    {
      if(t[x][y] > max && t[x][y] != -1)
        max = t[x][y];
      else if(t[x][y] < min && t[x][y] != -1)
        min = t[x][y];
      else if(t[x][y] == -1)
        t[x][y] = 0;
    }
  }
  for(x = 0; x < width; x++)
  {
    for(y = 0; y < height; y++)
    {
      if(t[x][y] != 0)
        t[x][y] = (t[x][y] - min)/ (max - min); // Normalization
        //t[x][y] /= threshold;
    }
  }
}

void displayfunc()
{
   int x, y;
   //
   glClear(GL_COLOR_BUFFER_BIT); // white
   //
   for( x = 0 ; x < width ; x++ )
   {
      for( y = 0 ; y < height ; y++ )
      { 
        glColor3f(t[x][y], 0, 0);
        glBegin(GL_POINTS);
        glVertex2f(x,y);
        glEnd();
      }
   }
   //
   glutSwapBuffers(); // single buffering..call glFlush();

  if(tempheight != height || tempheight != width)
  {
    width = tempheight;
    height = tempheight;
    glViewport(0,0,(GLsizei)tempheight,(GLsizei)tempheight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,1.0*tempheight,0.0,1.0*tempheight); // always a square
    glMatrixMode(GL_MODELVIEW);
    glutReshapeWindow((GLsizei)tempheight,(GLsizei)tempheight);
    free(t);
    t = (double**)malloc(sizeof(double*)*width); 
    int i;
    for(i = 0; i < width; i++)
    {
      t[i] = (double*)malloc(sizeof(double)*height);
    }
    init();
  }
}
void reshapefunc(int wscr,int hscr)
{
  if(tempheight < tempwidth)
  {
    tempheight = hscr;
    tempwidth = hscr;
  }
  else
  {
    tempheight = wscr;
    tempwidth = wscr;
  }
  glutReshapeWindow((GLsizei)tempheight,(GLsizei)tempheight);
  glViewport(0,0,(GLsizei)tempheight,(GLsizei)tempheight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0,1.0*tempheight,0.0,1.0*tempheight); // always a square
  glMatrixMode(GL_MODELVIEW);
}
void mousefunc(int button,int state,int xscr,int yscr)
{
    if(state==GLUT_DOWN)
    {
      if(button==GLUT_LEFT_BUTTON)
      {
          //printf("%f, %f\n", centerX, centerY);
          //printf("%d, %d\n", xscr, yscr); 
          centerX += (2.5*(xscr-width/2)*scaleFactor)/width;
          centerY += (2.5*(yscr-height/2)*scaleFactor)/height;
          //printf("%f, %f\n", centerX, centerY);
          scaleFactor /= 2;
          init() ;
      }
      else if(button == 3)
      {
        centerX += (2.5*(xscr-width/2)*scaleFactor)/width;
        centerY += (2.5*(yscr-height/2)*scaleFactor)/height;
        scaleFactor /= 2;
        init();
      }
      else if(button == 4)
      {
        if(scaleFactor <= 2)
        {
          centerX += (2.5*(xscr-width/2)*scaleFactor)/width;
          centerY += (2.5*(yscr-height/2)*scaleFactor)/height;
          scaleFactor *= 2;
          init();
        }
      }
    }
}

void keyfunc(unsigned char key,int xscr,int yscr)
{
  if(key == 'q')
  {
    exit(0) ;
  }
}
void specfunc(int key, int x, int y)
{
  if(key == GLUT_KEY_UP)
  {
    threshold *= 2;
    thresholdCounter++; 
    init();
  }
  else if(key == GLUT_KEY_DOWN)
  {
    if(thresholdCounter != 1)
    {
      threshold /= 2;
      thresholdCounter--;
      init();
    }
  }
  printf("%d, %d\n", threshold, thresholdCounter);
}
int main(int argc,char* argv[])
{
  // Boilerplate
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size); // same
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // different

  // End boilerplate  
  if(rank == 0)
  {
    t = (double**)malloc(sizeof(double*)*width); 
    int i;
    for(i = 0; i < width; i++)
    {
      t[i] = (double*)malloc(sizeof(double)*height);
    }
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(100,50);
    glutCreateWindow("");
    glClearColor(1.0,1.0,1.0,0.0);
    glShadeModel(GL_SMOOTH);
    //
    //
    init() ;
    //
    glutDisplayFunc(displayfunc);
    glutReshapeFunc(reshapefunc);
    glutMouseFunc(mousefunc);
    glutKeyboardFunc(keyfunc);
    glutSpecialFunc(specfunc);
    //
    glutMainLoop();
  }
  else
  {
    while(1)
    {
      int index;
      MPI_Recv(&width, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&height, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&index, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&centerX, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&centerY, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&scaleFactor, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&threshold, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&thresholdCounter, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

      int y;
      double column[height];

      for(y = 0; y < height; y++)
      {
        double complex z = ((2.5*(index-width/2)*scaleFactor)/width + centerX) + ((2.5*(y-height/2)*scaleFactor)/height - centerY)*I;
        column[y] = (mandelbrot(0, z, 0));
        if(column[y] == threshold)
          column[y] = -1;
      }
      MPI_Send(column, height, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
      MPI_Send(&index, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }
  }
  //
  MPI_Finalize(); 
  return 0;
}
// 
// end of file
// 
