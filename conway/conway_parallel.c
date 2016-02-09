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
#include <unistd.h>
#include <GL/glut.h>
#include "mpi.h"
// 
#define WIDTH   800
#define HEIGHT  800
#define scalefactor 5
// MPI vars
int rank;
int size;
MPI_Status status;
int tag = 0;
// End MPI vars

//

char t[WIDTH][HEIGHT];
char game[WIDTH/scalefactor][HEIGHT/scalefactor] = {0};
char tempGame[WIDTH/scalefactor][HEIGHT/scalefactor] = {0};
int xMin, xMax;
int pauseval;


void init()
{
  int x, y, i;

  if(rank > 0)
  {
    for(y = 0; y < HEIGHT/scalefactor; y++) //send leftmost values
    {
      int tempY;
      MPI_Recv(&tempY, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
      int tempNeighbors = 0;
      if(game[xMin][tempY] == 1)
        tempNeighbors++;
      if(tempY+1 < HEIGHT/scalefactor && game[xMin][tempY+1] == 1)
        tempNeighbors++;
      if(tempY-1 >= 0 && game[xMin][tempY-1] == 1)
        tempNeighbors++;
      //printf("%d\n", numNeighbors);
      MPI_Send(&tempNeighbors, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD);
    }
  }

  for(x = xMin; x <= xMax; x++)
  {
    for(y = 0; y < HEIGHT/scalefactor; y++)
	  {
      tempGame[x][y] = 0;

  		int numNeighbors = 0;

      if((x < xMax) || (rank == size-1)) //Get rightmost values
      {
        if(x+1 <= xMax && game[x+1][y] == 1)
          numNeighbors++;
        if(x+1 <= xMax && y+1 < HEIGHT/scalefactor && game[x+1][y+1] == 1)
          numNeighbors++;
        if(x+1 <= xMax && y-1 >= 0 && game[x+1][y-1] == 1)
          numNeighbors++;
      }
      else
      {
        int tempNeighbors;
        MPI_Send(&y, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD); //request values
        MPI_Recv(&tempNeighbors, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD, &status); //recieve values
        numNeighbors += tempNeighbors;
      }

      if(y + 1 < HEIGHT/scalefactor && game[x][y+1] == 1)
        numNeighbors++;
      if(y - 1 >= 0 && game[x][y-1] == 1)
        numNeighbors++;

      if(x == xMin && rank > 0)
      {
        int tempNeighbors;
        MPI_Send(&y, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD);
        MPI_Recv(&tempNeighbors, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
        numNeighbors += tempNeighbors;
      }
      else if(x > xMin || rank == 0)
      {
        if(y - 1 >= 0 && game[x-1][y-1] == 1)
          numNeighbors++;
        if(y + 1 < HEIGHT/scalefactor && game[x-1][y+1] == 1)
          numNeighbors++;
        if(game[x-1][y] == 1)
          numNeighbors++;
      }

      
      
  		if(game[x][y] == 1) //Living cell
  		{
  			if(numNeighbors == 2 || numNeighbors == 3)
  			{
  				tempGame[x][y] = 1;
  			}
  			else
  			{
  				tempGame[x][y] = 0;
  			}
  		}
  		else if(game[x][y] == 0)//Dead cell
  		{
  			if(numNeighbors == 3)
  			{
  				tempGame[x][y] = 1;
  			}
  			else
  			{
  				tempGame[x][y] = 0;
  			}
  		}
      else
      {
        tempGame[x][y] = 0;
      }
  	}
  }
  if(rank < size-1)
  {
    for(y = 0; y < HEIGHT/scalefactor; y++)
    {
      int tempY;
      MPI_Recv(&tempY, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD, &status);
      int tempNeighbors = 0;
      if(game[xMax][tempY] == 1)
        tempNeighbors++;
      if(tempY+1 < HEIGHT/scalefactor && game[xMax][tempY+1] == 1)
        tempNeighbors++;
      if(tempY-1 >= 0 && game[xMax][tempY-1] == 1)
        tempNeighbors++;
      //printf("%d\n", tempNeighbors);
      MPI_Send(&tempNeighbors, 1, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
    }
  }
  for(x = xMin; x <= xMax; x++)
  {
    for(y = 0; y < HEIGHT/scalefactor; y++)
    {
      game[x][y] = tempGame[x][y];
    }
  }
  
  if(rank == 0)
  {
    for(i = 1; i < size; i++)
    {
      MPI_Recv(tempGame, (WIDTH/scalefactor)*(HEIGHT/scalefactor), MPI_CHAR, i, tag, MPI_COMM_WORLD, &status);
      for(x = i*((WIDTH/scalefactor)/size); x <= (i+1)*((WIDTH/scalefactor)/size)-1; x++)
      {
        for(y = 0; y < HEIGHT/scalefactor; y++)
        {
          game[x][y] = tempGame[x][y];
        }
      }
    }

    for(x = 0; x < WIDTH; x++)
    {
      for(y = 0; y < HEIGHT; y++)
      {
        t[x][y] = game[x/scalefactor][y/scalefactor];
      }
    }
  }
  else
  {
    MPI_Send(tempGame, (WIDTH/scalefactor)*(HEIGHT/scalefactor), MPI_CHAR, 0, tag, MPI_COMM_WORLD);
  }
}
// 

// 

/*   
   GLUT_BITMAP_8_BY_13
   GLUT_BITMAP_9_BY_15
   GLUT_BITMAP_TIMES_ROMAN_10
   GLUT_BITMAP_TIMES_ROMAN_24
   GLUT_BITMAP_HELVETICA_10
   GLUT_BITMAP_HELVETICA_12
   GLUT_BITMAP_HELVETICA_18
*/

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
   	    glColor3f(t[x][y], t[x][y], t[x][y]);
        glBegin(GL_POINTS);
        glVertex2f(x,y);
        glEnd();
      }
   }
   //
   glutSwapBuffers(); // single buffering..call glFlush();
}

void idlefunc()
{
  if(pauseval == 1)
  {
    return;
  }
  else
  {
    usleep(1000);
    init();
    glutPostRedisplay(); // calls displayfunc
  }
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
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
      	if(pauseval == 0)
          pauseval = 1;
        else
          pauseval = 0;
      }
   }
   if(button==GLUT_RIGHT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
        printf("%d, %d\n", xscr/scalefactor, yscr/scalefactor);
      }
   }
}

void keyfunc(unsigned char key,int xscr,int yscr)
{
    if( key ==' ' )
    {
      init();
    }
    else if( key == 'q' )
    {
      exit( 0 ) ;
    }
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
    pauseval = 0;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutInitWindowPosition(100,50);
    glutCreateWindow("");
    glClearColor(1.0,1.0,1.0,0.0);
    glShadeModel(GL_SMOOTH);
  }
  //
  //
  int x, y;
  srand((int)clock());

  xMin = rank*((WIDTH/scalefactor)/size); //inclusive
  xMax = (rank+1)*((WIDTH/scalefactor)/size) - 1; //non-inclusive

  for(x = xMin; x <= xMax; x++)
  {
    for(y = 0; y < HEIGHT/scalefactor; y++)
    {
      float randomValue = (float)rand()/(float)RAND_MAX;
      if(randomValue < .5)
      {
        game[x][y] = 0;
        //game[x][y] = 1;
      }
      else
      {
        game[x][y] = 0;
      }
    }
  }
  game[10][100] = 1;
  game[10][101] = 1;
  game[9][102] = 1;
  game[9][100] = 1;
  game[8][100] = 1;
  init() ;
  //
  if(rank == 0)
  {
    glutIdleFunc(idlefunc);
    glutDisplayFunc(displayfunc);
    glutReshapeFunc(reshapefunc);
    glutMouseFunc(mousefunc);
    glutKeyboardFunc(keyfunc);
    //
    glutMainLoop();
  }
  else
  {
    while(1)
    {
      init();
    }
  }

  return 0;
}
// 
// end of file
// 
