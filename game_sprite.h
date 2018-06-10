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
extern unsigned char    spr_chktime(unsigned char *sprite) __z88dk_fastcall;
extern unsigned int	    spr_calc_index(unsigned char l, unsigned char c);
extern void             spr_set_up(unsigned char * f_state) __z88dk_fastcall;
extern void             spr_set_down(unsigned char * f_state) __z88dk_fastcall;
extern void             spr_set_left(unsigned char * f_state) __z88dk_fastcall;
extern void             spr_set_right(unsigned char * f_state) __z88dk_fastcall;
extern unsigned char    spr_move_up(void);
extern unsigned char    spr_move_down(void);
extern unsigned char    spr_move_horizontal(void);
extern unsigned char    spr_move_right(void);
extern unsigned char    spr_move_left(void);
extern unsigned char    spr_paint(void);
extern unsigned char    spr_paint_player(void);
extern unsigned char    spr_tile(unsigned char *f_sprite) __z88dk_fastcall;
extern unsigned char    spr_tile_dir(unsigned char *f_tile, unsigned char *f_sprite, unsigned char *f_inc);
extern void             spr_back_repaint(void);
extern void             spr_page_map(void);
extern void             spr_clear_scr(void);
extern void             spr_add_anim( unsigned char f_lin, unsigned char f_col, unsigned char f_tile, unsigned char f_end, unsigned char f_loops );
extern void             spr_play_anim(void);
extern void             spr_turn_horizontal(void);
extern void             spr_init_effects(void);
extern void             spr_btile_paint_back(void);
extern void             spr_flatten(void);
extern void             spr_unflattenP1(void);
extern unsigned char    spr_colision_boss(unsigned char f_lin, unsigned char f_col);
extern void             spr_destroy(unsigned char f_sprite) __z88dk_fastcall;


#endif
