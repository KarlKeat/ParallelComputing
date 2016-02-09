#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
typedef struct Node
{
	int row;
	int col;
	struct Node* next;
	
}ListNode;

int rows = 100;
int cols = 100;
ListNode* root = NULL;
double start, end;

double gettime()
{
	double t ;
	//
	struct timeval* ptr = (struct timeval*)malloc( sizeof(struct timeval) ) ;
	//
	gettimeofday( ptr , NULL ) ; // second argument is time zone... NULL
	//
	t = ptr->tv_sec * 1000000.0 + ptr->tv_usec ;
	//
	free( ptr ) ;
	//
	return t / 1000000.0 ;
}

int hasFire(char forest[rows][cols])
{
	if(root == NULL)
		return 0;
	else
		return 1;
}
void calculateBurning(char forest[rows][cols])
{
	ListNode* temp = NULL;
	ListNode* current = root;
	int r, c;
	while(current != NULL)
	{
		r = current -> row;
		c = current -> col;
		if(forest[r][c] == 'x')
		{
			if(r-1 >= 0 && forest[r-1][c] == '*')
			{	
				forest[r-1][c] = 'x';
				ListNode* head = (ListNode*)malloc(sizeof(ListNode));
				head -> row = r-1;
				head -> col = c;
				head -> next = temp;
				temp = head;
			}
			if(r+1 < rows && forest[r+1][c] == '*')
			{
				forest[r+1][c] = 'x';
				ListNode* head = (ListNode*)malloc(sizeof(ListNode));
				head -> row = r+1;
				head -> col = c;
				head -> next = temp;
				temp = head;
			}
			if(c-1 >= 0 && forest[r][c-1] == '*')
			{
				forest[r][c-1] = 'x';
				ListNode* head = (ListNode*)malloc(sizeof(ListNode));
				head -> row = r;
				head -> col = c-1;
				head -> next = temp;
				temp = head;
			}
			if(c+1 < cols && forest[r][c+1] == '*')
			{	
				forest[r][c+1] = 'x';
				ListNode* head = (ListNode*)malloc(sizeof(ListNode));
				head -> row = r;
				head -> col = c+1;
				head -> next = temp;
				temp = head;
			}
			forest[r][c] = '$';
		}
		current = current -> next;
	}
	root = temp;
}
int main()
{
	start = gettime();
	int trials = 500;
	float probability;
	char forest[rows][cols];
	int r, c;
	float randomValue;
	float ticks = 0;
	srand(time(NULL));
	for(probability = 0; probability < 1; probability += .01)
	{
		int t;
		for(t = 0; t < trials; t++)
		{
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
				ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
				temp -> row = r;
				temp -> col = 0;
				temp -> next = root;
				root = temp;
			}
			while(hasFire(forest))
			{
				calculateBurning(forest);
				ticks++;
			}
		}
		ticks /= trials;
		printf("%f\n", ticks);
		ticks = 0;
	}
	end = gettime();
	printf("\nRuntime: %f\n", end-start);
}


