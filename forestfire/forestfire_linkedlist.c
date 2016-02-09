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
int rows = 25;
int cols = 25;
ListNode* root = NULL;

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
	for(r = 0; r < rows; r++)
	{
		for(c = 0; c < cols; c++)
		{
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
		ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
		temp -> row = r;
		temp -> col = 0;
		temp -> next = root;
		root = temp;
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

