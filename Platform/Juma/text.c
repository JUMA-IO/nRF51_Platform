#include "gfx.h"

void gfx_set_font(const gfx_font_t* font)
{
    gfx_current_font = font;
}

uint16_t gfx_draw_char(uint16_t x, uint16_t y, char c, uint8_t ppo)
{
    uint8_t i;
    uint8_t var_width;
    uint8_t bytes_high;
    uint8_t bytes_per_char;
    const uint8_t *p;

	if (c < gfx_current_font->start_char || c > gfx_current_font->end_char) {
        return gfx_current_font->space;
	}

    if ((gfx_current_font->height % 8) > 0){
        bytes_high = (gfx_current_font->height / 8) + 1;
    }
    else{
        bytes_high = (gfx_current_font->height / 8);
    }

    bytes_per_char = gfx_current_font->width * bytes_high + 1; /* The +1 is the width byte at the start */

    p = gfx_current_font->font_table + (c - gfx_current_font->start_char) * bytes_per_char;

    /* The first byte per character is always the width of the character */
    var_width = *p;

    /* Font table in MikroElecktronica format
       - multi row fonts allowed (more than 8 pixels high)
       - variable width fonts allowed
       a complete column is written before moving to the next */


    p++; /* Step over the variable width field */

    for ( i = 0; i < var_width; i++ ) {
        uint8_t j;
        for ( j = 0; j < bytes_high; j++ ) {
            uint8_t dat = *( p + i*bytes_high + j );
            uint8_t bit;
            for (bit = 0; bit < 8; bit++) {
#if 0
                if (x+i >= gfx_screen_width || y+j*8+bit >= gfx_screen_height) {
                    /* Don't write past the dimensions of the LCD, skip the entire char */
                    return 0;
                }

                /* We should not write if the y bit exceeds font height */
                if ((j*8 + bit) >= gfx_current_font->height) {
                    /* Skip the bit */
                    continue;
                }
#endif
                if (dat & (1<<bit)) {
                    gfx_draw_pixel(x+i,y+j*8+bit,ppo);
                }
            }									
        }				
    }
    return var_width;
}

void gfx_draw_string(uint16_t x, uint16_t y, char *str, uint8_t ppo)
{
	uint8_t width;

//	if (y > (gfx_screen_height - gfx_current_font->height - 1)) {
//		/* Character won't fit */
//		return;
//	}

	while (*str) {
		width = gfx_draw_char(x,y,*str,ppo);
		x += (width + 1);
		str++;
	}		
}
