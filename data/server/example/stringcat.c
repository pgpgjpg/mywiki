#include <stdio.h>
#include <string.h>
#define MAX_LENGTH		1026	

void outputLine(char* s);
void outputLine(char *string)
{
	puts(string);
}

int main(int argc, char** argv)
{
	char *pArrays[] = {
		 "one"
		,"two"
		,"three"
		,"four"
		,"five"
		,"six"
	};
	int size = sizeof(pArrays)/sizeof(pArrays[0]);
	int index, length = 0;
	char line[MAX_LENGTH] = "";

	for (index=0;index<size;index++) {
		length += strlen(pArrays[index]) + 2;

		if (length >=  MAX_LENGTH) {
			break;
		}

		if ('\0' == line[0]) {
			strcpy(line, pArrays[index]);
		} else {
			strcat(line, ", ");
			strcat(line, pArrays[index]);
		}
	}

    outputLine(line);
}

