/*	UNIT: a C program for converting units
 *********************************************
 *	written by Larry Bartholdi, (C) 1990, 2020
 *	current version 1.06, 18.6.90
 *********************************************
 *	COMPILE FLAGS:
 *	DEFINFIX/DEFPREFIX: default computation mode
 *	SCINOT: scientific notation ( else Engineer )
 *	HIPREC: display precision ( default 3; w/ flag 16 )
 *	CASE: distinguish upper and lower cases
 */

int	stricmp( char *a, char *b )
{
	char	na, nb;

	while( *a )
	{
		if( *a >= 'a' && *a <= 'z')
			na = *a - 'a' + 'A';
		else	na = *a;
		if( *b >= 'a' && *b <= 'z')
			nb = *b - 'a' + 'A';
		else	nb = *b;
		if( na < nb ) 	return( -1 );
		if( na > nb ) 	return(  1 );
		a++;
		b++;
	}
	if( *b )  return( -1 );
	return( 0 );
}

#include	<setjmp.h>
#include 	<stdio.h>
#include	<math.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#include	<signal.h>
#include      <unistd.h>
#include	"unit.h"

/*       double sqrt(double x);
         double sin(double x);
         double cos(double x);
         double tan(double x);
         double fabs(double x);
         double asin(double x);
         double acos(double x);
         double atan(double x);
         double exp(double x);
         double log(double x);	    
*/

void	execute( char s[] );
/* extern void	hand_flt();		 handle floating point exceptions */

FILE *localfopen(char name[], char mode[])
{
  char s[1000], *home;
  home = getenv("HOME");
  s[0] = 0;
  if (home != 0) {
    strcat(s, home);
    strcat(s, "/.unit/");
  }
  strcat(s, name);
  return fopen(s, mode);
}
    
void	Read_err()
{
	FILE	*f;
	/* UNITY	u; */
	int	i, success = 1;
	char	s[121];

	success = ( f = localfopen( "unit.err","rt" ) ) != NULL;

	for( i = 0; i < ERRNUM; i++ )
	{
		if( success )
			success = fgets( s, 120, f ) != NULL;
		if( success )
		{
			s[strlen(s)-1] = 0;	/* delete the \n */
			strcpy( errcode[i] = malloc( strlen(s)+1 ), s );
		} else
			*( errcode[i] = malloc(1) ) = 0;
	}
	fclose( f );
}

void	error( int n )
{
	int i;
	static int done = 0;

	if( !done ) { Read_err(); done = 1; }

	if( n >= 0 )
		{	
			printf( "\nError %d: %s\n", n, errcode[n] );
			longjmp( jmpb, 1 );
		}
	else
		{
			for ( i = 0; i < ERRNUM && *errcode[i]; i++ )
				printf( " %2d: %s\n", i, errcode[i] ) ;
		}
}

void errors()
{
	int	n = -1;

	error( n );
}

void
#ifdef sun
hand_flt(sig,code,scp)		/* we've found a floating point exception */
int sig,				/* the signal */
    code;				/* and associated code */
struct sigcontext *scp;			/* needed by a sun */
#else
hand_flt(sig,code)	       	/* we've found a floating point exception */
int sig,				/* the signal */
    code;				/* and associated code */
#endif
{
   (void)signal(SIGFPE,hand_flt);	/* reset the trap */

   switch (code) {
#ifdef hp9000s300
   case 0:
	error(30);
   case 5:
	error(7);
#endif
#ifdef FPE_FPA_ERROR
   case FPE_FPA_ERROR:	       /* The fpa uses this signal so as
					   to ask help from the 68881 */
      if(fpa_handler(scp) != 0) {
	 return;
      }
      break;
#endif
#ifdef FPE_INTOVF_TRAP
   case FPE_INTOVF_TRAP:
      error(30);
#endif
#ifdef FPE_INTDIV_TRAP
   case FPE_INTDIV_TRAP:
      error(30);
#endif
#ifdef FPE_FLTOVF_TRAP
   case FPE_FLTOVF_TRAP:
      error(30);
#endif
#ifdef FPE_FLTDIV_TRAP
   case FPE_FLTDIV_TRAP:
      error(7);
#endif
#ifdef FPE_FLTUND_TRAP
   case FPE_FLTUND_TRAP:
      error(30);
#endif
#ifdef FPE_DECOVF_TRAP
   case FPE_DECOVF_TRAP:
      error(30);
#endif
#ifdef FPE_SUBRNG_TRAP
   case FPE_SUBRNG_TRAP:
      error(30);
#endif
#ifdef FPE_FLTOVF_FAULT
   case FPE_FLTOVF_FAULT:
      error(30);
#endif
#ifdef FPE_FLTDIV_FAULT
   case FPE_FLTDIV_FAULT:
      error(7);
#endif
#ifdef FPE_FLTUND_FAULT
   case FPE_FLTUND_FAULT:
      error(30);
#endif
   default:
      error(0);
   }
}

int	eq( EXPARR a, EXPARR b )
{
	EXPS	k;

	for( k = FU; k < LU; k++ )
	if( a[k] != b[k] )
		return( FALSE );
	return( TRUE );
}

void	quit()
{
	exit( 0 );
}

/*  fichier d'aide (help) */

void Read_help()
{
	FILE	*f;
	/* UNITY	u; */
	int	i, success = 1;
	char	s[121];

	success = ( f = localfopen( "unit.help","rt" ) ) != NULL;

	for( i = 0; i < HLPNUM; i++ )
	{
		if( success )
			success = fgets( s, 120, f ) != NULL;
		if( success )
		{
			s[strlen(s)-1] = 0;	/* delete the \n */
			strcpy( hlpcode[i] = malloc( strlen(s)+1 ), s );
		} else
			*( hlpcode[i] = malloc(1) ) = 0;
	}
	fclose( f );
}

void Read_hlpa()
{
	FILE	*f;
	/* UNITY	u; */
	int	i, success = 1;
	char	s[121];

	success = ( f = localfopen( "unit.hlpa","rt" ) ) != NULL;

	for( i = 0; i < HLPNUM; i++ )
	{
		if( success )
			success = fgets( s, 120, f ) != NULL;
		if( success )
		{
			s[strlen(s)-1] = 0;	/* delete the \n */
			strcpy( hlpacode[i] = malloc( strlen(s)+1 ), s );
		} else
			*( hlpacode[i] = malloc(1) ) = 0;
	}
	fclose( f );
}

void Read_hlpc()
{
	FILE	*f;
	/* UNITY	u; */
	int	i, success = 1;
	char	s[121];

	success = ( f = localfopen( "unit.hlpc","rt" ) ) != NULL;

	for( i = 0; i < HLPNUM; i++ )
	{
		if( success )
			success = fgets( s, 120, f ) != NULL;
		if( success )
		{
			s[strlen(s)-1] = 0;	/* delete the \n */
			strcpy( hlpccode[i] = malloc( strlen(s)+1 ), s );
		} else
			*( hlpccode[i] = malloc(1) ) = 0;
	}
	fclose( f );
}

void Read_hlpe()
{
	FILE	*f;
	/* UNITY	u; */
	int	i, success = 1;
	char	s[121];

	success = ( f = localfopen( "unit.hlpe","rt" ) ) != NULL;

	for( i = 0; i < HLPNUM; i++ )
	{
		if( success )
			success = fgets( s, 120, f ) != NULL;
		if( success )
		{
			s[strlen(s)-1] = 0;	/* delete the \n */
			strcpy( hlpecode[i] = malloc( strlen(s)+1 ), s );
		} else
			*( hlpecode[i] = malloc(1) ) = 0;
	}
	fclose( f );
}

void Read_hlpf()
{
	FILE	*f;
	/* UNITY	u; */
	int	i, success = 1;
	char	s[121];

	success = ( f = localfopen( "unit.hlpf","rt" ) ) != NULL;

	for( i = 0; i < HLPNUM; i++ )
	{
		if( success )
			success = fgets( s, 120, f ) != NULL;
		if( success )
		{
			s[strlen(s)-1] = 0;	/* delete the \n */
			strcpy( hlpfcode[i] = malloc( strlen(s)+1 ), s );
		} else
			*( hlpfcode[i] = malloc(1) ) = 0;
	}
	fclose( f );
}

void Read_hlpm()
{
	FILE	*f;
	/* UNITY	u; */
	int	i, success = 1;
	char	s[121];

	success = ( f = localfopen( "unit.hlpm","rt" ) ) != NULL;

	for( i = 0; i < HLPNUM; i++ )
	{
		if( success )
			success = fgets( s, 120, f ) != NULL;
		if( success )
		{
			s[strlen(s)-1] = 0;	/* delete the \n */
			strcpy( hlpmcode[i] = malloc( strlen(s)+1 ), s );
		} else
			*( hlpmcode[i] = malloc(1) ) = 0;
	}
	fclose( f );
}

void Read_hlpn()
{
	FILE	*f;
	/* UNITY	u; */
	int	i, success = 1;
	char	s[121];

	success = ( f = localfopen( "unit.hlpn","rt" ) ) != NULL;

	for( i = 0; i < HLPNUM; i++ )
	{
		if( success )
			success = fgets( s, 120, f ) != NULL;
		if( success )
		{
			s[strlen(s)-1] = 0;	/* delete the \n */
			strcpy( hlpncode[i] = malloc( strlen(s)+1 ), s );
		} else
			*( hlpncode[i] = malloc(1) ) = 0;
	}
	fclose( f );
}

void Read_hlps()
{
	FILE	*f;
	/* UNITY	u; */
	int	i, success = 1;
	char	s[121];

	success = ( f = localfopen( "unit.hlps","rt" ) ) != NULL;

	for( i = 0; i < HLPNUM; i++ )
	{
		if( success )
			success = fgets( s, 120, f ) != NULL;
		if( success )
		{
			s[strlen(s)-1] = 0;	/* delete the \n */
			strcpy( hlpscode[i] = malloc( strlen(s)+1 ), s );
		} else
			*( hlpscode[i] = malloc(1) ) = 0;
	}
	fclose( f );
}

void	help()
{
	int i;
	static int done = 0;

	if( !done ) { Read_help();  done = 1; }

	for (i = 0; i < HLPNUM && *hlpcode[i]; i++ )
		printf( "%s\n", hlpcode[i]) ;
}

void	helpa()
{
	int i;
	static int done = 0;

	if( !done ) { Read_hlpa();  done = 1; }

	for (i = 0; i < HLPNUM && *hlpacode[i]; i++ )
		printf( "%s\n", hlpacode[i]) ;
}

void	helpc()
{
	int i;
	static int done = 0;

	if( !done ) { Read_hlpc();  done = 1; }

	for (i = 0; i < HLPNUM && *hlpccode[i]; i++ )
		printf( "%s\n", hlpccode[i]) ;
}

void	helpe()
{
	int i;
	static int done = 0;

	if( !done ) { Read_hlpe();  done = 1; }

	for (i = 0; i < HLPNUM && *hlpecode[i]; i++ )
		printf( "%s\n", hlpecode[i]) ;
}

void	helpf()
{
	int i;
	static int done = 0;

	if( !done ) { Read_hlpf();  done = 1; }

	for (i = 0; i < HLPNUM && *hlpfcode[i]; i++ )
		printf( "%s\n", hlpfcode[i]) ;
}

void	helpm()
{
	int i;
	static int done = 0;

	if( !done ) { Read_hlpm();  done = 1; }

	for (i = 0; i < HLPNUM && *hlpmcode[i]; i++ )
		printf( "%s\n", hlpmcode[i]) ;
}

void	helpn()
{
	int i;
	static int done = 0;

	if( !done ) { Read_hlpn();  done = 1; }

	for (i = 0; i < HLPNUM && *hlpncode[i]; i++ )
		printf( "%s\n", hlpncode[i]) ;
}

void	helps()
{
	int i;
	static int done = 0;

	if( !done ) { Read_hlps();  done = 1; }

	for (i = 0; i < HLPNUM && *hlpscode[i]; i++ )
		printf( "%s\n", hlpscode[i]) ;
}

void	uinsert( UNITY u, int diskpos, int fw )
{
	FILE	*f;
	UPTR	father, x;
	int	i;

	x = root;
	do {
		father = x;
		i = cmp( u.name, x->r.name );

		if( i < 0 )
			x = x->left;
		else if( i > 0 )
			x = x->right;
		else {
			x->r = u;

			if( fw )
			{
				if( ( f = fopen( "unit.dta","r+wb" )
								) == NULL )
					return;
				if( fseek( f, x->pos * sizeof(u), SEEK_SET ) )
					return;
				fwrite( &u, sizeof(u), 1, f );
				fclose( f );
			}
			return;
		}
	} while( x );

	x = malloc( sizeof(UREC) );

	if( cmp( u.name, father->r.name ) < 0 )
		father->left = x;
	else	father->right = x;

	x->left = NULL;
	x->right = NULL;
	x->r = u;
	x->pos = diskpos;

	if( fw )
	{
		nofdata++;

		if( ( f = fopen( "unit.dta","ab" ) ) == NULL )
			return;
		fwrite( &u, sizeof(u), 1, f );
		fclose( f );
	}
}

void	loadinit()
{
	FILE	*f;
	UNITY	u;
	/* int	i; */
	int    success = 1;
	/* char	s[121]; */

	if( ( f = fopen( "unit.dta","rb" ) ) == NULL )
	if( ( f = localfopen( "unit.dta","rb" ) ) == NULL )
	{
		printf( "\"unit.dta\": not found\n" );
		exit( 255 );
	}

	root = (UPTR) malloc( sizeof(UREC) );

	root->left = NULL;
	root->right = NULL;
	root->pos = 0;
	fread( &root->r, sizeof(root->r), 1, f );

	for( nofdata = 1; success; nofdata++ )
	{
		if( fread( &u, sizeof(u), 1, f ) != 1 )
			success = 0;
		else	uinsert( u, nofdata, FALSE );
	}
	nofdata--;

	fclose( f );

	if( ( f = fopen( "unit.mac","rt" ) ) == NULL )
	if( ( f = localfopen( "unit.mac","rt" ) ) == NULL )
		return;

	for( success = 1; success && macnum < MAXMACNUM; macnum++ )
	{
		success = fgets( macro[macnum].name, 11, f ) != NULL;
		if( success )
			success = fgets( 
				macro[macnum].code, MAXMACLEN, f ) != NULL;  
		if( success )
		{
			macro[macnum].name[strlen(macro[macnum].name)-1] = 0;
			macro[macnum].code[strlen(macro[macnum].code)-1] = 0;
				/* delete the \n's */
		}
	}
	macnum--;

	fclose( f );
}

void	initprc()
{
	EXPS	k;

	sp = 0;
	flags = NOFLAGS;
	macnum = 0;

	loadinit();
	(void)signal(SIGFPE,hand_flt);	/* catch floating point exceptions */

	printf( "UNIT version %s, %s  (C)1990  L. Bartholdi\n", VER, DATE );

	for( k = FU; k < LU; k++ )
		lastu.exp[k] = 0;
	lastu.f = 0.0;
}

void	pop( STKEL *x )
{
	if( sp == 0 )
		error( 2 );
	*x = stack[--sp];
}

void	clearstack()
{
	sp = 0;
}

void	push( STKEL x )
{
	if( sp >= MAXSP )
		error( 16 );
	stack[sp++] = x;
}

int	search( char n[], UPTR p, UNITY *u )
{
	int	i;

	if( p == NULL )
		return( FALSE );

	i = cmp( n, p->r.name );

	if( i < 0 )
		return( search( n, p->left, u ) );
	else if( i > 0 )
		return( search( n, p->right, u ) );
	else {
		*u = p->r;
		return( TRUE );
	}
}

void	setcmode( int m )
{
	cmode = m;
	npar = 0;
	opp = 0;
	spp = 0;
	jop = TRUE;
}

void	operator( STKEL *result, char ch, STKEL x, STKEL y )
{
	EXPS	k;
	int	i, j;

	switch( ch )
	{
	case	'^':
		i = y.f;
		if( ( floor( y.f ) != y.f) || !eq( y.exp,o ) || 
							fabs( y.f ) > 32.0 )
			error( 13 );
		else
			result->f = 1.0;
		if( i > 0 )
			{
			for ( j = 0; j < i; j++ )
				result->f = result->f * x.f; 
			}
		else 	{
			for ( j = 0; j < -i; j++ )
				result->f = result->f / x.f;
			}
		for( k = FU; k < LU; k++ )
			result->exp[k] = x.exp[k] * i;
		break;
	case	'*':
		result->f = x.f * y.f;
		for( k = FU; k < LU; k++ )
			result->exp[k] = x.exp[k] + y.exp[k];
		break;
	case	'/':
		if( y.f == 0.0 )
			error( 7 );
		result->f = x.f / y.f;
		for( k = FU; k < LU; k++ )
			result->exp[k] = x.exp[k] - y.exp[k];
		break;
	case	'+':
		if( eq( x.exp,y.exp ) )
			result->f = x.f + y.f;
		else	error( 1 );
		break;
	case	'-':
		if( eq( x.exp,y.exp ) )
			result->f = x.f - y.f;
		else	error( 1 );
	}
}

void	reduce()
{
	STKEL	x;

	switch( cmode )
	{
	case	INFIX:
		jop = TRUE;
		while( --opp >= 0 )
		{
			if( opbuf[opp] != '(')
			{
				pop( &x );
				if( sp == 0 )
					error( 2 );
				operator( &st, opbuf[opp], st, x );
			} else {
				if( --npar < 0 )
					error( 19 );
			}
		}
		opp = 0;
		break;

	case	PREFIX:
		jop = TRUE;
		while( --opp >= 0 )
		{
			if( opbuf[opp] == '(')
			{
				if( npar-- < 1 )
					error( 19 );
			} else {
				spp--;
				while( sp-1 != spbuf[spp] )
				{
					pop( &x );
					if( sp == 0 )
						error( 2 );
					operator( &st, opbuf[opp], st, x );
				}
			}
		}
		opp = 0;
	}
}

void	donum( double f )
{
	STKEL	x;
	EXPS	k;

	for( k = FU; k < LU; k++ )
		x.exp[k] = 0;
	x.f = f;

	if( flags == INTO || flags == DEF )
	{
		if( sp == 0 )
			error( 2 );
		operator( &st, '/', st, x );
		return;
	}

	if( cmode == INFIX && !jop )
		error( 27 );
	jop = FALSE;

	push( x );
}

int	prior( char a, char b )
{
	return( a != '(' && b != '(' && b != ')' &&
		(a == '^' || ((a == '*' || a == '/') && b != '^') ) );
}

void	docomp( char ch )
{
	STKEL	x;

	switch( cmode )
	{
	case	PREFIX:
		switch( ch )
		{
		case	'(':
			jop = TRUE;

			npar++;
			spbuf[spp++] = sp;
			if( spp == MAXSP )
				error( 20 );

			opbuf[opp++] = ch;
			if( opp == MAXOP )
				error( 20 );
			break;
		case	')':
			jop = FALSE;
			if( --npar < 0 )
				error( 18 );
			if( sp == spbuf[--spp] )
				error( 26 );
			if( --opp == 0 )
				error( 12 );
			while( sp-1 != spbuf[spp] )
			{
				pop( &x );
				if( sp == 0 )
					error( 2 );
				operator( &st, opbuf[opp], st, x );
			}
			opp--;
			break;
		default:
			if( spp == 0 )
				error( 2 );
			if( opp < MAXOP )
			{
				if( !jop )
					error( 26 );
				jop = FALSE;
				opbuf[opp++] = ch;
			}
			else	error( 20 );
		}
		break;

	case	INFIX:
		switch( ch )
		{
		case	'+':	case	'-':
		case	'*':	case	'/':	case '^':
			if( jop )
				error( 26 );
			jop = TRUE;

			while( opp > 0 && prior( opbuf[opp-1], ch ) )
			{
				pop( &x );
				if( sp == 0 )
					error( 2 );
				operator( &st, opbuf[--opp], st, x );
			}
			break;
		case	'(':
			if( !jop )
				error( 27 );
			npar++;
			break;
		case	')':
			if( jop )
				error( 26 );
			if( --npar < 0 )
				error( 18 );

			while( --opp >= 0 && opbuf[opp] != '(')
			{
				pop( &x );
				if( sp == 0 )
					error( 2 );
				operator( &st, opbuf[opp], st, x );
			}
			if( opp < 0 )
				error( 18 );
			return;
		}

		if( opp < MAXOP )
			opbuf[opp++] = ch;
		else	error( 20 );
		break;

	case	POSTFIX:
		if( ch == '(' || ch == ')')
			error( 17 );
		pop( &x );
		if( sp == 0 )
			error( 2 );
		operator( &st, ch, st, x );
		break;
	}
}

void	chsign()
{
	if( sp == 0 )
		error( 2 );
	st.f = -st.f;
}

void	inverse()
{
	EXPS	k;

	if( sp == 0 )
		error( 2 );

	if( st.f == 0.0 )
		error( 4 );

	st.f = 1 / st.f;
	for( k = FU; k < LU; k++ )
		st.exp[k] = -st.exp[k];
}

void	sqroot()
{
	EXPS	k;

	if( sp == 0 )
		error( 2 );

	if( st.f < 0.0 )
		error( 3 );

	st.f = sqrt( st.f );
	for( k = FU; k < LU; k++ )
	{
		if( st.exp[k] & 1 )
			error( 6 );
		st.exp[k] = st.exp[k] / 2;
	}
}

void	sine()
{
	if( sp == 0 )
		error( 2 );

	st.exp[RADIAN]--;
	if( !eq( st.exp, o ) )
		error( 22 );

	st.f = sin( st.f );
}

void	cosine()
{
	if( sp == 0 )
		error( 2 );

	st.exp[RADIAN]--;
	if( !eq( st.exp, o ) )
		error( 22 );

	st.f = cos( st.f );
}


void	tang()
{
	if( sp == 0 )
		error( 2 );

	st.exp[RADIAN]--;
	if( !eq( st.exp, o ) )
		error( 22 );

	st.f = tan( st.f );
}

void	asine()
{
	if( sp == 0 )
		error( 2 );

	if( !eq( st.exp, o ) )
		error( 24 );

	if( fabs( st.f ) > 1.0 )
		error( 25 );

	st.exp[RADIAN]++;
	st.f = asin( st.f );
}

void	acosine()
{
	if( sp == 0 )
		error( 2 );

	if( !eq( st.exp, o ) )
		error( 24 );

	if( fabs( st.f ) > 1.0 )
		error( 25 );

	st.exp[RADIAN]++;
	st.f = acos( st.f );
}

void	atang()
{
	if( sp == 0 )
		error( 2 );

	if( !eq( st.exp, o ) )
		error( 24 );

	st.exp[RADIAN]++;
	st.f = atan( st.f );
}

void	expon()
{
	if( sp == 0 )
		error( 2 );

	if( !eq( st.exp, o ) )
		error( 24 );

	st.f = exp( st.f );
}

void	logarithm()
{
	if( sp == 0 )
		error( 2 );

	if( !eq( st.exp, o ) )
		error( 24 );

	if( st.f <= 0.0 )
		error( 23 );

	st.f = log( st.f );
}

void	logdecimal()
{
	if( sp == 0 )
		error( 2 );

	if( !eq( st.exp, o ) )
		error( 24 );

	if( st.f <= 0.0 )
		error( 23 );

	st.f = log( st.f ) / log( 10.0 );
}

void	logdeux()
{
	if( sp == 0 )
		error( 2 );

	if( !eq( st.exp, o ) )
		error( 24 );

	if( st.f <= 0.0 )
		error( 23 );

	st.f = log( st.f ) / log( 2.0 );
}

void	root2()
{
	EXPS	k;

	if( sp == 0 )
		error( 2 );

	if( st.f < 0.0 )
		error( 3 );

	st.f = sqrt( 2.0 * st.f );
	for( k = FU; k < LU; k++ )
	{
		if( st.exp[k] & 1  )
			error( 6 );
		st.exp[k] = st.exp[k] / 2;
	}
}

void	sqr2()
{
	EXPS	k;

	if( sp == 0 )
		error( 2 );

	st.f = 0.5 * st.f * st.f;
	for( k = LU; k < FU; k++ )
		st.exp[k] = st.exp[k] * 2;
}

void	getnum( char name[], int num, int positive, char s[] )
{
	if( num == 0 )
		return;
	if( positive ^ ( num > 0 ) )
		return;

	sprintf( s+strlen(s), " %s", name );

	if( num != 1 )
		sprintf( s+strlen(s), "%d", num );
}

void	getexp( EXPARR exp, char s[] )
{
	int	sign;
	EXPS	k;

	struct {
		char	name[3];
		int	exp;
		}
		t[] = {
		{"$", DOLLAR },
		{"R", RADIAN },
		{"Sr", STERADIAN },
		{"A", AMPERE },
		{"Bq", BQ },
		{"Cd", CANDELA },
		{"K", KELVIN },
		{"Kg", KILO },
		{"M", METER },
		{"S", SECOND }
		};

	strcpy( s, "[" );

	for( sign = TRUE; sign >= FALSE; sign-- )
	for( k = FU; k < LU; k++ )
		getnum( t[k].name, exp[t[k].exp], sign, s );

	sprintf( s+strlen(s), " ]" );
}

void	printexp( EXPARR exp )
{
	char	s[120];

	getexp( exp, s );

	printf( "MKSA exponents: %s\n", s );
}

void	dointo( char s[] )
{
	STKEL	x;
	UNITY	r;
	EXPS	k;

	if( cmp( s, "MKSA" ) == 0 )
	{
		pop( &x );
		flags = NOFLAGS;

		printf( "%.*lG\t", prec, x.f );
		printexp( x.exp );

		return;
	}

	if( search( s, root, &r ) )
	{
		flags = NOFLAGS;
		pop( &x );

		if( r.f2 == VAROFF )
		{
			for( k = FU; k < LU; k++ )
				r.exp[k] = x.exp[k];

			r.f1 = x.f;
			uinsert( r, nofdata+1, FALSE );
			return;
		}

		if( !eq( x.exp, r.exp ) )
			error( 1 );

		printf( "%.*lG\t%s\n", prec, ( x.f - r.f2 ) / r.f1, s );
		return;
	}
	error( 5 );
}

void	uscan( EXPARR x, UPTR p, int *col )
{
	if( p == NULL )
		return;

	uscan( x, p->left, col );

	if( eq( x, p->r.exp ) )
	{
		printf( "%15s", p->r.name );

		if( ++*col == 5 )
		{
			*col = 0;
			printf( "\n" );
		}
	}

	uscan( x, p->right, col );
}

void	question()
{
	int	col = 0;

	reduce();
	if( sp == 0 )
		error( 2 );
	printexp( st.exp );

	printf( "Equivalent units: \n" );
	uscan( st.exp, root, &col );
	if( col != 0 )
		printf( "\n" );
}

void	all1( int t )
{
	reduce();
	if( sp == 0 )
		error( 2 );
	flags = t;
	lastu = st;
}

void	setoffset()
{
	STKEL	x;

	pop( &x );
	offset = x.f;
}

void	setprec()
{
	STKEL	x;
	pop( &x );

	if( x.f >= 1.0 && x.f <= 16.0 )
		prec = x.f;
}

void	swap()
{
	STKEL	a, b;

	pop( &a );
	pop( &b );
	push( a );
	push( b );
}

void	last()
{
	if( cmode == INFIX && !jop )
		error( 27 );
	jop = FALSE;
	push( lastu );
}

void	dodup()
{
	if( sp == 0 )
		error( 2 );
	push( st );
}

double	frac( double x )
{
	return( x - trunc(x) );
}

void	putdms()
{
	STKEL	x;
	int	hr, min, sec;
	EXPARR	_angle = { 0,0,0,0,0,0,0,0,1,0 },
		_time  = { 0,0,1,0,0,0,0,0,0,0 };

	reduce();
	pop( &x );

	if( x.f > 1E9 )
	{
		push( x );
		dointo( "MKSA" );
		return;
	}

	if( eq( x.exp, _angle ) )
		x.f = x.f * 3600.0 * 180.0/3.1415926535;
	else	if( eq( x.exp, o ) )
		x.f = x.f * 3600.0;
	else	if( !eq( x.exp, _time ) )
		error( 28 );

	hr  = x.f / 3600;
	min = abs(frac( x.f / 3600.0 ) * 60);
	sec = abs(round(frac( frac(x.f/3600.0) * 60 ) * 60));

	if( sec == 60 ) { min = min + hr / abs(hr); sec = 0; }
	if( min == 60 ) { hr  = hr + hr / abs(hr) ; min = 0; }

	printf( " %d* %d' %d\"     ", hr, min, sec );
	printf( " %d:%02d:%02d\n", hr, min, sec );
}

void	dir1()
	{
		flags = DIR; 
	}
void	begmacro()
	{	macn = -1; flags = MACROF; }
void	into1()
	{	all1( INTO ); }
void	dot()
	{	all1( INTO ); dointo( "MKSA" ); }
void	def1()
	{	all1( DEF ); }
void	var1()
	{	all1( DEF ); offset = VAROFF; }
void	wrt1()
	{	all1( DEF ); diskmod = TRUE; }
void	dvar1()
	{	all1( DEF ); offset = VAROFF; diskmod = TRUE; }
void	setpostfix()
	{	setcmode( POSTFIX ); }
void	setinfix()
	{	setcmode( INFIX ); }
void	setprefix()
	{	setcmode( PREFIX ); }

void	define( char name[] )
{
	UNITY	u;
	STKEL	x;
	EXPS	k;

	flags = NOFLAGS;
	pop( &x );

	if( x.f == 0.0 )
		error( 29 );

	strcpy( u.name, name );
	u.f1 = x.f;
	u.f2 = offset;
	offset = 0.0;
	for( k = FU; k < LU; k++ )
		u.exp[k] = x.exp[k];

	uinsert( u, nofdata, diskmod );
	diskmod = 0;
}

int	match( char m[], char s[] )
{
	while( *m )
	{
		if( *m >= 'a' && *m <= 'z')
			*m = *m - 'a' + 'A';
		switch( *m )
		{
		case	'?':
			s++;
			break;
		case	'*':
			if( !m[1] )
				return( TRUE );
			while( m[1] != *s && *s )
				s++;
			break;
		default:
			if( *m != *s++ )
				return( FALSE );
		}
		m++;

		if( *m && !*s )
			return( FALSE );
	}
	return( TRUE );
}

void	scandir( char m[], UPTR p, int *col )
{
	if( p == NULL )
		return;

	scandir( m, p->left, col );

	if( match( m, p->r.name ) )
	{
		printf( "%15s", p->r.name );

		if( ++*col == 5 )
		{
			*col = 0;
			printf( "\n" );
		}
	}
	scandir( m, p->right, col );
}

void	printdir( char m[] )
{
	int	col = 0;

	flags = NOFLAGS;
	scandir( m, root, &col );
	if( col != 0 )
		printf( "\n" );
}

void	reset()
{
	flags = NOFLAGS;
	jop = TRUE;
	diskmod = FALSE;
	clearstack();

	opp = 0;
	spp = 0;
	offset = 0;
	npar = 0;
	recur = 1;
}

int	umod( char n[] )
{
	STKEL	x;
	UNITY	r;
	EXPS	k;

	if( search( n, root, &r ) )
	{
		if( r.f2 == VAROFF )
		{
			for( k = FU; k < LU; k++ )
				x.exp[k] = r.exp[k];
			x.f = r.f1;
			push( x );

			return( TRUE );
		}

		if( sp == 0 )
			error( 2 );

		st.f = st.f * r.f1 + r.f2;
		for( k = FU; k < LU; k++ )
			st.exp[k] += r.exp[k];

		return( TRUE );
	}
	return( FALSE );
}

void	domacro( char s[] )
{
	int	i;

	if( macn == -1 )
	{
		for( i = 0; i < macnum; i++ )
		if( cmp( macro[i].name, s ) == 0 )
			macn = i;

		if( macn == -1 )
		{

			macn = macnum;
			if( ++macnum > MAXMACNUM )
				error( 31 );
		}

		strcpy( macro[macn].name, s );
		macro[macn].code[0] = 0;
		return;
	}

	if( strlen(macro[macn].code) + strlen(s) + 2 > MAXMACLEN )
		error( 31 );

	sprintf( macro[macn].code+strlen(macro[macn].code), "%s ", s );

	if( cmp( s, ";" ) == 0 )
		flags = NOFLAGS;
}

int	playmacro( char s[] )
{
	int	i;
	char	*p, *q;

	for( i = 0; i < macnum; i++ )
	if( cmp( macro[i].name, s ) == 0 )
	{
		p = macro[i].code;

		while( *p != ';')
		{
			for( q = p; *q != ' '; q++ );

			*q = 0;
			execute( p );
			*q = ' ';
			p = q+1;
		}
		return( TRUE );
	}
	return( FALSE );
}

void	savemacro()
{
	int	i;
	FILE	*f;

	if( (f = fopen("unit.mac", "wt")) == NULL )
		error( 14 );

	for( i = 0; i < macnum; i++ )
	if( fprintf( f, "%s\n%s\n", macro[i].name, macro[i].code ) == EOF )
		error( 14 );

	fclose( f );
}

void	listmacro()
{
	int	i;

	for( i = 0; i < macnum; i++ )
		printf(" %s -> '%s'\n", macro[i].name, macro[i].code );
}

struct	{
	char	*s;
	void	(*p)();
	}
	dispatchtbl[] = {
	{".", dot },
	{".DMS", putdms },
	{"?", question },
	{":", begmacro },
	{"ACOS", acosine },
	{"AIDE", help },
	{"AOS", setinfix },
	{"ARITH", helpa },
	{"ASIN", asine },
	{"ATAN", atang },
	{"CHS", chsign },
	{"CLEAR", clearstack },
	{"COM", helpc },
	{"COMMANDES", helpc },
	{"COS", cosine },
	{"DIG", setprec },
	{"DIGITS", setprec },
	{"DEF", def1 },
	{"DEFINE", def1 },
	{"DIR", dir1 },
	{"DUP", dodup },
	{"DVAR", dvar1 },
	{"EN", into1 },
	{"ERR", errors },
	{"ERREURS", errors },
	{"EX", quit },
	{"EXAMPLE", helpe },
	{"EXEMPLE", helpe },
	{"EXEMPLE2", helpf },
	{"EXAMPLE2", helpf },
	{"EXIT", quit },
	{"EXP", expon },
        {"FIN", quit },
	{"HALT", quit },
	{"HELP", help },
	{"INFIX", setinfix },
	{"INTO", into1 },
	{"INV", inverse },
	{"LAST", last },
	{"LIST", helpc },
	{"LMAC", listmacro },
	{"LN", logarithm },
	{"LOG", logdecimal },
	{"L2", logdeux },
	{"MACRO", helpm },
	{"MATH", helpa },
	{"NEW", helpn },
	{"OFFSET", setoffset },
	{"PREFIX", setprefix },
	{"POLISH", setpostfix },
	{"POSTFIX", setpostfix },
	{"QUIT", quit },
	{"R2", root2 },
	{"REDUCE", reduce },
	{"RPN", setpostfix },
	{"S2", sqr2 },
	{"SAVE", helps },
	{"SIN", sine },
	{"SMAC", savemacro },
	{"SQRT", sqroot },
	{"STOP", quit },
	{"SWAP", swap },
	{"TAN", tang },
	{"TO", into1 },
	{"VAR", var1 },
	{"WRITE", wrt1 },
	{"", NULL }
	};

int	dispatch( char s[] )
{
	int	i;

	for( i = 0; dispatchtbl[i].p; i++ )
	if( cmp( s, dispatchtbl[i].s ) == 0 )
	{
		(*dispatchtbl[i].p)();
		return( TRUE );
	}
	return( FALSE );
}

void	execute( char s[] )
{
	double	f;
	int	i;

	if( flags == MACROF )
	{
		domacro( s );
		return;
	}

	if( sscanf( s, "%lf%n", &f, &i ) && i == strlen(s) && cmp( s, "+" ) && cmp( s, "-" ) ) /* added by LB, 20200504: check if all input was read */
	{
		donum( f );
		return;
	}

	if( cmp( s, "RESET" ) == 0 )
	{
		reset();
		return;
	}

	if( ++recur > MAXRECUR )
		error( 9 );

	switch( flags )
	{
	case	INTO:
		dointo( s );
		break;
	case	DIR:
		printdir( s );
		break;
	case	DEF:
		define( s );
		break;
	default:
		if( cmp( s, "**" ) == 0 )
			strcpy( s, "^" );
		if( (*s == '^' || *s == '+' || *s == '-' ||
			*s == '*' || *s == '/' ||
			*s == '(' || *s == ')') && !s[1] )
		{
			docomp( *s );
			break;
		}

		if( !dispatch( s ) )
		if( !playmacro( s ) )
		if( !umod( s ) )
			error( 5 );
	} /* case */

	recur--;
	fflush( stdout );
}

void	finishjob()
{
	switch( flags ) {
	case	DIR:
		printdir("*");
		break;
	case	NOFLAGS:
		return;
	default:
		error(32);
	}
}

/* 	char	*cmdptr;       maintenant dans    unit.h    */
	/* it HAS to be a global !!! */
	/* because of setjmp */

int	main()
{
	/* int	i; */
	char	*q, old;
	char	s[100];

	initprc();
	execute( "RESET" );

	if( setjmp( jmpb ) )
	{
		for( q = s; q < cmdptr; q++ )
			printf( " " );
		printf( "     ^\n" );

		flags = NOFLAGS;
		execute( "RESET" );
	}

	for(;;)
	{
		if( sp == 0 )
			printf( "   ! " );
		else	printf( "[%2d] ", sp );
		for( q = s; q < s + 100; *q++ = 0 );   
		fgets( cmdptr = s, 100, stdin );

		{ /* added 20200503 to fix EOF */
		  int n = strlen(s);
		  if (n > 0 && s[n-1] == '\n')
		    s[n-1] = 0;
		  else {
		    finishjob();
		    quit();
		  }
		}
		
		while( *cmdptr )
		{
			for( q = cmdptr; *q && *q != ' ' && *q != '\t'; q++ );

			old = *q;
			*q = 0;
			execute( cmdptr );
			*q = old;

			for( cmdptr = q; *cmdptr == ' ' || *cmdptr == '\t';
				 cmdptr++ );
		}
		finishjob();
	}
}
