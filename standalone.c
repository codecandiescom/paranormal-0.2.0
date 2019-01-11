/* FIXME: This should eventually be a full-fledged standalone
   program... now it's just a tiny thing for debugging/profiling */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <glib.h>

#include "paranormal.h"
#include "actuators.h"
#include "presets.h"

static struct pn_actuator *actuator;

void
pn_set_rc (struct pn_rc *new_rc)
{
  /* FIXME: Write this! */
}

void
pn_error (const char *fmt, ...)
{
  va_list ap;

  va_start (ap, fmt);
  vfprintf (stderr, fmt, ap);
  va_end (ap);
  fprintf (stderr, "\n");
}

void
pn_fatal_error (const char *fmt, ...)
{
  va_list ap;

  va_start (ap, fmt);
  vfprintf (stderr, fmt, ap);
  va_end (ap);
  fprintf (stderr, "\n");

  pn_cleanup ();
  if (actuator)
    destroy_actuator (actuator);

  exit (-1);
}

void
pn_quit (void)
{
  pn_cleanup ();
  if (actuator)
    destroy_actuator (actuator);
  exit (0);
}

static void
generate_pcm_data (void)
{
  int j, i;

  for (j=0; j<2; j++)
    for (i=0; i<512; i++)
      pn_sound_data->pcm_data[j][i] = -5000.0 + (gint16) (10000.0 * rand ()/(RAND_MAX+1.0));
}

static void
generate_freq_data (void)
{
  int j, i;

  for (j=0; j<2; j++)
    for (i=0; i<256; i++)
      pn_sound_data->freq_data[j][i] = (gint16) (250.0 * rand ()/(RAND_MAX+1.0));
}

static void
load_pn_rc (void)
{
  if (! pn_rc)
    {
      pn_rc = g_new (struct pn_rc, 1);
      pn_rc->actuator = load_preset ("test.pna");
      if (! pn_rc->actuator)
	pn_error ("Unable to load the test actuator \"test.pna\"");
    }
}

int
main (int argc, char **argv)
{
  actuator = load_preset ("test.pna");
  if (! actuator)
    pn_fatal_error ("Unable to load the test actuator \"test.pna\"");

  load_pn_rc ();

  pn_init ();

  for (;;)
    {
      generate_pcm_data ();
      generate_freq_data ();
      pn_render ();
    }

  pn_cleanup ();
}
