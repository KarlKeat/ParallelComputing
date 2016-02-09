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

int n, r, c;
ListNode* root = NULL;

int hasFire()
{
	if(root == NULL)
		return 0;
	else
		return 1;
}
void calculateBurning(size_t s, char forest[s][s])
{
	ListNode* temp = NULL;
	ListNode* current = root;
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
			if(r+1 < n && forest[r+1][c] == '*')
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
			if(c+1 < n && forest[r][c+1] == '*')
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
int main(int argc, char* argv[])
{
	
	srand(time(NULL));
	int nmax = atoi(argv[1]);
	float probability = .7;
	float randomValue;
	int xPeaks[nmax] = {0};
	float yPeaks[nmax] = {0};
	int trial;
	float prob;

	for(n = 1; n <= nmax; n++)
	{
		float[100] probData = {0};
		int i = 0;
		for(prob = 0; prob <= 1; prob += .01)
		{	
			int ticksData = 0;
			for(trial = 0; trial < 1000; trial++)
			{
				char trees[n][n];
				int ticks = 0;
		
				for(r = 0; r < n; r++)
				{
					for(c = 0; c < n; c++)
					{
						randomValue = (float)rand()/(float)RAND_MAX;
						//printf("%f\n", randomValue);
						if(randomValue <= probability)
						{
							trees[r][c] = '*';
						}
						else
						{
							trees[r][c] = '-';
						}
					}
				}
				for(r = 0; r < n; r++)
				{
					trees[r][0] = 'x';
					ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
					temp -> row = r;
					temp -> col = 0;
					temp -> next = root;
					root = temp;
				}
				while(hasFire())
				{
					calculateBurning(n, trees);
					ticks++;
				}	
				ticksData += ticks;
			}
			probData[i] = (float)ticksData/1000.0;
			i++;
		}
		int xMax = 0;
		float yMax = 0;
		int x;
		for(x = 0; x < 100; x++)
		{
			if(probData[x] > xMax)
			{
				xMax = x;
				yMax = probData[x];
			}
		}
		xPeaks[n] = xMax;
		yPeaks[n] = yMax;
	}		
}

