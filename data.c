/* DATA.C -- translates a binary data file to an ascii one */

#include	<setjmp.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	"unit.h"

int main()
{
	FILE	*src, *dst;
	int	success = 1;
	UNITY	u;

	if( (src = fopen("unit.dta", "rb")) == NULL )
	{
		printf("unit.dta: open failed\n");
		exit( 255 );
	}

	if( (dst = fopen("unit.asc", "wt")) == NULL )
	{
		printf("unit.asc: create failed\n");
		exit( 255 );
	}

	while( success )
	{
		if( fread( &u, sizeof(u), 1, src ) != 1 )
			success = 0;
		else    fprintf( dst, 
                        "%10s\t%.12g\t%.12g\t%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d\n",
			u.name, u.f1, u.f2, u.exp[0], u.exp[1], u.exp[2],
			u.exp[3], u.exp[4], u.exp[5], u.exp[6], u.exp[7],
			u.exp[8], u.exp[9] );
	}

	fclose( src );
	fclose( dst );
}
