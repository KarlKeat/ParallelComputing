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

void init()
{
  int x, y;
      for(y = 0; y < HEIGHT; y++)
  {   
      for(x = 0; x < WIDTH; x++)
    {
      double complex z = ((2.5*(x-WIDTH/2)*scaleFactor)/WIDTH + centerX) + ((2.5*(y-HEIGHT/2)*scaleFactor)/HEIGHT - centerY)*I;
      //printf("%f, %f\n", ((x-400)*scaleFactor - centerX)/WIDTH, ((y-400)*scaleFactor - centerY)/HEIGHT);
      t[x][y] = (char)(mandelbrot(0, z, 0)*2/thresholdCounter);
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
   init() ;
      }
   }
}

void keyfunc(unsigned char key,int xscr,int yscr)
{
   if( key ==' ' )
   {
    threshold *= 2;
  thresholdCounter++; 
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
