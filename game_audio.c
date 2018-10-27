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
#include "game.h"
#include "game_banks.h"
#include "game_enemies.h"
#include "game_engine.h"
#include "game_player.h"
//#include "game_sound.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include <arch/zx.h>
#include <arch/zx/nirvana+.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

void audio_ingame(void) {

  if (game_song_play && !game_song_play_start) {
    game_song_play_start = 1;
    if (game_mode == 0) {
      ay_song_play(AY_SONG_LOOP, 4, ay_song_ingame0);
    } else {
      ay_song_play(AY_SONG_LOOP, 4, ay_song_ingame1);
    }
  }
}
void audio_round_init(void) {
  ay_song_play(AY_SONG_ONCE, 6, ay_song_round_init);
}

void audio_time(void) {
  ay_song_stop();
  ay_song_play(AY_SONG_ONCE, 6, ay_song_time_atack);
}

void audio_dead(void) {
  ay_fx_stop();
  ay_fx_play(4, ay_fx_dead);
}

void audio_crumble(void) {
  ay_fx_play(4, ay_fx_crumble);
}

void audio_jump(void) {
  ay_fx_play(4, ay_fx_jump);
  player_fall_start = 1;
}

void audio_fall(void) {
  if (player_fall_start) {
    //ay_fx_stop();
    ay_fx_play(4, ay_fx_fall);
    player_fall_start = 0;
  }
}
void audio_game_start(void) { ay_fx_play(4, ay_fx_game_start); }


void audio_door_open(void) {
  ay_fx_play(4, ay_fx_door_open);
}
void audio_tick(void) {
  ay_fx_stop();
  ay_fx_play(4, ay_fx_tick);
}
void audio_coin(void) {
  // ay_fx_stop();
  ay_fx_play(4, ay_fx_coin);
}
void audio_coin_noentiendo(void) {
  // ay_fx_stop();
  ay_fx_play(4, ay_fx_coin_noentiendo);
}
void audio_puerta(void) {
  // ay_fx_play(4, ay_fx_puerta);
}

void audio_level_start(void) {
  ay_reset();
  // ay_song_play(AY_SONG_ONCE, 6, ay_song_06_lotr_level_start);
}
void audio_level_complete(void) {
  ay_reset();
  // ay_song_play(AY_SONG_ONCE, 6, ay_song_06_lotr_level_complete);
}
void audio_game_end(void) {
  ay_reset();
  // ay_song_play(AY_SONG_ONCE, 3, ay_song_03_sunflower);
}
void audio_menu(void) {
  ay_reset();
  ay_song_play(AY_SONG_LOOP, 4, ay_song_menu);
}

void audio_levelup(void) {
  ay_fx_stop();
  // ay_fx_play(4, ay_fx_levelup);
}
