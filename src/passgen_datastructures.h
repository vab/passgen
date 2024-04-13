/*  Author:         V. Alex Brennen     */
/*  Copyright:      None                */
/*  License:        Public Domain       */

#ifndef PASSGEN_DATASTRUCTURES
#define PASSGEN_DATASTRUCTURES

/* This is the basic configuration datastructure.  It is initialized by
	init_passgen_config() to default values.  Later, it is reconfigured
	according to the /etc/passgen.conf file if it exists by the
	read_passgen_config() function.
*/
struct passgen_config
{
	int	num_chars;
	int	num_passwds;
	int	use_urandom;
	int	no_space;
	int	passwd_type;
	int	homoglyph_suppression_level;
	int lowercase_only;
	int uppercase_only;
	int suppress_config_warning;
};

#endif
