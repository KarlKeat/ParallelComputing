#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
int rows = 100;
int cols = 100;

int hasFire(char forest[rows][cols])
{
	int r, c;
	for(r = 0; r < rows; r++)
	{
		for(c = 0; c < cols; c++)
		{
			if(forest[r][c] == 'x')
				return 1;
		}
	}
	return 0;
}
void calculateBurning(char forest[rows][cols])
{
	char temp[rows][cols];
	int r, c;
	for(r = 0; r < rows; r++)
	{
		for(c = 0; c < cols; c++)
		{
			if(forest[r][c] == 'x')
			{
				temp[r][c] = 'N';
				if(r-1 >= 0 && forest[r-1][c] == '*')
					temp[r-1][c] = 'x';
				if(r+1 < rows && forest[r+1][c] == '*')
					temp[r+1][c] = 'x';
				if(c-1 >= 0 && forest[r][c-1] == '*')
					temp[r][c-1] = 'x';
				if(c+1 < cols && forest[r][c+1] == '*')
					temp[r][c+1] = 'x';
				forest[r][c] = '$';
			}
		}
	}
	for(r = 0; r < rows; r++)
	{
		for(c = 0; c < cols; c++)
		{
			if(temp[r][c] == 'x')
			{
				forest[r][c] = 'x';
			}
		}
	}
}
int main()
{
	float probability;
	int i;
	srand(time(NULL));
	for(probability = 0; probability <= 1; probability += .01)
	{
		int ticks[1000];
		for(i = 0; i < 1000; i++)
		{
			char forest[rows][cols];
			int r, c;
			float randomValue;
			int tick = 0;
			for(r = 0; r < rows; r++)
			{
				for(c = 0; c < cols; c++)
				{
					randomValue = (float)rand()/(float)RAND_MAX;
					if(randomValue <= probability)
					{
						forest[r][c] = '*';
					}
					else
					{
						forest[r][c] = '-';
					}
				}
			}
			for(r = 0; r < rows; r++)
			{
				forest[r][0] = 'x';
			}
			while(hasFire(forest))
			{	
				calculateBurning(forest);
				tick++;
			}
			ticks[i] = tick;
		}
		float avg = 0;
		for(i = 0; i < 1000; i++)
			avg += ticks[i];
		avg /= 1000;
		printf("%f\t%f\n", probability, avg);
	}
}

