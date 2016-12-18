/*
 * File: syslev.c
 *
 * System level program
 *
 * Bob Eager   June 2000
 *
 */

/*
 * History:
 *
 *	1.0	Initial version.
 *	1.1	Added support for letters in level2 section.
 *		Corrected omission of some zero digits.
 *		Added support for component type field.
 *
 */

#pragma	strings(readonly)

#include <os2.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* Definitions */

#define	VERSION			1	/* Major version */
#define	EDIT			1	/* Edit number within major version */

#define	BUFSIZE			200	/* Space for structure */
#define	SYSLEVEL		"SYSLEVEL\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"

/* Structure definitions */

#pragma	pack(1)
typedef struct _SL {
UCHAR	id[2];				/* Identifier (0xffff) */
UCHAR	name[31];			/* Function name */
UCHAR	type;				/* Type code (0x25 = application) */
UCHAR	reserved_1[3];			/* Reserved */
USHORT	system_id;			/* System identifier */
UCHAR	system_edition;			/* System edition (0x0f = OTHER) */
UCHAR	level[2];			/* Program level nibbles */
UCHAR	reserved_2[2];			/* Reserved */
UCHAR	current_csd[7];			/* Current CSD level */
UCHAR	separator_1;			/* Separator (0x5f) */
UCHAR	previous_csd[7];		/* Current CSD level */
UCHAR	separator_2;			/* Separator (0x5f) */
UCHAR	title[80];			/* Program title */
UCHAR	service_id[9];			/* Service identifier */
UCHAR	level2;				/* Program level extension */
UCHAR	stype[19];			/* Component type */
} SL, *PSL;

/* Forward references */

static	VOID	display(PUCHAR, PUCHAR);
static	VOID	error(PUCHAR, ...);
static	PUCHAR	makestring(PUCHAR, INT);
static	VOID	putusage(VOID);

/* Local storage */

static	PUCHAR	progname;		/* Name of program, as a string */

/* Help text */

static	const	PUCHAR helpinfo[] = {
"%s: system level program",
"Synopsis: %s file",
""
};


/*
 * Main program
 *
 */

INT main(INT argc, PUCHAR argv[])
{	UINT n;
	PUCHAR p;
	FILE *ifp;
	UCHAR buf[BUFSIZE];

	progname = strrchr(argv[0], '\\');
	if(progname != (PUCHAR) NULL)
		progname++;
	else
		progname = argv[0];
	p = strchr(progname, '.');
	if(p != (PUCHAR) NULL) *p = '\0';
	strlwr(progname);

	if(argc != 2) {
		putusage();
		exit(EXIT_FAILURE);
	}

	/* Open file */

	ifp = fopen(argv[1], "rb");
	if(ifp == (FILE *) NULL) {
		error("cannot open input file '%s'", argv[1]);
		exit(EXIT_FAILURE);
	}

	/* Get the data */

	memset(buf, 0, BUFSIZE);
	n = fread(buf, 1, BUFSIZE, ifp);
	if(n == 0) {
		error("file is corrupt or unreadable");
		exit(EXIT_FAILURE);
	}

	/* Display it */

	display(argv[1], buf);

	/* Clean up */

	fclose(ifp);

	return(EXIT_SUCCESS);
}


/*
 * Display the sys level information
 *
 */

static VOID display(PUCHAR file, PUCHAR buf)
{	PSL sl;
	UINT lev2;

	sl = (PSL) &buf[0];

	fprintf(stdout, "File:\t\t\t%s\n", file);

	if(sl->id[0] != 0xff || sl->id[1] != 0xff)
		fprintf(stdout, "*** ID field is not 0xffff\n");

	if(strcmp(&(sl->name[0]), SYSLEVEL) != 0)
		fprintf(stdout, "*** Function name is not SYSLEVEL\n");

	fprintf(
		stdout,
		"Product:\t\t%s\n",
		makestring(&(sl->title[0]), sizeof(sl->title)));

	if(sl->stype[0] != '\0')
		fprintf(stdout, "Type:\t\t\t%s\n", sl->stype);

	fprintf(
		stdout,
		"Version:\t\t%d.%d",
		sl->level[0] >> 4,
		sl->level[0] & 0x0f);
		fprintf(stdout, "%d", sl->level[1]);
	if(sl->level2 != '\0') {
		lev2 = sl->level2;
		if(lev2 < 0x10) {
			fprintf(stdout, ".%d", lev2);
		} else {
			if((0x10 <= lev2) && (lev2 <= 0x2f)) {
				lev2 = lev2 - 0x10 + 'A' - 1;
			} else {
				lev2 = lev2 - 0x30 + 'a' - 1;
			}
			fprintf(stdout, ".%c", lev2);
		}
	}
	fputc('\n', stdout);

	fprintf(
		stdout,
		"Current CSD level:\t%s\n",
		makestring(&(sl->current_csd[0]), sizeof(sl->current_csd)));
	fprintf(
		stdout,
		"Prior CSD level:\t%s\n",
		makestring(&(sl->previous_csd[0]), sizeof(sl->previous_csd)));

	fprintf(
		stdout,
		"Component ID:\t\t%s\n",
		makestring(&(sl->service_id[0]), 9));
	if(sl->stype[0] != 0) {
		fprintf(
			stdout, "Component type:\t\t%s\n",
			&(sl->stype[0]));
	}
}


/*
 * Return a value as a (static) string.
 *
 */

static PUCHAR makestring(PUCHAR str, INT len)
{	static UCHAR s[100];
	INT i;

	for(i = 0; i < len; i++) s[i] = str[i];
	s[len] = '\0';

	return(s);
}


/*
 * Print message on standard error in printf style,
 * accompanied by program name.
 *
 */

static VOID error(PUCHAR mes, ...)
{	va_list ap;

	fprintf(stderr, "%s: ", progname);

	va_start(ap, mes);
	vfprintf(stderr, mes, ap);
	va_end(ap);

	fputc('\n', stderr);
}


/*
 * Output program usage information.
 *
 */

static VOID putusage(VOID)
{	PUCHAR *p = (PUCHAR *) helpinfo;
	PUCHAR q;

	for(;;) {
		q = *p++;
		if(*q == '\0') break;

		fprintf(stderr, q, progname);
		fputc('\n', stderr);
	}
	fprintf(stderr, "\nThis is version %d.%d\n", VERSION, EDIT);
}

/*
 * End of file: syslev.c
 *
 */
