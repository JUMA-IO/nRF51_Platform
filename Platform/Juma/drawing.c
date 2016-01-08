#include "gfx.h"

void gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t ppo)
{
    uint16_t offset;
    uint8_t bit;
    
	if (x > (gfx_screen_width-1) || y > (gfx_screen_height-1)) {
		/* don't do anything if x/y is outside bounds of display size */
		return;
	}

    offset = (x + y * gfx_screen_width) / 8;
    bit = 1 << (7 - x%8);

    if (ppo == GFX_INVERT) {
        gfx_buffer[offset] ^= bit;
    } else {
        if ((ppo == GFX_SET_BLACK) ^ (gfx_flags & GFX_BLACK_IS_1)) {
            gfx_buffer[offset] &= ~ bit;
        } else {
            gfx_buffer[offset] |= bit;
        }
    }

	gfx_update_bbox(x,y,x,y);
}

void gfx_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t ppo) {
	uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
	uint8_t dx, dy;
	int8_t err;
	int8_t ystep;
	
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}
	
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	
	gfx_update_bbox( x0, y0, x1, y1 );

	dx = x1 - x0;
	dy = abs(y1 - y0);
	
	err = dx / 2;
	
	if (y0 < y1) {
		ystep = 1;

	} else {
		ystep = -1;
	}
	
	for (; x0<=x1; x0++) {
		if (steep) {
			gfx_draw_pixel(y0, x0, ppo);
		} else {
			gfx_draw_pixel(x0, y0, ppo);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void gfx_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t ppo)
{
	int16_t i;
	for (i=x; i<x+w; i++) {
		int16_t j;
		for (j=y; j<y+h; j++) {
			gfx_draw_pixel(i, j, ppo);
		}
	}
	gfx_update_bbox(x, y, x+w-1, y+h-1);
}

void gfx_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t ppo)
{
	int16_t i;
	for (i=x; i<x+w; i++) {
		gfx_draw_pixel(i, y, ppo);
		gfx_draw_pixel(i, y+h-1, ppo);
	}
	for (i=y; i<y+h; i++) {
		gfx_draw_pixel(x, i, ppo);
		gfx_draw_pixel(x+w-1, i, ppo);
	} 
	gfx_update_bbox(x, y, x+w-1, y+h-1);
}

void gfx_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t ppo)
{
		
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;
	
	gfx_update_bbox(x0-r, y0-r, x0+r, y0+r);
	
	gfx_draw_pixel(x0, y0+r, ppo);
	gfx_draw_pixel(x0, y0-r, ppo);
	gfx_draw_pixel(x0+r, y0, ppo);
	gfx_draw_pixel(x0-r, y0, ppo);
	
	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		
		gfx_draw_pixel(x0 + x, y0 + y, ppo);
		gfx_draw_pixel(x0 - x, y0 + y, ppo);
		gfx_draw_pixel(x0 + x, y0 - y, ppo);
		gfx_draw_pixel(x0 - x, y0 - y, ppo);
		
		gfx_draw_pixel(x0 + y, y0 + x, ppo);
		gfx_draw_pixel(x0 - y, y0 + x, ppo);
		gfx_draw_pixel(x0 + y, y0 - x, ppo);
		gfx_draw_pixel(x0 - y, y0 - x, ppo);
		
	}
}

void gfx_fill_circle(uint16_t x0, uint16_t y0, uint16_t r, uint8_t ppo)
{
	
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;
	
	int16_t i;

	gfx_update_bbox(x0-r, y0-r, x0+r, y0+r);
	
	for (i=y0-r; i<=y0+r; i++) {
		gfx_draw_pixel(x0, i, ppo);
	}
	
	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		
		for (i=y0-y; i<=y0+y; i++) {
			gfx_draw_pixel(x0+x, i, ppo);
			gfx_draw_pixel(x0-x, i, ppo);
		} 
		for (i=y0-x; i<=y0+x; i++) {
			gfx_draw_pixel(x0+y, i, ppo);
			gfx_draw_pixel(x0-y, i, ppo);
		}    
	}
}

void gfx_draw_image(uint16_t x, uint16_t y, const uint8_t* image, uint16_t width, uint16_t height, uint8_t ppo)
{
    uint16_t offset;
    uint8_t bit;
	int16_t i;
    
	for (i=0; i<width; i++) {
		int16_t j;
		for (j=0; j<height; j++) {
            offset = (i + j * width) / 8;
            bit = 1 << (i%8);

            if (image[offset] & bit) {
                gfx_draw_pixel(x+i, y+j, ppo);
            }
		}
	}
	gfx_update_bbox(x, y, x+width-1, y+height-1);
}
