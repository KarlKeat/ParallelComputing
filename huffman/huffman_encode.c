#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node
{
	char symbol;
	int frequency;
	struct Node* left;
	struct Node* right;
}TreeNode;

void getCode(char** scheme, TreeNode* root, char* current)
{
	//printf("%s\n",current);
	if(root == NULL)
		return;
	else if(root -> left == NULL && root -> right == NULL)
	{
		sprintf(scheme[root -> symbol], "%s", current);
		//strcpy(scheme[root -> symbol], current);
		return;
	}
	char currLeft[100];
	char currRight[100];
	sprintf(currLeft, "%s%c", current, '0');
	sprintf(currRight, "%s%c", current, '1');
	getCode(scheme, root -> left, currLeft);
	getCode(scheme, root -> right, currRight);
}
int main(void)
{
	int i;
	TreeNode* treenodes[256];
	for(i = 0; i<256; i++)
	{
		TreeNode* tempTree = (TreeNode*)malloc(sizeof(TreeNode));
		tempTree -> frequency = 0;
		treenodes[i] = tempTree;
	}
	FILE* file = fopen("mobydick.txt", "r");	
	FILE* outfile = fopen("encodeOUT.txt", "w");
	int numbytes = 1;
	char temp;
	while(numbytes != 0)
	{
		numbytes = fread(&temp, sizeof(char), 1, file);

		if(treenodes[temp] -> frequency == 0)
		{	
			treenodes[temp] -> symbol = temp;
			treenodes[temp] -> frequency = 1;
			treenodes[temp] -> left = NULL;
			treenodes[temp] -> right = NULL;
		}
		else
		{
			treenodes[temp] -> frequency++;
		}
	}
	//Pick two smallest TreeNodes
	//Parent node = sum of two smallest frequencies
	fclose(file);
	int num_nodes = 256;
	for(i = 0; i < 256; i++)
	{
		if(treenodes[i] -> frequency == 0)
		{
			treenodes[i] = NULL;
			num_nodes--;
		}
	}
	int min_indx1 = -1;
	int min_indx2 = -1;
	while(num_nodes > 1)
	{	
		for(i = 0; i < 256; i++)
		{
			if(treenodes[i] != NULL)
			{
				if(min_indx1 == -1)
				{	
					min_indx1 = i;
				}			
				else if(treenodes[i] -> frequency < treenodes[min_indx1] -> frequency)
				{
					min_indx2 = min_indx1;
					min_indx1 = i;
				}
				else if(min_indx2 == -1 || treenodes[i] -> frequency < treenodes[min_indx2] -> frequency)
				{	
					min_indx2 = i;
				}
			}
			// build parent node
			// reset indices
		}
		TreeNode* tmp = treenodes[min_indx1];
		treenodes[min_indx1] = (TreeNode*)malloc(sizeof(TreeNode));
		treenodes[min_indx1] -> symbol = '$';
		treenodes[min_indx1] -> left = tmp;
		treenodes[min_indx1] -> right = treenodes[min_indx2];
		treenodes[min_indx1] -> frequency = treenodes[min_indx2] -> frequency + tmp -> frequency;
		treenodes[min_indx2] = NULL;
		num_nodes--;
		min_indx1 = -1;
		min_indx2 = -1;
	}
	//Set find root of tree
	TreeNode* root;
	for(i = 0; i < 256; i++)
	{
		if(treenodes[i] != NULL)
		{
			root = treenodes[i];
			break;
		}
	}

	
	char* keys[256];
	char* values[256];
	for(i = 0; i < 256; i++)
	{
		keys[i] = NULL;
		values[i] = NULL;
	}
	char* scheme[256];
	for(i = 0; i<256; i++)
	{
		scheme[i] = (char*)malloc(100);
		memset(scheme[i], 'x', 100);
	}
	getCode(scheme, root, "");
	for(i = 0; i<256; i++)
	{
		if(scheme[i][0] != 'x')
		{
			fprintf(outfile, "%c%s\n", i, scheme[i]);
		}
	}
	file = fopen("mobydick.txt", "r");
	numbytes = 1;
	while(numbytes != 0)
	{
		numbytes = fread(&temp, sizeof(char), 1, file);
		if(numbytes == 0)
			break;
		fprintf(outfile, "%s", scheme[temp]);
	}
	fprintf(outfile, "\n");
	fprintf(stderr, "Output printed to encodeOUT.txt\n");
}

