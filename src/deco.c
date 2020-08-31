/*
 * Helpers for handling window decorations
 *
 * Copyright Johan Malm 2020
 */

#include "labwc.h"
#include "theme/theme.h"
#include "config/rcxml.h"
#include "common/bug-on.h"
#include "common/log.h"

#define BORDER_WIDTH (1)

struct wlr_box deco_max_extents(struct view *view)
{
	struct wlr_box box = {
		.x = view->x - BORDER_WIDTH,
		.y = view->y - rc.title_height - BORDER_WIDTH,
		.width = view->w + 2 * BORDER_WIDTH,
		.height = view->h + rc.title_height + 2 * BORDER_WIDTH,
	};
	return box;
}

struct wlr_box deco_box(struct view *view, enum deco_part deco_part)
{
	int margin;

	struct wlr_box box = { .x = 0, .y = 0, .width = 0, .height = 0 };
	BUG_ON(!view);
	BUG_ON(!view->surface);
	if (!view || !view->surface)
		return box;
	BUG_ON(!view->been_mapped);
	BUG_ON(!view->show_server_side_deco);
	if ((view->w < 1) || (view->h < 1)) {
		warn("view (%p) has no width/height", view);
		return box;
	}
	switch (deco_part) {
	case LAB_DECO_BUTTON_CLOSE:
		wlr_texture_get_size(theme.xbm_close_active_unpressed,
				     &box.width, &box.height);
		margin = (rc.title_height - box.height) / 2;
		box.x = view->x + view->w + margin - rc.title_height;
		box.y = view->y - rc.title_height + margin;
		break;
	case LAB_DECO_BUTTON_MAXIMIZE:
		wlr_texture_get_size(theme.xbm_maximize_active_unpressed,
				     &box.width, &box.height);
		margin = (rc.title_height - box.height) / 2;
		box.x = view->x + view->w + margin - rc.title_height * 2;
		box.y = view->y - rc.title_height + margin;
		break;
	case LAB_DECO_BUTTON_ICONIFY:
		wlr_texture_get_size(theme.xbm_iconify_active_unpressed,
				     &box.width, &box.height);
		margin = (rc.title_height - box.height) / 2;
		box.x = view->x + view->w + margin - rc.title_height * 3;
		box.y = view->y - rc.title_height + margin;
		break;
	case LAB_DECO_PART_TITLE:
		box.x = view->x;
		box.y = view->y - rc.title_height;
		box.width = view->w;
		box.height = rc.title_height;
		break;
	case LAB_DECO_PART_TOP:
		box.x = view->x - BORDER_WIDTH;
		box.y = view->y - rc.title_height - BORDER_WIDTH;
		box.width = view->w + 2 * BORDER_WIDTH;
		box.height = BORDER_WIDTH;
		break;
	case LAB_DECO_PART_RIGHT:
		box.x = view->x + view->w;
		box.y = view->y - rc.title_height;
		box.width = BORDER_WIDTH;
		box.height = view->h + rc.title_height;
		break;
	case LAB_DECO_PART_BOTTOM:
		box.x = view->x - BORDER_WIDTH;
		box.y = view->y + view->h;
		box.width = view->w + 2 * BORDER_WIDTH;
		box.height = +BORDER_WIDTH;
		break;
	case LAB_DECO_PART_LEFT:
		box.x = view->x - BORDER_WIDTH;
		box.y = view->y - rc.title_height;
		box.width = BORDER_WIDTH;
		box.height = view->h + rc.title_height;
		break;
	default:
		break;
	}
	return box;
}

enum deco_part deco_at(struct view *view, double lx, double ly)
{
	enum deco_part deco_part;
	for (deco_part = 0; deco_part < LAB_DECO_NONE; ++deco_part) {
		struct wlr_box box = deco_box(view, deco_part);
		if (wlr_box_contains_point(&box, lx, ly))
			return deco_part;
	}
	return LAB_DECO_NONE;
}
