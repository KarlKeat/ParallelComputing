#include <complex.h>
#include <math.h>
#include <stdio.h>

int threshold = 255;

int main()
{
	float x, y;
	for(y = -4; y <= 4; y += .05)
	{
		for(x = -4; x <= 4; x += .05)
		{
			double complex z = x + y*I;
			int result = mandelbrot(0, z, 0);
			if(result != threshold)
			{
				printf("*");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
	return 0;
}
int mandelbrot(double complex z, double complex c,  int n)
{
	if(valueExceeds(z) || n>=threshold)
		return n;
	double complex z2 = cpow(z, 2) + c;
	return mandelbrot(z2, c, n+1);
}
int valueExceeds(double complex x)
{	
	if(cabs(x) >= 2)
		return 1;
	return 0;
}
