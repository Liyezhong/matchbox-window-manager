/* 
 *  Matchbox Window Manager - A lightweight window manager not for the
 *                            desktop.
 *
 *  Authored By Matthew Allum <mallum@o-hand.com>
 *
 *  Copyright (c) 2002, 2004 OpenedHand Ltd - http://o-hand.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

#include "timeline.h"

void
wm_set_timelines_fps (Wm *wm, int fps)
{
  wm->timeline_interval_msecs = 1000000 / fps;
}

void
wm_run_timelines (Wm *wm)
{
  MBList *item = wm->timelines, *next;
  struct timeval tv;

  if (item == NULL)
    return;

  gettimeofday (&tv, NULL);

  while (item != NULL)
    {
      MBTimeline *tl = (MBTimeline*)item->data;
      int         n_frames = 1;

      next = item->next;

      if (tl->frame_num > tl->n_frames)
	dbg("%s() Timeline should be stopped!", __func__);

      if (tl->Last_called_msecs)
	{
	  dbg("%s() %li , %li, %li %li\n", __func__, 
	      (tv.tv_usec - tl->Last_called_msecs),
	      tv.tv_usec,
	      tl->Last_called_msecs,
	      wm->timeline_interval_msecs);
		 
	  if ((tv.tv_usec - tl->Last_called_msecs) 
	                         > wm->timeline_interval_msecs)
	    n_frames = (tv.tv_usec - tl->Last_called_msecs) 
       	                    / wm->timeline_interval_msecs;
	}


      dbg("%s() advance %i frames\n", __func__,n_frames);

      tl->Last_called_msecs = tv.tv_usec;

      tl->frame_num += n_frames;

      if (tl->frame_num > tl->n_frames)
	tl->frame_num = tl->n_frames;

      if (tl->func)
	tl->func (wm, tl->n_frames,  tl->frame_num, tl->func_data);

      /* Dont touch timeline anymore as callback may have removed it */

      item = next;
    }
}

MBTimeline*
mb_timeline_new (int n_frames,  MBTimelineFunc func, void *userdata)
{
  MBTimeline *tl;

  tl = malloc (sizeof(MBTimeline));
  memset (tl, sizeof(MBTimeline), 0);

  tl->n_frames  = n_frames;
  tl->func      = func;
  tl->func_data = userdata;

  return tl;
}

void
mb_timeline_stop (Wm *wm, MBTimeline *timeline)
{
  list_remove(&wm->timelines, (void *)timeline);
  /* FIXME: free the timeline ? */
}

void
mb_timeline_start (Wm *wm, MBTimeline *timeline)
{
  list_add(&wm->timelines, NULL, 0, (void *)timeline);

  timeline->frame_num = 1;

  if (timeline->func)
    timeline->func (wm, timeline->n_frames, 
	      timeline->frame_num, timeline->func_data);

}

