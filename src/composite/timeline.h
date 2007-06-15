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

#ifndef _TIMELINE_H_
#define _TIMELINE_H_

#include "structs.h"
#include "list.h"
void
wm_set_timelines_fps (Wm *wm, int fps);

void
wm_run_timelines (Wm *wm);

MBTimeline*
mb_timeline_new (int n_frames,  MBTimelineFunc func, void *userdata);

void
mb_timeline_stop (Wm *wm, MBTimeline *timeline);

void
mb_timeline_start (Wm *wm, MBTimeline *timeline);

#endif
