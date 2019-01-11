/* FIXME: add fullscreen / screenshots */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

#include <SDL.h>

#include "paranormal.h"
#include "actuators.h"

/* SDL stuffs */
static SDL_Surface *screen;

/* Globals */
struct pn_rc         *pn_rc;
struct pn_image_data *pn_image_data;
struct pn_sound_data *pn_sound_data;

/* Trig Pre-Computes */
float sin_val[360];
float cos_val[360];

/* **************** drawing doodads **************** */

static void
blit_to_screen (void)
{
  int j;

  SDL_LockSurface (screen);

  /* FIXME: add scaling support */

  SDL_SetPalette (screen, SDL_LOGPAL|SDL_PHYSPAL,
		  (SDL_Color*)pn_image_data->cmap, 0, 256);

  for (j=0; j<pn_image_data->height; j++)
      memcpy (screen->pixels + j*screen->pitch,
	      pn_image_data->surface[0] + j*pn_image_data->width,
	      pn_image_data->width);


  SDL_UnlockSurface (screen);

  SDL_UpdateRect (screen, 0, 0, 0, 0);
}

static void
resize_video (guint w, guint h)
{
  pn_image_data->width = w;
  pn_image_data->height = h;

  if (pn_image_data->surface[0])
    g_free (pn_image_data->surface[0]);
  if (pn_image_data->surface[1])
    g_free (pn_image_data->surface[1]);

  pn_image_data->surface[0] = g_malloc0 (w * h);
  pn_image_data->surface[1] = g_malloc0 (w * h);

  screen = SDL_SetVideoMode (w, h, 8, SDL_HWSURFACE |
			     SDL_HWPALETTE | SDL_RESIZABLE);
  if (! screen)
    pn_fatal_error ("Unable to create a new SDL window: %s",
		    SDL_GetError ());
}

static void
take_screenshot (void)
{
  char fname[32];
  struct stat buf;
  int i=0;

  do
    sprintf (fname, "pn_%05d.bmp", ++i);
  while (stat (fname, &buf) == 0);

  SDL_SaveBMP (screen, fname);
}

/* FIXME: This should resize the video to a user-set
   fullscreen res */
static void
toggle_fullscreen (void)
{
  SDL_WM_ToggleFullScreen (screen);  
  if (SDL_ShowCursor (SDL_QUERY) == SDL_ENABLE)
    SDL_ShowCursor (SDL_DISABLE);
  else
    SDL_ShowCursor (SDL_ENABLE);
}

/* **************** basic renderer management **************** */
void
pn_init (void)
{
  int i;

  pn_sound_data = g_new0 (struct pn_sound_data, 1);
  pn_image_data = g_new0 (struct pn_image_data, 1);

  if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) < 0)
    pn_fatal_error ("Unable to initialize SDL: %s", SDL_GetError ());

  resize_video (640, 360);

  SDL_WM_SetCaption (PACKAGE " " VERSION, PACKAGE);

  for(i=0; i<360; i++)
    {
      sin_val[i] = sin(i*(M_PI/180.0));
      cos_val[i] = cos(i*(M_PI/180.0));
    }
}

void
pn_cleanup (void)
{
  SDL_FreeSurface (screen);
  SDL_Quit ();


  if (pn_image_data)
    {
      if (pn_image_data->surface[0])
	g_free (pn_image_data->surface[0]);
      if (pn_image_data->surface[1])
	g_free (pn_image_data->surface[1]);
      g_free (pn_image_data);
    }
  if (pn_sound_data)
    g_free (pn_sound_data);
}

/* Renders one frame and handles the SDL window */
void
pn_render (void)
{
  SDL_Event event;

  /* Handle window events */
  while (SDL_PollEvent (&event))
    {
      switch (event.type)
	{
	case SDL_QUIT:
	  pn_quit ();
	  g_assert_not_reached ();
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_ESCAPE:
	      pn_quit ();
	      g_assert_not_reached ();
	    case SDLK_RETURN:
	      if (event.key.keysym.mod & (KMOD_ALT | KMOD_META))
		toggle_fullscreen ();
	      break;
	    case SDLK_BACKQUOTE:
	      take_screenshot ();
	      break;
	    default:
	    }
	  break;
	case SDL_VIDEORESIZE:
	  resize_video (event.resize.w, event.resize.h);	  
	  break;
	}
    }

  if (pn_rc->actuator)
    {
      exec_actuator (pn_rc->actuator);
      blit_to_screen ();
    }
}

/* this MUST be called if a builtin's output is to surface[1]
   (by the builtin, after it is done) */
void
pn_swap_surfaces (void)
{
  guchar *tmp = pn_image_data->surface[0];
  pn_image_data->surface[0] = pn_image_data->surface[1];
  pn_image_data->surface[1] = tmp;
}










