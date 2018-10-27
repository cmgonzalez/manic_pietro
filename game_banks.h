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

//TODO RENAME THIS IS NOT AY IT'S UPPER MEM
#ifndef game_banks_H
#define game_banks_H

// Bit Flags Returned from ay_is_playing()

#define AY_PLAYING_NONE          0
#define AY_PLAYING_SONG          1
#define AY_PLAYING_FX            2

extern unsigned char ay_is_playing(void) __preserves_regs(d,e);

// AY Effects Player

extern void ay_fx_play(unsigned char bank, void *effect) __preserves_regs(a,b,c) __z88dk_callee;
extern void ay_fx_stop(void) __preserves_regs(b,c,d,e,h,l);

// AY Song Player

#define AY_SONG_LOOP             0
#define AY_SONG_ONCE             1

extern void ay_song_play(unsigned char flag, unsigned char bank, void *song) __preserves_regs(b,c) __z88dk_callee;
extern void ay_song_stop(void) __preserves_regs(b,c,d,e,h,l);

// AY Miscellaneous

extern void ay_reset(void) __preserves_regs(b,c,d,e,h,l);

//BANK 1 Images
extern unsigned char img1[];

//BANK 3 Sprites 16x16
extern unsigned char hisprites1[];
extern unsigned char hisprites2[];
//BANK 4 Songs
extern unsigned char ay_song_ingame0[];
extern unsigned char ay_song_ingame1[];
extern unsigned char ay_song_menu[];
extern unsigned char ay_song_round_init[];

//BANK 4 Sound fx
extern unsigned char ay_fx_coin[];
extern unsigned char ay_fx_dead[];
extern unsigned char ay_fx_jump[];
extern unsigned char ay_fx_fall[];
extern unsigned char ay_fx_game_start[];
extern unsigned char ay_fx_tick[];
extern unsigned char ay_fx_door_open[];
extern unsigned char ay_fx_crumble[];
extern unsigned char ay_fx_coin_noentiendo[];
//BANK 6 Songs
extern unsigned char ay_song_time_atack[];
//BANK 7 Tiles 8x8
extern unsigned char hitiles1[];
extern unsigned char hitiles2[];




#endif
