/*  Author:		V. Alex Brennen (vab@mit.edu)	*/
/*  Copyright:		None				*/
/*  License:		Public Domain			*/
/*  Created:		2006.12.26			*/
/*  Last Updated:	2011.06.18			*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/file.h>

#include "passgen_datastructures.h"


int	init_passgen_config(struct passgen_config **);
int	read_passgen_config(struct passgen_config **);
void	config_reject(char *);
int	file_exists(char *);

