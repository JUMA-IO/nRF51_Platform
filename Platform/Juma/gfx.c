#include <string.h>
#include <stdio.h>
#include "gfx.h"

uint8_t* gfx_buffer;
gfx_bbox_t gfx_bbox;
uint16_t gfx_screen_width, gfx_screen_height;
uint8_t gfx_flags;
const gfx_font_t* gfx_current_font;

void gfx_init_screen(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t flags)
{
    gfx_buffer = buffer;
    gfx_screen_width = width;
    gfx_screen_height = height;
    gfx_flags = flags;
}

void gfx_update_bbox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax)
{
	/* Keep and check bounding box within limits of LCD screen dimensions */
	if (xmin > (gfx_screen_width-1)) {
		xmin = gfx_screen_width-1;
	}
	if (xmax > (gfx_screen_width-1)) {
		xmax = gfx_screen_width-1;
	}

	if (ymin > (gfx_screen_height-1)) {
		ymin = gfx_screen_height-1;
	}
	if (ymax > (gfx_screen_height-1)) {
		ymax = gfx_screen_height-1;
	}

	/* Update the bounding box size */
	if (xmin < gfx_bbox.x_min) {
		gfx_bbox.x_min = xmin;
	}		
	if (xmax > gfx_bbox.x_max) {
		gfx_bbox.x_max = xmax;
	}
	if (ymin < gfx_bbox.y_min) {
		gfx_bbox.y_min = ymin;
	}		
	if (ymax > gfx_bbox.y_max) {
		gfx_bbox.y_max = ymax;
	}			
}

void gfx_reset_bbox()
{
	/* Used after physically writing to the LCD */
	gfx_bbox.x_min = gfx_screen_width - 1;
	gfx_bbox.x_max = 0;
	gfx_bbox.y_min = gfx_screen_height -1;
	gfx_bbox.y_max = 0;	
}

void gfx_clear(void)
{
    if (gfx_flags & GFX_BLACK_IS_1) {
        memset(gfx_buffer, 0x00, gfx_screen_width * gfx_screen_height / 8);
    } else {
        memset(gfx_buffer, 0xff, gfx_screen_width * gfx_screen_height / 8);
    }
}
