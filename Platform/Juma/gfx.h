#ifndef _GFX_H
#define _GFX_H

#include "sys_common.h"

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

typedef struct {
	uint8_t x_min;
	uint8_t y_min;
	uint8_t x_max;
	uint8_t y_max;
} gfx_bbox_t;

extern uint16_t gfx_screen_width;
extern uint16_t gfx_screen_height;
extern uint8_t gfx_flags;

/* Global variables used for GFX library */
extern uint8_t* gfx_buffer;
extern gfx_bbox_t gfx_bbox;

void gfx_update_bbox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax);
void gfx_reset_bbox(void);
void gfx_clear_buffer(void);

extern const gfx_font_t* gfx_current_font;

#endif
