#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h"
#define rows 100
#define cols 100
//int rows = 100;
//int cols = 100;

typedef struct Node
{
	int row;
	int col;
	struct Node* next;
	
}ListNode;

//global vars
int num_prob_steps = 100;
int trials = 500;
double cputime;

double gettime()
{
	double t ;
	struct timeval* ptr = (struct timeval*)malloc( sizeof(struct timeval) ) ;
	gettimeofday( ptr , NULL ) ; // second argument is time zone... NULL
	t = ptr->tv_sec * 1000000.0 + ptr->tv_usec ;
	free( ptr ) ;
	return t / 1000000.0 ;
}

ListNode* calculateBurning(char forest[][cols], ListNode* root)
{
	ListNode* temp = NULL;
	ListNode* current = root;
	int r, c;
	while(current != NULL)
	{
		r = current -> row;
		c = current -> col;
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
		root = current -> next;
		free(current);
		current = root ;
	}
	return temp;
}

int main(int argc, char* argv[])
{
	double start, end;
	start = gettime();
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
	int i, j, k;
	int node = 1;
	if(rank == 0)
	{
		float probability = 0;
		for(i = 0; i < num_prob_steps; i++)
		{
			if(node == size)
				node = 1;
			MPI_Send(&probability, 1, MPI_FLOAT, node, tag, MPI_COMM_WORLD);
			probability += 1.0/num_prob_steps;
			node++;
		}
		float kill = -1.0;
		for(k = 1; k < size; k++)
		{
			MPI_Send(&kill, 1, MPI_FLOAT, k, tag, MPI_COMM_WORLD);
		}
		node = 1;
		float result;
		for(j = 0; j < num_prob_steps; j++)
		{
			if(node == size)
				node = 1;
			MPI_Recv(&result, 1, MPI_FLOAT, node, tag, MPI_COMM_WORLD, &status);
			node++;
			printf("%f\n", result);
		}
		printf("\n");
	}
	else
	{
		
		while(1)
		{
			float probability;
			MPI_Recv(&probability, 1, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &status);
			if(probability < 0)
			{
				break;
			}
			float ticks = 0;
			int t;
			for(t = 0; t < trials; t++)
			{
				char forest[rows][cols];
				int r, c;
				float randomValue;
				srand((int)clock()*rank);
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
				ListNode* root = NULL;
				for(r = 0; r < rows; r++)
				{
					forest[r][0] = 'x';
					ListNode* newFlame = (ListNode*)malloc(sizeof(ListNode));
					newFlame -> row = r;
					newFlame -> col = 0;
					newFlame -> next = root;
					root = newFlame;
				}
				while(root != NULL)
				{
					root = calculateBurning(forest, root);
					ticks++;
				}
			}
			ticks = ticks/trials;
			MPI_Send(&ticks, 1, MPI_FLOAT, 0, tag, MPI_COMM_WORLD);
		}
	}	
	end = gettime();
	double nodetime = end-start;
	MPI_Reduce(&nodetime, &cputime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Finalize();	
	if(rank == 0)
	{
		printf("Runtime: %f\n", nodetime);
		printf("CPU time: %f\n", cputime);
	}
	return 0;
}

