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
#include "game_audio.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include <arch/zx/nirvana+.h>
#include <arch/zx.h>
#include <input.h>
#include <stdlib.h>

void enemy_turn(void) {

  sprite = 0;

  while (sprite < SPR_P1) {
    s_class = class[sprite];
    if (s_class > 0 && spr_chktime(&sprite)) {
      s_lin0 = lin[sprite];
      s_col0 = col[sprite];
      // s_tile0 = tile[sprite] + colint[sprite];
      s_state = state[sprite];
      enemy_move();
      spr_paint();

      state[sprite] = s_state;
      last_time[sprite] = zx_clock();
    }
    ++sprite;
  }
}


void boss_turn() {

  if (game_check_time(&boss_time, 6)) {



    s_col1 = boss_col;
    s_lin1 = boss_lin;

    // MOVE VERTICAL
    if (BIT_CHK(boss_stat, STAT_JUMP)) {
      boss_lin = boss_lin - 2;
      if (boss_lin < 48) {
        spr_set_down(&boss_stat);
      }
    } else {
      boss_lin = boss_lin + 2;
      if (boss_lin >= (GAME_LIN_FLOOR - 48)) {
        spr_set_up(&boss_stat);
      }
    }
    if (game_check_time(&boss_time_fire, 64)) {
      --boss_col;
      --boss_col;
      tmp0 = 0;
      audio_disparo_fire();
      if (game_world == 1) {
        tmp0 = 1;
      }
      boss_time_fire = zx_clock();
      ++boss_col;
      ++boss_col;
    }



    // PAINT
    boss_draw();


    boss_time = zx_clock();
    // KILL PLAYER INSTANTLY
    if (spr_colision_boss(lin[SPR_P1], col[SPR_P1])) {
      player_lost_life();
    }


  }
}

void boss_draw() {
  intrinsic_halt();
  if (boss_inc) {
    boss_inc = 0;
    boss_draw_frame(0);
  } else {
    boss_inc = 1;
    boss_draw_frame(2);
  }
}

void boss_draw_frame(unsigned char f_inc) __z88dk_fastcall {
  NIRVANAP_drawT_raw(boss_tile + f_inc + 0, boss_lin, s_col1);
  NIRVANAP_drawT_raw(boss_tile + f_inc + 1, boss_lin, s_col1 + 2);
  NIRVANAP_drawT_raw(boss_tile + f_inc + 12, boss_lin + 16, s_col1);
  NIRVANAP_drawT_raw(boss_tile + f_inc + 13, boss_lin + 16, s_col1 + 2);
}

void enemy_move(void) {

  switch (sprite_kind[s_class]) {
  case E_STATIC:
    enemy_static();
    break;
  case E_HORIZONTAL:
    enemy_horizontal();
    break;
  case E_WALK:
    enemy_horizontal();
    break;
  case E_VERTICAL:
    enemy_vertical();
    break;
  case E_GHOST:
    enemy_ghost();
    break;
  case E_GOTA:
    enemy_gota();
    break;
  }
}

void enemy_gota() {
  if (colint[sprite] == 0) {
    if (spr_move_down()) {
      colint[sprite] = 1;
    }
  } else {
    colint[sprite]++;
    if (colint[sprite] == sprite_frames[s_class]) {
      // Respawn!
      //lin[sprite] = (game_respawn_index[sprite] >> 3) << 4;
      colint[sprite] = 0;
    }
  }
}

void enemy_ghost() {
  unsigned char v;
  v = 0;

  if (s_col0 < col[SPR_P1]) {
    spr_set_right(&s_state);
    tile[sprite] = spr_tile(&sprite);
    spr_move_right();
    v = 1;
  }
  if (s_col0 > col[SPR_P1]) {
    spr_set_left(&s_state);
    tile[sprite] = spr_tile(&sprite);
    spr_move_left();
    v = 1;
  }

  if (s_col0 == col[sprite]) {
    if (s_lin0 > lin[SPR_P1]) {
      spr_move_up();
    }
    if (s_lin0 < lin[SPR_P1]) {
      spr_move_down();
    }
    if (v == 0 && s_lin0 != lin[sprite]) {
      ++colint[sprite];
      if (colint[sprite] == sprite_frames[s_class]) {
        colint[sprite] = 0;
      }
    }
  } else {
    spr_back_repaint();
    s_col0 = col[sprite];
  }
}

void enemy_static() {

  ++colint[sprite];
  if (colint[sprite] == sprite_frames[s_class]) {
    colint[sprite] = 0;
  }
}

void enemy_horizontal() {
  if (BIT_CHK(s_state, STAT_DIRR)) {
    if (spr_move_right()) {

      spr_set_left(&s_state);
      state[sprite] = s_state;
      tile[sprite] = spr_tile(&sprite);

    }
  } else {
    if (spr_move_left()) {
      spr_set_right(&s_state);
      state[sprite] = s_state;
      tile[sprite] = spr_tile(&sprite);

    }
  }
}

void enemy_vertical() {

}

void enemy_walk(void) {

}

void enemy_kill(unsigned char f_sprite) __z88dk_fastcall {
  s_lin0 = lin[f_sprite];
  s_col0 = col[f_sprite];
  spr_destroy(f_sprite);
  spr_add_anim(s_lin0, s_col0, TILE_ANIM_DEAD, 3, 0);

}

void enemy_avoid_fall() {

  /* Don't fall on edge*/
  if (BIT_CHK(s_state, STAT_DIRR)) {
    index1 = spr_calc_index(lin[sprite] + 16, col[sprite] + 2);
  } else {
    index1 = spr_calc_index(lin[sprite] + 16, col[sprite] - 2);
  }
  if (scr_map[index1] < TILE_FLOOR) {
    spr_turn_horizontal();
  }
}

unsigned char enemy_avoid_dead() {

  index1 = spr_calc_index(lin[sprite], col[sprite]);
  if (BIT_CHK(s_state, STAT_DIRR)) {
    index1++;
  } else {
    index1--;
  }

  /* Don't fall on edge*/
  tmp0 = scr_map[index1];
  if (tmp0 > TILE_ITEM_E && tmp0 < TILE_FLOOR) {
    spr_turn_horizontal();
    return 1;
  }

  /* Don't hit on Deadly*/
  index1 = index1 + 16;
  tmp0 = scr_map[index1];
  if (tmp0 > TILE_ITEM_E && tmp0 < TILE_FLOOR) {
    spr_turn_horizontal();
    return 1;
  }

  return 0;
}


void enemy_init(unsigned char f_lin, unsigned char f_col, unsigned char f_class,
                unsigned char f_dir) {
  unsigned char f_sprite;
  // Get the first available sprite
  f_sprite = 0;
  while (f_sprite < SPR_P1) {
    if (class[f_sprite] == 0) {
      break;
    } else {
      f_sprite++;
    }
  }

  // Out of sprites
  if (f_sprite < SPR_P1) {

    class[f_sprite] = f_class;
    sprite_speed[f_sprite] = sprite_base_speed[f_class];


    lin[f_sprite] = f_lin;
    col[f_sprite] = f_col;
    state[f_sprite] = 0;
    state_a[f_sprite] = 0;
    jump_lin[f_sprite] = 0;
    colint[f_sprite] = 0;
    if (f_dir == DIR_RIGHT) {
      BIT_SET(state[f_sprite], STAT_DIRR);
    }
    if (f_dir == DIR_LEFT) {
      BIT_SET(state[f_sprite], STAT_DIRL);
      colint[f_sprite] = sprite_frames[f_class] - 1;
    }
    tile[f_sprite] = spr_tile(&f_sprite);
    spr_timer[f_sprite] = zx_clock();
    last_time[f_sprite] = 0;
    ++spr_count;
  }
}
