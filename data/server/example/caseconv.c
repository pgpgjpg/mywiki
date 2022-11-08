#include <stdio.h>
#include <ctype.h>

int main(int argc, char** argv)
{
	int ch;
	
	while (EOF != (ch = getchar())) { 
		if (isupper(ch)) {
			putchar(tolower(ch));
		} else if (islower(ch)) {
			putchar(toupper(ch));
		} else {
			putchar(ch);
		}
	}

	return 0;
}

