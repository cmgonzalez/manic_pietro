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
#include "game_ay.h"
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
      ay_song_play(AY_SONG_LOOP, 4, ay_song_04_ingame);
  }
}

void audio_explosion(void) {
  ay_fx_stop();
  ay_fx_play(6, ay_fx_06_explosion);
}

void audio_explosion1(void) {
  ay_fx_play(6, ay_fx_06_explosion);
  // ay_song_play(AY_SONG_ONCE, 4, 4, ay_fx_06_explosion1);
}

void audio_disparo1(void) { ay_fx_play(6, ay_fx_06_disparo1); }

void audio_disparo2(void) { ay_fx_play(6, ay_fx_06_disparo2); }

void audio_disparo_axe(void) { ay_fx_play(6, ay_fx_06_disparo_axe); }

void audio_disparo_fire(void) { ay_fx_play(6, ay_fx_06_disparo_fire); }

void audio_efecto(void) { ay_fx_play(6, ay_fx_06_efecto); }
void audio_salto(void) { ay_fx_play(6, ay_fx_06_salto); }
void audio_golpe(void) { ay_fx_play(6, ay_fx_06_golpe); }
void audio_coin(void) {
  // ay_fx_stop();
  ay_fx_play(6, ay_fx_06_coin);
}
void audio_magic1(void) { ay_fx_play(6, ay_fx_06_magic1); }
void audio_sin_mana(void) { ay_fx_play(6, ay_fx_06_sin_mana); }
void audio_puerta(void) { ay_fx_play(6, ay_fx_06_puerta); }

void audio_lotr_lose_a_life(void) {
  ay_reset();
  ay_song_play(AY_SONG_ONCE, 6, ay_song_06_lotr_lose_a_life);
}
void audio_level_start(void) {
  ay_reset();
  ay_song_play(AY_SONG_ONCE, 6, ay_song_06_lotr_level_start);
}
void audio_level_complete(void) {
  ay_reset();
  ay_song_play(AY_SONG_ONCE, 6, ay_song_06_lotr_level_complete);
}
void audio_game_end(void) {
  ay_reset();
  //ay_song_play(AY_SONG_ONCE, 3, ay_song_03_sunflower);
}
void audio_menu(void) {
  ay_reset();
  //ay_song_play(AY_SONG_LOOP, 3, ay_song_03_tdm_cs9);
}

void audio_levelup(void) {
  ay_fx_stop();
  ay_fx_play(6, ay_fx_06_levelup);
}
