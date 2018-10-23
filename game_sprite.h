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
#ifndef GAME_SPRITE_H
#define GAME_SPRITE_H
extern unsigned char    spr_chktime(void);
extern unsigned int	    spr_calc_index(unsigned char l, unsigned char c);
extern void             spr_set_up();
extern void             spr_set_down();
extern void             spr_set_left();
extern void             spr_set_right();

extern unsigned char    spr_move_up_f(void);
extern unsigned char    spr_move_down_f(void);
extern void             spr_move_horizontal(void);
extern unsigned char    spr_move_right_f(void);
extern unsigned char    spr_move_left_f(void);
extern unsigned char    spr_move_right(void);
extern unsigned char    spr_move_left(void);
extern void             spr_paint(void);
extern unsigned char    spr_paint_player(void);
extern unsigned char    spr_get_tile(unsigned char *f_sprite) __z88dk_fastcall;
extern unsigned char    spr_get_tile_dir(unsigned char *f_tile, unsigned char *f_inc);
extern void             spr_back_repaint(void);
extern void             spr_clear_scr(void);
extern void             spr_turn_horizontal(void);
extern void             spr_destroy(unsigned char f_sprite) __z88dk_fastcall;
extern void             spr_draw8(unsigned char f_spr8, unsigned char f_lin, unsigned char f_col);
extern unsigned char    spr_horizontal_check(unsigned char f_col) __z88dk_fastcall;


#endif
