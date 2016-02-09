#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
int rows = 25;
int cols = 25;

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
			printf("%c ", forest[r][c]);
		}
		printf("\n");
	}
}
int main()
{
	float probability = .7;
	char forest[rows][cols];
	int r, c;
	float randomValue;
	int ticks = 0;
	srand(time(NULL));
	for(r = 0; r < rows; r++)
	{
		for(c = 0; c < cols; c++)
		{
			randomValue = (float)rand()/(float)RAND_MAX;
			//printf("%f\n", randomValue);
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
		for(c = 0; c < cols; c++)
		{
			printf("%c ", forest[r][c]);
		}
		printf("\n");
	}
	while(hasFire(forest))
	{
		printf("\n");
		calculateBurning(forest);
		ticks++;
	}
	printf("%d\n", ticks);
}

