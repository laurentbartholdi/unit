
#include	<stdio.h>
#include	<string.h>
#include	<math.h>
#include	<setjmp.h>
#include	"unit.h"
#include      "stdlib.h"
#include      "string.h"

int main()
{
	FILE	*src, *dst;
	int	    i;
	UNITY	u;
	char buf[100], *tmp;

	if( (src = fopen("unit.asc", "r")) == NULL )
	{
		printf("unit.asc: open failed\n");
		exit( 255 );
	}

	if( (dst = fopen("unit.dta", "wb")) == NULL )
	{
		printf("unit.dta: create failed\n");
		exit( 255 );
	}

	while( !feof(src) )
	{
		fgets(buf,100,src);
		if ( (tmp = strtok(buf, " 	")) == NULL )
			break;
		strcpy(u.name, tmp);
		if ( (tmp = strtok(NULL, " 	")) == NULL )
			break;
		u.f1 = atof(tmp);
		if ( (tmp = strtok(NULL, " 	")) == NULL )
			break;
		u.f2 = atof(tmp);
		for ( i = 0; i < 10; i++)
		{
			if ( (tmp = strtok(NULL, " ")) == NULL )
				break;
			u.exp[i] = (short)  atoi(tmp);
		}
		if ( i < 10 )
			break;

		fwrite( &u, sizeof(u), 1, dst );
		printf("%s %g %g %d %d %d %d %d %d %d %d %d %d\n", u.name, u.f1, u.f2,
			u.exp[0],u.exp[1], u.exp[2], u.exp[3], u.exp[4], u.exp[5],
			u.exp[6], u.exp[7], u.exp[8], u.exp[9]);
	}

	fclose( src );
	fclose( dst );
}
