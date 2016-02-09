#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void)
{
	FILE* file = fopen("decodeME.txt", "r");

	char tree[10000];
	
	char decoded_message[sizeof(tree)];

	char current_letter;

	int numbytes;
	int file_len = 0;
	int decoded_len;

	char temp;

	char temp_num_codes[10];
	int num_codes;

	int i = 0;

	int j;

	for(i = 0; i<sizeof(tree); i++)
	{
		tree[i] = '*';
		decoded_message[i] = '*';
	}

	i = 0;	
	while(temp != '\n')
	{
		numbytes = fread(&temp, sizeof(char), 1, file);
		temp_num_codes[i] = temp;
		i++;
	}
	num_codes = atoi(temp_num_codes);

	for(i = 0; i<num_codes; i++)
	{
		numbytes = fread(&current_letter, sizeof(char), 1, file);
		j = 1;
		do
		{
			numbytes = fread(&temp, sizeof(char), 1, file);
			if(numbytes == 0)
				break;
			if(temp=='0')
			{
				j *= 2;
			}
			else if(temp=='1')
			{
				j = j*2 +1;
			}
			else if(temp != '\n')
			{
				printf("WTF\n");
				return 1;
			}
		} 
		while(temp != '\n');
		tree[j] = current_letter;
	}
	
	i = 0;
	j = 1;
	while(1)
	{
		numbytes = fread(&temp, sizeof(char), 1, file);
		file_len++;
		if(numbytes == 0)
		{
			break;
		}
		if(temp == '0')
		{
			j *= 2;
		}
		else if(temp == '1')
		{
			j = j*2 + 1;
		}
		if(tree[j] != '*')
		{
			decoded_message[i] = tree[j];
			j = 1;
			i++;
		}
	}
	i = 0;
	decoded_len = 0;
	while(decoded_message[i] != '*')
	{
		printf("%c", decoded_message[i]);
		i++;
		decoded_len++;
	}
	printf("\n");
	printf("Encoded size: %d\n", file_len);
	printf("Decoded size: %d\n", decoded_len*8);
	double temp_file_len = file_len;
	double comp_ratio = 8*decoded_len - file_len;
	comp_ratio /= 8*decoded_len;
	printf("Compression ratio (encoded/decoded): %f\n", comp_ratio);
	
	// SHANNON NUMBER
	double shannon_num = 0;
	double temp_decoded_len = decoded_len;
	int char_frequency[256] = {0};
	for(i = 0; i < decoded_len; i++)
	{
		char_frequency[decoded_message[i]]++;
	}
	double prob;
	for(i = 0; i < 256; i++)
	{
		if(char_frequency[i] != 0)
		{
			prob = char_frequency[i]/temp_decoded_len;
			shannon_num += -1*char_frequency[i]*log(prob)/log(2);
		}
	}
	printf("Shannon number: %f\n", shannon_num);
	return 0;
}
