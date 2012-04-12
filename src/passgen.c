/*  Author:		V. Alex Brennen (vab@mit.edu)	*/
/*  Copyright:		None				*/
/*  License:		Public Domain			*/
/*  Created:		2006.09.15			*/
/*  Last Updated:	2011.06.18			*/

#include "passgen.h"


/* This is a simple program to read some data from /dev/random or
 * /dev/urandom and map it to a set of ASCII characters. That group
 * of pseudorandom characters can then be used as a password.
 */

int main(int argc,char *argv[])
{
	const char version[] = "1.2.0";

	int	rslt = 0;
	int	arg = 0;
	int	n = 0;


	/* Init the config, then read config file */
	struct passgen_config *config = (struct passgen_config *)malloc(sizeof(struct passgen_config));
	if(config == NULL)
	{
		fprintf(stderr,"Failed to malloc config struct.\n");
		fprintf(stderr,"Perhaps, the system is out of memory?\n");
		fprintf(stderr,"Exiting...\n\n");

		return -1;
	}
	rslt = init_passgen_config(&config);
	if(rslt != 0)
	{
		fprintf(stderr,"Warning!  Failed to initialize passgen config.\n");
		fprintf(stderr,"Perhaps, the system is experiencing memory problems?\n");
		fprintf(stderr,"Exiting...\n\n");

		return -1;
	}
	if(argc > 1)
	{
		for(arg = 1; arg < argc; arg++)
		{
			if(argv[arg][0] == '-')
			{
				if(argv[arg][1] == 'C')
				{
					config->suppress_config_warning = 1;
					
					break;
				}
				else if(argv[arg][1] == 'h')
				{
					print_help();
					if(config != NULL)
						free(config);

					return 0;
				}
				else if(argv[arg][1] == 'v')
				{
					printf("CryptNET Password Generator Version %s\n", version);
					if(config != NULL)
						free(config);

					return 0;
				}
				else if(argv[arg][1] == '-')
				{
					if(strstr(argv[arg],"help") != NULL)
					{
						print_help();
						if(config != NULL)
							free(config);

						return 0;
					}
					else if(strstr(argv[arg],"version") != NULL)
					{
						printf("CryptNET Password Generator Version %s\n", version);
						if(config != NULL)
							free(config);

						return 0;
					}
				}
			}
		}
	}
	rslt = read_passgen_config(&config);
	if((rslt != 0) && (config->suppress_config_warning == 0))
	{
		fprintf(stderr,"Password candidates may not match desired criteria.\n");
		fprintf(stderr,"Continuing...\n\n");
	}

	if(argc > 1)
	{
		int conf_lowercase_set = 0;
		int conf_uppercase_set = 0;
		int conf_urandom_set = 0;
		int conf_random_set = 0;


		for(arg = 1; arg < argc; arg++)
		{
			if(argv[arg][0] == '-')
			{
				if(argv[arg][1] == 'a')
				{
					config->passwd_type = D_ALPHA_NUMERIC;
				}
				else if(argv[arg][1] == 'A')
				{
					config->passwd_type = D_ALPHA_ONLY;
				}
				else if(argv[arg][1] == 'C')
				{
					continue;
				}
				else if(argv[arg][1] == 'H')
				{
					n = arg;
					n++;
					if(((argv[n] != NULL) && isdigit(argv[n][0])))				
					{
						config->homoglyph_suppression_level = atoi(argv[n]);
						if((config->homoglyph_suppression_level > 2) ||
							(config->homoglyph_suppression_level < 0))
						{
							invalid_argument();
							fprintf(stderr,"Configuration Error: Homoglyph suppression levels must be between 0 and 2.\n");
							if(config != NULL)
								free(config);

							return 0;
						}
					}
					else
					{
						invalid_argument();
						fprintf(stderr,"Configuration Error: Proper format is -n x, where x in an integer.\n");
						if(config != NULL)
							free(config);

						return 0;
					}
				}
				else if(argv[arg][1] == 'l')
				{
					/* Read in info on how many chars are wanted */
					n = arg;
					n++;
					if((argv[n] != NULL) && (isdigit(argv[n][0])))
					{
						config->num_chars = atoi(argv[n]);
					}
					else
					{
						invalid_argument();
						fprintf(stderr,"Configuration Error: Proper format is -l x, where x in an integer.\n");
						if(config != NULL)
							free(config);

						return 0;
					}
				}
				else if(argv[arg][1] == 'L')
				{
					conf_lowercase_set = 1;
					if(conf_uppercase_set == 1)
					{
						invalid_argument();
						fprintf(stderr,"Configuration Error: Lowercase only and uppercase only options are mutually exclusive.\n");
						if(config != NULL)
							free(config);
		
						return 0;
					}
					config->lowercase_only = 1;
				}
				else if(argv[arg][1] == 'n')
				{
					/* Read in info on how many chars are wanted */
					n = arg;
					n++;
					if(((argv[n] != NULL) && isdigit(argv[n][0])))				
					{
						config->num_passwds = atoi(argv[n]);
					}
					else
					{
						invalid_argument();
						fprintf(stderr, "Configuration Error: Proper format is -n x, where x in an integer.\n");
						if(config != NULL)
							free(config);

						return 0;
					}
				}
				else if(argv[arg][1] == 'N')
				{
					config->passwd_type = D_NUMERIC_ONLY;
				}
				else if(argv[arg][1] == 'r')
				{
					conf_random_set = 1;
					if(conf_urandom_set == 1)
					{
						invalid_argument();
						fprintf(stderr,"Configuration Error: Options to use /dev/random and /dev/urandom are mutually exclusive.\n");
						if(config != NULL)
							free(config);

						return 0;
					}
					config->use_urandom = 0;
				}
				else if(argv[arg][1] == 's')
				{
					config->no_space = 1;
				}
				else if(argv[arg][1] == 'u')
				{
					conf_urandom_set = 1;
					if(conf_random_set == 1)
					{
						invalid_argument();
						fprintf(stderr,"Configuration Error: Options to use /dev/random and /dev/urandom are mutually exclusive.\n");
						if(config != NULL)
							free(config);

						return 0;
					}
					config->use_urandom = 1;
				}
				else if(argv[arg][1] == 'U')
				{
					conf_uppercase_set = 1;
					if(conf_lowercase_set == 1)
					{
						invalid_argument();
						fprintf(stderr,"Configuration Error: Lowercase only and uppercase only options are mutually exclusive.\n");
						if(config != NULL)
							free(config);

						return 0;
					}
					config->uppercase_only = 1;
				}
				else if(isdigit(argv[arg][0]))
				{
					continue;
				}
				else
				{
					invalid_argument();
					if(config != NULL)
						free(config);

					return 0;
				}
			}
		}
	}
	rslt = map_data_to_chars(config);
	if(rslt != 0)
	{
		fprintf(stderr,"ERROR:  The char mapping function returned an error.\n");
		fprintf(stderr,"Password generation failed.\n");
		fprintf(stderr,"Exiting.\n\n");
		if(config != NULL)
			free(config);

		return -1;
	}

	if(config != NULL)
		free(config);


	return 0;
}


int map_data_to_chars(struct passgen_config *config)
{
	int lcv = 0;
	int i = 0;
	int rslt = 0;
	int size = 0;
	int array_size = 0;
	unsigned int tmp = 0;
	char c = 0x00;

	FILE 	*pseudorandom = NULL;

	/* There are four different arrays here because having different mapping arrays for the 
	   different major types of passwords will prevent the generator from discarding
	   pseudorandomness with excessive generated character rejections. This matters primarily
	   if we're pulling the pseudorandomness from /dev/random.  But, since /dev/urandom is 
	   also seeded a very small decrease in randomness is possible with excessive discards. */
	static char array[95] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q',
			 'R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h',
			 'i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y',
			 'z','0','1','2','3','4','5','6','7','8','9','~','`','!','@','#','$',
			 '%','^','&','*','(',')','[',']','{','}','/','?','=','+','-','_','|',
			 '\\',';',':','"','\'',',','<','.','>',' '};

	char alpha_numeric_array[62] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q',
			 'R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h',
			 'i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y',
			 'z','0','1','2','3','4','5','6','7','8','9'};

	char alpha_array[52] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q',
			 'R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h',
			 'i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y',
			 'z'};

	char numeric_array[10] = {'0','1','2','3','4','5','6','7','8','9'};

	char homoglyph_lvl1_suppress[4] = {'0','O','1','l'};

	char homoglyph_lvl2_suppress[9] = {'0','O','1','l','\'','`','"',' ','_'};

	char buff = 0x00;
	char *passwd = NULL;


	size = config->num_chars;

	/* Here array_size is array *address size*.  Arrays size is one more than array address size
	   since addresses start at zero in the C language.
	*/
	if(config->passwd_type == D_DEFAULT)
	{
		array_size = 94;
	}
	else if(config->passwd_type == D_ALPHA_NUMERIC)
	{
		array_size = 61;
	}
	else if(config->passwd_type == D_ALPHA_ONLY)
	{
		array_size = 51;
	}
	else if(config->passwd_type == D_NUMERIC_ONLY)
	{
		array_size = 9;
	}
	else
	{
		fprintf(stderr,"passgen:  Error unknown password type requested: %d.\n", config->passwd_type);

		return -1;
	}
	
	if(config->use_urandom == 0)
	{
		if(NULL == (pseudorandom = fopen("/dev/random","r")))
		{
			fprintf(stderr,"ERROR:  Failed to open /dev/random!\n");
			fprintf(stderr,"Exiting...\n\n");

			return 0;
		}
	}
	else
	{
		if(NULL == (pseudorandom = fopen("/dev/urandom","r")))
		{
			fprintf(stderr,"ERROR:  Failed to open /dev/urandom!\n");
			fprintf(stderr,"Exiting...\n\n");

			return 0;
		}
	}

	passwd = malloc(size+1);
	if(passwd == NULL)
	{
		printf("Failed to allocate memory.\n");
		fclose(pseudorandom);

		return -1;
	}

	for(i = 0;i < config->num_passwds; i++)
	{
		lcv = 0;
		memset(passwd,0x00,size+1);
		
		while(lcv < size)
		{
			buff = 0x00;
	
			rslt = fread(&buff,1,1,pseudorandom);
			if(rslt < 0)
			{
				fprintf(stderr,"ERROR:  Read of data from specified device failed.\n");
				fclose(pseudorandom);
				free(passwd);

				return -1;
			}

			tmp = buff;
			if(tmp > array_size)
			{
				tmp = tmp % array_size;
			}

			if((config->passwd_type == D_DEFAULT))
			{
				c = array[tmp];
			}
			if(config->passwd_type == D_ALPHA_NUMERIC)
			{
				c = alpha_numeric_array[tmp];
			}
			else if(config->passwd_type == D_ALPHA_ONLY)
			{
				c = alpha_array[tmp];
			}
			else if(config->passwd_type == D_NUMERIC_ONLY)
			{
				c = numeric_array[tmp];
			}
			else
			{
				c = array[tmp];
			}
			
			/* If we're excluding spaces and the generated password character is space generate another character */
			if((config->no_space == 1) && (c == ' '))
			{
				continue;
			}

			/* If we're suppressing homoglyphs and one comes up, read another byte of randomness
			 * and map it to the array */
			if(config->homoglyph_suppression_level > 0)
			{
				if(config->homoglyph_suppression_level == 1)
				{
					if((memchr(homoglyph_lvl1_suppress,array[tmp],4)) != NULL)
					{
						continue;
					}
				}
				else if(config->homoglyph_suppression_level == 2)
				{
					if((memchr(homoglyph_lvl2_suppress,array[tmp],9)) != NULL)
					{
						continue;
					}
				}
				else
				{
					fprintf(stderr,"There appears to be an error with the homoglyph suppression configuration.\n");
					fclose(pseudorandom);
					free(passwd);
	
					return -1;
				}
			}

			if(config->uppercase_only == 1)
			{
				c = toupper(c);
			}
			else if(config->lowercase_only == 1)
			{
				c = tolower(c);	
			}

			/* This shouldn't be possible, but it is caught just in case. */
			if(c == '\0')
			{
				fclose(pseudorandom);
				free(passwd);

				return -1;
			}
			passwd[lcv] = c;

			lcv++;
		}
		passwd[lcv] = '\0';
		printf("%s\n", passwd);
	}
	fclose(pseudorandom);
	free(passwd);


	return 0;
}


int print_help(void)
{
	printf("Usage: passgen\n");
	printf("\t-a\t\tAlpha numeric characters only.\n");
	printf("\t-A\t\tAlphabetic characters only.\n");
	printf("\t-C\t\tSuppress config file not found messages.\n");
	printf("\t-h\t\tThis help text.\n");
	printf("\t-H\t\tHomoglyph suppression level (0-2).\n");
	printf("\t-l\t\tThe length of the password.\n");
	printf("\t-L\t\tFor any letters, use lowercase only.\n");
	printf("\t-n\t\tNumber of passwords to provide.\n");
	printf("\t-N\t\tNumeric characters only.\n");
	printf("\t-r\t\tUse /dev/random for password generation.\n");
	printf("\t-s\t\tExclude the space character.\n");
	printf("\t-u\t\tUse /dev/urandom rather than /dev/random.\n");
	printf("\t\t\t(Will speed password generation)\n");
	printf("\t-U\t\tFor any letters, user uppercase only.\n");
	printf("\t-v\t\tDisplay version information.\n");
	printf("\t--help\t\tThis help text.\n");
	printf("\t--version\tDisplay version information.\n");
	printf("\n");
	printf("Persisting alternate default configuration values may be set in passgen.conf.\n");
	printf("For for information: `man passgen.conf`\n");
	printf("\n");


	return 0;
}


int invalid_argument(void)
{
	fprintf(stderr,"passgen: Invalid arument given\n\n");

	print_help();


	return 0;
}
