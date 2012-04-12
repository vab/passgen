/*  Author:		V. Alex Brennen (vab@mit.edu)	*/
/*  Copyright:		None				*/
/*  License:		Public Domain			*/
/*  Created:		2006.09.15			*/
/*  Last Updated:	2011.06.18			*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "passgen_conf.h"
#include "passgen_datastructures.h"


/* The potential password types as static flags */
#define D_DEFAULT	0
#define D_ALPHA_NUMERIC	1
#define D_ALPHA_ONLY	2
#define D_NUMERIC_ONLY	3


/* Function prototypes */
int	map_data_to_chars(struct passgen_config *);
int	print_help(void);
int	invalid_argument(void);

