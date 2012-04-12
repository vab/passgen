/*  Author:		V. Alex Brennen (vab@mit.edu)	*/
/*  Copyright:		None				*/
/*  License:		Public Domain			*/
/*  Created:		2006.12.26			*/
/*  Last Updated:	2011.06.18			*/

#include "passgen_conf.h"


int init_passgen_config(struct passgen_config **config)
{
	/* We default to 14 chars because that is thought to still 
 	 * relatively secure.
	 */
	(*config)->num_chars = 14;
	(*config)->num_passwds = 5;
	(*config)->use_urandom = 0;
	(*config)->no_space = 0;
	(*config)->passwd_type = 0;
	(*config)->homoglyph_suppression_level = 0;
	(*config)->lowercase_only = 0;
	(*config)->uppercase_only = 0;
	(*config)->suppress_config_warning = 0;

	return 0;
}


int read_passgen_config(struct passgen_config **config)
{
	FILE *conf_file = NULL;
	char line[513];
	char file_loc[1024];
	char *p = NULL;


	memset(line,0x00,513);
	memset(file_loc,0x00,1024);

	if(file_exists(CONF))
	{
		strncpy(file_loc,CONF,1023);
	}
	else if(file_exists("/etc/passgen.conf"))
	{
		strncpy(file_loc,"/etc/passgen.conf",63);
	}
	else if(file_exists("/usr/local/etc/passgen.conf"))
	{
		strncpy(file_loc,"/usr/local/etc/passgen.conf",63);
	}
	else
	{
		if((*config)->suppress_config_warning == 0)
		{
			fprintf(stderr,"Warning: Unable to find configuration file.\n");
		}
		
		return -1;
	}

	if((conf_file = fopen(file_loc,"r")) == NULL)
	{
		if((*config)->suppress_config_warning == 0)
		{
			fprintf(stderr,"Failed to open configuration file: %s\n",file_loc);
		}
		
		return -1;
	}

	while(fgets(line,512,conf_file) != NULL)
	{
		if( (!(memcmp(line,"#",1) == 0)) && (!(isspace(line[0]))) )
		{
			char *name = NULL;
			char *value = NULL;

			name = &line[0];
			p = memchr(name,' ',512);
			if(p == NULL)
				continue;
			value = ++p;
			if(value == NULL)
				continue;
			p--;
			*p='\0';
			value[strlen(value)-1] = '\0';

			if(memcmp(name,"num_chars",9) == 0)
			{
				if(!isdigit(value[0]))
					config_reject(name);
				else
					(*config)->num_chars = atoi(value);
				if((*config)->num_chars < 1)
					config_reject(name);
			}
			else if(memcmp(name,"num_passwds",11) == 0)
			{
				if(!isdigit(value[0]))
					config_reject(name);
				else
					(*config)->num_passwds = atoi(value);
				if((*config)->num_passwds < 1)
					config_reject(name);
			}
			else if(memcmp(name,"use_urandom",11) == 0)
			{
				if(!isdigit(value[0]))
					config_reject(name);
				else
					(*config)->use_urandom = atoi(value);
				if(((*config)->use_urandom != 0) && ((*config)->use_urandom != 1))
					config_reject(name);
			}
			else if(memcmp(name,"no_space",8) == 0)
			{
				if(!isdigit(value[0]))
					config_reject(name);
				else
					(*config)->no_space = atoi(value);
				if(((*config)->no_space != 0) && ((*config)->no_space != 1))
					config_reject(name);
			}
			else if(memcmp(name,"passwd_type",11) == 0)
			{
				if(!isdigit(value[0]))
					config_reject(name);
				else
					(*config)->passwd_type = atoi(value);
				if(((*config)->passwd_type > 3) || ((*config)->passwd_type < 0))
					config_reject(name);
			}
			else if(memcmp(name,"homoglyph_suppression_level",26) == 0)
			{
				if(!(isdigit(value[0])))
					config_reject(name);
				else
					(*config)->homoglyph_suppression_level = atoi(value);
				if(((*config)->homoglyph_suppression_level > 2) || ((*config)->homoglyph_suppression_level < 0))
					config_reject(name);
			}
			else if(memcmp(name,"forced_case",14) == 0)
			{
				int forced_case = 0;
				
				if(!isdigit(value[0]))
					config_reject(name);
				else
					forced_case = atoi(value);
				if((forced_case < 0) || (forced_case > 2))
					config_reject(name);
				else
				{
					if(forced_case == 1)
					{
						(*config)->lowercase_only = 1;
					}
					else if(forced_case == 2)
					{
						(*config)->uppercase_only = 1;	
					}
					else
					{
						(*config)->lowercase_only = 0;
						(*config)->uppercase_only = 0;
					}
				}
			}
			else
			{
				config_reject(name);
			}
		}
	}
	fclose(conf_file);

	return 0;
}


void config_reject(char *name)
{
	fprintf(stderr,"passgen.conf:  Bad value, rejecting config var: %s\n",name);
}


int file_exists(char *path)
{
	int rslt = 0;
	struct stat tmp_stats;


	rslt = lstat(path, &tmp_stats);
	if(rslt != -1)
	{
		return 1;
	}

	return 0;
}
