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
#ifndef GAME_AY_H
#define GAME_AY_H

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

// AY Songs
//BANK_3


//BANK_4

extern unsigned char ay_song_04_ingame[];
extern unsigned char ay_song_04_ingame0[];

//BANK_6
extern unsigned char ay_song_06_lotr_lose_a_life[];
extern unsigned char ay_song_06_lotr_level_complete[];
extern unsigned char ay_song_06_lotr_level_start[];
// AY Sound Effects
extern unsigned char ay_fx_06_coin[];
extern unsigned char ay_fx_06_dano_por_toque[];
extern unsigned char ay_fx_06_explosion[];
extern unsigned char ay_fx_06_golpe[];
extern unsigned char ay_fx_06_magic1[];
extern unsigned char ay_fx_06_puerta[];
extern unsigned char ay_fx_06_salto[];
extern unsigned char ay_fx_06_sin_mana[];
extern unsigned char ay_fx_06_magic[];
extern unsigned char ay_fx_06_levelup[];
extern unsigned char ay_fx_06_disparo1[];
extern unsigned char ay_fx_06_disparo2[];
extern unsigned char ay_fx_06_efecto[];
extern unsigned char ay_fx_06_explosion1[];
extern unsigned char ay_fx_06_disparo_axe[];
extern unsigned char ay_fx_06_disparo_fire[];


#endif
