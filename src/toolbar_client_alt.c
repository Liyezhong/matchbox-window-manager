/* matchbox - a lightweight window manager

   Copyright 2002 Matthew Allum

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
*/

/* Toolbar windows are small collapsable 'panel' like windows at bottom 
 * of display. They are mainly to hold input methods like software keyboards
 * stroke recognisers etc. 
 */

#include "toolbar_client.h"

#ifdef USE_ALT_INPUT_WIN

Client*
toolbar_client_new(Wm *w, Window win)
{
  Window trans_win;
  Client *c = base_client_new(w, win), *trans_client = NULL; 

  if (!c) return NULL;

  c->type = MBCLIENT_TYPE_DIALOG; 
   
  c->configure    = &toolbar_client_configure;
  c->reparent     = &toolbar_client_reparent;
  c->hide         = &toolbar_client_hide;
  c->iconize      = &toolbar_client_hide;
  c->show         = &toolbar_client_show;
  c->move_resize  = &toolbar_client_move_resize;
  c->destroy      = &toolbar_client_destroy;

  XGetTransientForHint(w->dpy, win, &trans_win);
   
  if (trans_win && (trans_win != win))
    {
      trans_client = wm_find_client(w, trans_win, WINDOW);

      if (trans_client)
	{
	  if (trans_client->type == MBCLIENT_TYPE_DIALOG)
	    {
	      c->flags |= CLIENT_TB_ALT_TRANS_FOR_DIALOG;
	    }
	  else if (trans_client->type & (MBCLIENT_TYPE_APP|MBCLIENT_TYPE_DESKTOP))
	    {
	      c->flags |= CLIENT_TB_ALT_TRANS_FOR_APP;
	    }
	  else trans_client = NULL;
	}
      
      if (trans_client == NULL)
	{
	  fprintf(stderr, "matchbox: Alternate toolbar window lacks valid transient parent\n");
	  return NULL; 		/* SledgeHammer for now */
	}
    }
   
   return c;
}

int
toolbar_win_offset(Client *c)
{
  return 0;
}

void
toolbar_client_configure(Client *c)
{
  Wm *w = c->wm;

  if (c->flags & CLIENT_IS_MINIMIZED)
    return;

  c->y = w->dpy_height - wm_get_offsets_size(w, SOUTH, c, True) - c->height;
  c->x = wm_get_offsets_size(w, WEST,  NULL, False);
  c->width = w->dpy_width
    - wm_get_offsets_size(w, WEST,  NULL, False)
    - wm_get_offsets_size(w, EAST,  NULL, False);
}

void
toolbar_client_move_resize(Client *c)
{
  Wm *w = c->wm;

  base_client_move_resize(c);

  XResizeWindow(w->dpy, c->window, c->width, c->height);
  XMoveResizeWindow(w->dpy, c->frame, c->x, c->y, c->width, c->height );
}

void
toolbar_client_reparent(Client *c)
{
  Wm *w = c->wm;

  XSetWindowAttributes attr;

  attr.override_redirect = True; 
  attr.background_pixel  = w->grey_col.pixel;
  attr.event_mask        = ChildMask|ButtonPressMask|ExposureMask;
  
  c->frame =
    XCreateWindow(w->dpy, w->root, 0, c->y,
		  w->dpy_width, c->height, 0,
		  CopyFromParent, CopyFromParent, CopyFromParent,
		  CWOverrideRedirect|CWEventMask|CWBackPixel,
		  &attr);
  
  attr.background_pixel = w->grey_col.pixel;
  
  c->title_frame = None;
  
  XSetWindowBorderWidth(w->dpy, c->window, 0);
  XAddToSaveSet(w->dpy, c->window);
  XSelectInput(w->dpy, c->window,
	       ButtonPressMask|ColormapChangeMask|PropertyChangeMask);
  
  dbg("%s() reparenting\n", __func__ );
  
  XReparentWindow(w->dpy, c->window, c->frame, 0, 0);
}


void
toolbar_client_show(Client *c)
{
  Wm   *w = c->wm;
  long win_state; 

  if (!c->mapped)
    {

      XMapSubwindows(w->dpy, c->frame);
      XMapWindow(w->dpy, c->frame);

      if (c->flags & CLIENT_TB_ALT_TRANS_FOR_APP)
	{
	  /* resize the main client were transient for */
	  
	  Client *app_client = c->trans;
	  
	  if (app_client) /* possible crasher here, should check exists */
	    {
	      if (app_client->flags & CLIENT_FULLSCREEN_FLAG)
		{
		  c->y = w->dpy_height - c->height;
		  c->width = w->dpy_width;
		  toolbar_client_move_resize(c);
		}

	      app_client->height -= c->height;
	      app_client->move_resize(app_client);
	      app_client->redraw(app_client, False);
	      /* XXX Send config ? */
	    }
	}
      
    }

  dialog_client_show(c);

  c->mapped = True;

  /* XXX for dialog positioning 

  dialog_constrain_geometry(Client *c,
  int    *req_x,
  int    *req_y,
  int    *req_width,
  int    *req_height)
  */



}

void
toolbar_client_hide(Client *c)
{
  Wm   *w = c->wm;

  if (c->flags & CLIENT_IS_MINIMIZED || client_get_state(c) == IconicState) 
    return;
}

void
toolbar_client_destroy(Client *c)
{
  Wm *w = c->wm;

  dbg("%s() called\n", __func__);
   
  c->mapped = False; /* Setting mapped to false will allow the 
                        dialog resizing/repositioning via restack
                        to ignore use  */

  /* resize the main client were transient for */
  if (c->flags & CLIENT_TB_ALT_TRANS_FOR_APP)
    {
      Client *app_client = c->trans;

      if (app_client) /* possible crasher here, should check still exists */
	{
	  app_client->height += c->height;
	  app_client->move_resize(app_client);
	  app_client->redraw(app_client, False);
	  /* XXX Send config ? */
	}
    }

  if (w->focused_client == c)
    w->focused_client = NULL;

  base_client_destroy(c);     
}


void
toolbar_client_redraw(Client *c, Bool use_cache)
{
  Wm *w = c->wm;


}

#endif /* USE_ALT_INPUT_WIN */



