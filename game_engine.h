/*
	This file is part of Manic Pietro.

	Manic Pietro is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Manic Pietro is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Manic Pietro.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "globals.h"
extern void          game_draw_map(void);
extern void          game_fps(void);
extern void          game_cls(void);
extern void          game_fill_row(unsigned char f_row, unsigned char f_asc) __z88dk_callee;
extern void			     game_update_stats(void);
extern void			     game_start_timer(void);
extern void			     game_round_init(void);
extern void			     game_loop(void);
extern unsigned char game_check_map(unsigned char f_lin, unsigned char f_col);
extern unsigned char game_check_cell(unsigned int *f_index) __z88dk_fastcall;
extern void          game_cell_paint();
extern void          game_cell_paint_index();
extern void			     game_print_score(void);
extern void          game_paint_attrib( unsigned char *f_attrib[], char f_start, unsigned char f_end, unsigned char f_lin);
extern void			     game_colour_message( unsigned char f_row, unsigned char f_col, unsigned char f_col2, unsigned int f_miliseconds, unsigned char skip);
extern void			     game_key_paint(void);
extern unsigned char game_check_time(unsigned int *start, unsigned int lapse);
extern void          game_attribs(void);
extern void          game_page_map(void);
extern void          game_flash_exit(unsigned char f_attrib);
//TODO MOVE TO SPRITES
extern unsigned char game_copy_sprite_std(unsigned char f_hi_sprite, unsigned char f_low_sprite);
void                 game_copy_sprite_color_reset(void);
void                 game_copy_sprite(unsigned char f_hi_sprite, unsigned char f_low_sprite, unsigned char f_flip);
extern void          game_intro( );
extern void          game_shoe( );
extern void          game_anim_air(void);
extern void          game_solar_ray(void);
extern void          game_solar_ray1();
extern void          game_solar_ray0();
extern void          game_crumble();
//extern void          game_set_attr(unsigned char f_lin, unsigned char f_col, unsigned char f_attrib );
extern void          page( unsigned char bank );
extern unsigned char reverse(unsigned char b);
extern void          zx_print_char(unsigned char ui_row, unsigned char ui_col, unsigned char c);
extern void          game_anim_conveyor();

extern void          game_text( unsigned char f_index, unsigned char f_row );
extern void          game_intro_willy();
extern void          game_end_willy();
extern void          game_end_pietro();
extern void          game_img( unsigned char *f_img, unsigned char f_page,
                               unsigned char f_lin, unsigned char f_col,
                              unsigned char f_width, unsigned char f_height) ;
#endif
