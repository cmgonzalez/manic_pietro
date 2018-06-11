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
#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

extern void          player_init(unsigned char f_lin, unsigned char f_col, unsigned char f_tile);
extern unsigned char player_check_input(void);
extern unsigned char player_action_jump();
extern unsigned char player_move(void);
extern void          player_turn(void);
extern void          player_new_jump(void);
extern unsigned char player_move_input(void);
extern void          player_score_add(unsigned int f_score) __z88dk_fastcall;
extern unsigned char player_check_floor(unsigned char f_lin, unsigned char f_col);
extern unsigned char player_check_ceil(unsigned char f_lin, unsigned char f_col);
extern void          player_pick_item(void);
extern void          player_tile( unsigned char f_tile, unsigned char f_inc );
extern unsigned char player_move_jump(void);
extern void          player_damage(unsigned char f_val) __z88dk_fastcall;
extern void          player_lost_life();
extern void          player_gasta_brick();


#endif
