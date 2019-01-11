/* FIXME: Add stuff to this! */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

#include "actuators.h"

/* needed for linking w/ builtins */
void pn_fatal_error (const char *fmt, ...) {}
void pn_error (const char *fmt, ...) {}
void pn_quit (void) {}

int
main (int argc, char **argv)
{
  int i, j;
  struct pn_actuator_desc *a;

  for (i=1; i<argc; i++)
    {
      if (*argv[i] == '-')
	{
	  switch (argv[i][1])
	    {
	    case 'l':
	      for (j=0; builtin_table[j]; j++)
		printf ("%s\n", builtin_table[j]->name);
	      break;
	    }
	  continue;
	}

      /* find the actuator */
      a = get_actuator_desc (argv[i]);
      if (! a)
	{
	  fprintf (stderr, "Unknown actuator: %s\n", argv[i]);
	  continue;
	}

      printf ("-------------------------------------\n");
      printf ("%s\n\n%s\n\n", a->name, a->doc);

      if (a->option_descs)
	{
	  printf ("Available options:\n");
	  for (j=0; a->option_descs[j].name; j++)
	    {
	      printf ("%s [", a->option_descs[j].name);
	      switch (a->option_descs[j].type)
		{
		case OPT_TYPE_INT:
		  printf ("INT");
		  break;
		case OPT_TYPE_FLOAT:
		  printf ("FLOAT");
		  break;
		case OPT_TYPE_STRING:
		  printf ("STRING");
		  break;
		case OPT_TYPE_COLOR:
		  printf ("COLOR");
		  break;
		case OPT_TYPE_COLOR_INDEX:
		  printf ("COLOR INDEX");
		  break;
		case OPT_TYPE_BOOLEAN:
		  printf ("BOOLEAN");
		  break;
		}
	      printf ("]: %s\n", a->option_descs[j].doc);
	    }
	}
      else
	printf ("No options available\n");
    }

  return 0;
}
