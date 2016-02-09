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
// 
#define WIDTH   600
#define HEIGHT  600

// 
char   t[WIDTH][HEIGHT];
char   game[WIDTH/5][HEIGHT/5];
char   tempGame[WIDTH/5][HEIGHT/5];
// 


void init()
{
  int x, y;
  for(x = 0; x < WIDTH/5; x++)
  {
  	for(y = 0; y < HEIGHT/5; y++)
	{
		tempGame[x][y] = 0;

		int numNeighbors = 0;
		if(x + 1 < WIDTH/5 && game[x+1][y] == 1)
			numNeighbors++;
		if(x + 1 < WIDTH/5 && y + 1 < HEIGHT/5 && game[x+1][y+1] == 1)
			numNeighbors++;
		if(x + 1 < WIDTH/5 && y - 1 >= 0 && game[x+1][y-1] == 1)
			numNeighbors++;
		if(y + 1 < WIDTH/5 && game[x][y+1] == 1)
			numNeighbors++;
		if(y - 1 >= 0 && game[x][y-1] == 1)
			numNeighbors++;
		if(y - 1 >= 0 && x - 1 >= 0 && game[x-1][y-1] == 1)
			numNeighbors++;
		if(x - 1 >= 0 && y + 1 < HEIGHT/5 && game[x-1][y+1] == 1)
			numNeighbors++;
		if(x - 1 >= 0 && game[x-1][y] == 1)
			numNeighbors++;

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
		else //Dead cell
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
	}
  }
  for(x = 0; x < WIDTH/5; x++)
  {
  	for(y = 0; y < HEIGHT/5; y++)
 	{
		game[x][y] = tempGame[x][y];
	}
  }

  for(x = 0; x < WIDTH; x++)
  {
  	for(y = 0; y < HEIGHT; y++)
	{
		t[x][y] = game[x/5][y/5];
	}
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
   usleep(100000);
   init();
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
      	init() ;
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
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(WIDTH,HEIGHT);
   glutInitWindowPosition(100,50);
   glutCreateWindow("");
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   //
   //
   int x, y;
   srand((int)clock());
   for(x = 0; x < WIDTH/5; x++)
   {
   	for(y = 0; y < HEIGHT/5; y++)
	{
		float randomValue = (float)rand()/(float)RAND_MAX;
		if(randomValue < .5)
		{
			game[x][y] = 1;
		}
		else
		{
			game[x][y] = 0;
		}
	}
   }
   init() ;
   //
   glutDisplayFunc(displayfunc);
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(mousefunc);
   glutKeyboardFunc(keyfunc);
   //
   glutMainLoop();
   //
   return 0;
}
// 
// end of file
// 
