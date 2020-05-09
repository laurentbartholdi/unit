/* include file for UNIT.C */

typedef	enum { METER, KILO, SECOND, AMPERE, KELVIN, CANDELA, BQ,
	DOLLAR, RADIAN, STERADIAN }
	EXPS;

typedef	short
	EXPARR[10];

typedef	struct {
	char	name[11];
	EXPARR	exp;
	double	f1, f2;
	}
	UNITY;

typedef	struct {
	int	pos;
	void	*left, *right;
	UNITY	r;
	}
	UREC;

typedef	UREC
	*UPTR;

typedef	struct {
	double	f;
	EXPARR	exp;
	}
	STKEL;

#define	FU	METER
#define	LU	(STERADIAN+1)
#define	ERRNUM	33
#define	HLPNUM	65
#define	MAXMACNUM	50
#define	MAXMACLEN	200
#define	VER	"1.06"
#define	DATE	"18.06.90"
#define	VAROFF	1.23456E37
	/* special offset meaning we declare a variable */
#define	MAXRECUR	6
#define	MAXOP	10
#define	MAXSP	10
#define	MAXSTK	20
#define	TRUE	1
#define	FALSE	0

#ifdef	CASE
#define	cmp	strcmp
#else
#define	cmp	stricmp
#endif

enum { NOFLAGS, INTO, DEF, DIR, MACROF }
	flags;
jmp_buf	jmpb;

enum { POSTFIX, INFIX, PREFIX }
	cmode =
#ifdef	DEFINFIX
	INFIX;
#else
#ifdef	DEFPREFIX
	PREFIX;
#else
	POSTFIX;
#endif
#endif

EXPARR	o = { 0, };

short	prec =
#ifdef	HIPREC
	16;
#else
	3;
#endif

UPTR	root;
char	*errcode[ERRNUM];

char	*hlpcode[HLPNUM];
char	*hlpacode[HLPNUM];
char	*hlpccode[HLPNUM];
char	*hlpecode[HLPNUM];
char	*hlpfcode[HLPNUM];
char	*hlpmcode[HLPNUM];
char	*hlpncode[HLPNUM];
char	*hlpscode[HLPNUM];

STKEL	stack[MAXSTK];
STKEL	lastu;
int	sp;
#define	st	(stack[sp-1])

double	offset;

int	opp;
char	opbuf[MAXOP];
	/* chars waiting to be used in infix mode */
int	spp;
int	spbuf[MAXSP];
	/* stack ptrs waiting to be used in prefix mode */
short	npar;	/* # of () not closed */

short	recur;
int	nofdata;
short	jop;
short	diskmod;

int	macnum, macn;
struct {
	char	name[11];
	char	code[MAXMACLEN];
	}
	macro[MAXMACNUM];

char	*cmdptr;     /* points toward the begining of the current command
			string  */
