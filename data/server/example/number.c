#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int main(int argc, char** argv)
{
   char   *string, *stopstring;
   double x;
   long   l;
   int    base;
   unsigned long ul;
   double d1 = 3.1415926;
   double d2 = 3.1415927;
   
   	if (fabs(d1 - d2) <= 0.00001) {
   	
	}

   
   string = "3.1415926This stopped it";
   x = strtod( string, &stopstring );
   printf("string = %s\n", string );
   printf("   strtod = %f\n", x );
   printf("   Stopped scan at: %s\n\n", stopstring );
   
   
   string = "-10110134932This stopped it";
   l = strtol( string, &stopstring, 10 );
   printf("string = %s\n", string );
   printf("   strtol = %ld\n", l );
   printf("   Stopped scan at: %s\n\n", stopstring );


   string = "10110134932";
   printf( "string = %s\n", string );
   /* base 2, 4, and 8로 해서 문자열을 숫자로 변경한다: */
   for( base = 2; base <= 8; base *= 2 ) {
      /* 문자열을 숫자로 변경한다. */
      ul = strtoul( string, &stopstring, base );
      printf( "   strtol = %ld (base %d)\n", ul, base );
      printf( "   Stopped scan at: %s\n", stopstring );
   }
   
    return 0;    
    
}
