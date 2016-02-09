// 
// to compile type... make 
// uses file named... Makefile
// 
// a.out: fireGL.c
// 	gcc -lGL -lGLU -lglut fireGL.c
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
#include <string.h>
// 
#define WIDTH   400
#define HEIGHT  400
// 
char   t[WIDTH][HEIGHT]     ; 
// 
int threshold = 127;
int thresholdCounter = 1;
double centerX = 0;
double centerY = 0;
double scaleFactor = 2;
//
int mandelbrot(double complex z, double complex c,  int n)
{
        if(valueExceeds(z) || n>=threshold)
                return n-1;
        double complex z2 = cpow(z, 2) + c;
        return mandelbrot(z2, c, n+1);
}
int valueExceeds(double complex x)
{
        if(cabs(x) >= 2)
                return 1;
        return 0;
}

void init(int rank, int size, MPI_Status status, int tag)
{
  printf("%d\n", rank);
  if(rank == 0)
  {
    int recipient = 1;
    int colNum;
    for(colNum = 0; colNum < WIDTH; colNum++)
    {
      MPI_Send(&colNum, 1, MPI_INT, recipient, tag, MPI_COMM_WORLD);
      recipient++;
      if(recipient == size)
      {
        recipient = 1;
      }
    }
    colNum = -1;
    for(recipient = 1; recipient < size; recipient++)
    {
      MPI_Send(&colNum, 1, MPI_INT, recipient, tag, MPI_COMM_WORLD);
    }
    for(colNum = 0; colNum < WIDTH; colNum++)
    {
      int result[HEIGHT];
      int colTemp;
      MPI_Recv(&result, HEIGHT, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&colTemp, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
      int y;
      for(y = 0; y < HEIGHT; y++)
      	t[colTemp][y] = result[y];
    }
  }
  else
  {
    while(1)
    {
      int index;
      MPI_Recv(&index, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

      if(index == -1)
        break;

      MPI_Recv(&centerX, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&centerY, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&scaleFactor, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&threshold, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&thresholdCounter, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&centerX, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
      
      int myCol[HEIGHT];
      memcpy(myCol, compute(index), HEIGHT);
      MPI_Send(&myCol, HEIGHT, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }
  }

	return;
}
int compute(int col)
{
  int x;
  int column[HEIGHT];
  for(x = 0; x < WIDTH; x++)
  {
    double complex z = ((2.5*(x-WIDTH/2)*scaleFactor)/WIDTH + centerX) + ((2.5*(col-HEIGHT/2)*scaleFactor)/HEIGHT - centerY)*I;
    column[x] = (char)(mandelbrot(0, z, 0)*2/thresholdCounter);
  }
  return column;
}


void displayfunc()
{
   int    x  , y  ;
   //
   glClear(GL_COLOR_BUFFER_BIT); // white
   //
   for( x = 0 ; x < WIDTH ; x++ )
   {
      for( y = 0 ; y < HEIGHT ; y++ )
      { 
	 glColor3b(t[x][y], 0, 0);
         glBegin(GL_POINTS);
         glVertex2f(x,y);
         glEnd();
      }
   }
   //
   glutSwapBuffers(); // single buffering..call glFlush();
}
void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)WIDTH,(GLsizei)HEIGHT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*WIDTH,0.0,1.0*HEIGHT); // always a square
   glMatrixMode(GL_MODELVIEW);
   glutReshapeWindow((GLsizei)WIDTH,(GLsizei)HEIGHT);
}

int main(int argc,char* argv[])
{  
  // MPI vars
  int rank;
  int size;
  MPI_Status status;
  int tag = 0;
  // End MPI vars
  // Boilerplate
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size); // same
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // different
  // End boilerplate
  // function vars
  //
void keyfunc(unsigned char key,int xscr,int yscr)
{
  if( key ==' ' )
  {
    threshold *= 2;
    thresholdCounter++;	
    init(rank, size, status, tag);
  }
  else if( key == 'q' )
  {
    exit(0);
  }
}
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
      	printf("%f, %f\n", centerX, centerY);
      	printf("%d, %d\n", xscr, yscr); 
      	centerX += (2.5*(xscr-WIDTH/2)*scaleFactor)/WIDTH;
      	centerY += (2.5*(yscr-HEIGHT/2)*scaleFactor)/HEIGHT;
      	printf("%f, %f\n", centerX, centerY);
      	scaleFactor /= 2;
      	init(rank, size, status, tag) ;
      }
   }
}


  if(rank == 0)
  {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutInitWindowPosition(100,50);
    glutCreateWindow("");
    glClearColor(1.0,1.0,1.0,0.0);
    glShadeModel(GL_SMOOTH);
    
    init(rank, size, status, tag);
    
    glutDisplayFunc(displayfunc);
    glutReshapeFunc(reshapefunc);
    glutMouseFunc(mousefunc);
    glutKeyboardFunc(keyfunc);
   
    glutMainLoop();
  }
  MPI_Finalize();
  return 0;
}
// 
// end of file
// 
