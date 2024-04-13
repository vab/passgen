/*  Author:         V. Alex Brennen     */
/*  Copyright:      None                */
/*  License:        Public Domain       */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/file.h>

#include "passgen_datastructures.h"


int     init_passgen_config(struct passgen_config **);
int     read_passgen_config(struct passgen_config **);
void    config_reject(char *);
int     file_exists(char *);
