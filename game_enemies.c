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
#include "game_audio.h"
#include "game_ay.h"
#include "game_enemies.h"
#include "game_engine.h"
#include "game_player.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include <arch/zx.h>
#include <arch/zx/nirvana+.h>
#include <input.h>
#include <stdlib.h>

void enemy_turn(void) {

  sprite = 0;
  nirv_sprite_index = 0;
  while (sprite < spr_count) {

    s_class = class[sprite];
    if (s_class != 0) {
      if (spr_chktime(&sprite)) {

        s_lin0 = lin[sprite];
        s_col0 = col[sprite];
        s_tile0 = tile[sprite];
        p_state = &state[sprite];
        p_state_a = &state_a[sprite];
        enemy_move();
        spr_paint();
        last_time[sprite] = zx_clock();
      }
    }
    ++sprite;
    ++nirv_sprite_index;
    if (nirv_sprite_index >= NIRV_TOTAL_SPRITES) {
      nirv_sprite_index = 0;
      NIRVANAP_halt();
    }
  }
  if (spr_count > NIRV_TOTAL_SPRITES) {
    NIRVANAP_halt();
  }
}

void enemy_move(void) {

  switch (spr_kind[sprite]) {
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
    if (colint[sprite] == spr_frames[sprite]) {
      // Respawn!
      // lin[sprite] = (game_respawn_index[sprite] >> 3) << 4;
      colint[sprite] = 0;
    }
  }
}

void enemy_static() {

  ++colint[sprite];
  if (colint[sprite] == spr_frames[sprite]) {
    colint[sprite] = 0;
  }
}

void enemy_horizontal() {
  if (BIT_CHK(*p_state, STAT_DIRR)) {
    spr_move_right_f();
    if (col[sprite] == value_b[sprite] && colint[sprite] == 3 ||
        col[sprite] > 30) { // HACK spr_frames[sprite] - 1
      spr_set_left();
      state[sprite] = *p_state;
      tile[sprite] = spr_get_tile(&sprite);
    }
  } else {
    spr_move_left_f();
    if (col[sprite] == value_a[sprite] && colint[sprite] == 0 ||
        col[sprite] <= 1) {
      spr_set_right();
      state[sprite] = *p_state;
      tile[sprite] = spr_get_tile(&sprite);
    }
  }
}

void enemy_vertical() {

  if (BIT_CHK(*p_state, STAT_JUMP)) {
    spr_move_up_f();
    if (lin[sprite] == value_a[sprite] || lin[sprite] > GAME_LIN_FLOOR) {
      spr_set_down();
    }
  } else {
    spr_move_down_f();
    if (lin[sprite] == value_b[sprite] || lin[sprite] > GAME_LIN_FLOOR) {
      spr_set_up();
    }
  }
}

void enemy_walk(void) {}

void enemy_kill(unsigned char f_sprite) __z88dk_fastcall {
  s_lin0 = lin[f_sprite];
  s_col0 = col[f_sprite];
  spr_destroy(f_sprite);
}

void enemy_avoid_fall() {

  /* Don't fall on edge*/
  if (BIT_CHK(*p_state, STAT_DIRR)) {
    index1 = spr_calc_index(lin[sprite] + GAME_OFFSET_Y, col[sprite] + 2);
  } else {
    index1 = spr_calc_index(lin[sprite] + GAME_OFFSET_Y, col[sprite] - 2);
  }
  if (scr_map[index1] < TILE_FLOOR) {
    spr_turn_horizontal();
  }
}

unsigned char enemy_avoid_dead() {

  index1 = spr_calc_index(lin[sprite], col[sprite]);
  if (BIT_CHK(*p_state, STAT_DIRR)) {
    index1++;
  } else {
    index1--;
  }

  /* Don't fall on edge*/
  v0 = scr_map[index1];
  if (v0 > TILE_ITEM_E && v0 < TILE_FLOOR) {
    spr_turn_horizontal();
    return 1;
  }

  /* Don't hit on Deadly*/
  index1 = index1 + 16;
  v0 = scr_map[index1];
  if (v0 > TILE_ITEM_E && v0 < TILE_FLOOR) {
    spr_turn_horizontal();
    return 1;
  }

  return 0;
}

void enemy_init() {
  unsigned char f_sprite;

  // Get the first available sprite
  f_sprite = 0;
  while (f_sprite < GAME_MAX_ENEMIES) {
    if (class[f_sprite] == 0) {
      v0 = 0;
      while (v0 <= (GAME_TOTAL_INDEX_CLASSES * 6)) {

        if (spr_init[v0] == scr_map[index1]) {
          // Class Found!
          sprite = f_sprite;
          state[sprite] = 0;
          state_a[sprite] = 0;
          colint[sprite] = 0;

          class[sprite] = scr_map[index1];

          // spr_tile[sprite] = spr_init[v0 + 1];
          BIT_SET(state[sprite], spr_init[v0 + 2]);
          spr_frames[sprite] = spr_init[v0 + 3];
          spr_kind[sprite] = spr_init[v0 + 4];

          spr_speed[sprite] = scr_map[index1 + 1];

          // Tile paging
          if (spr_init_tile[class[sprite] - 64] == 0) {
            // Not paged
            if (spr_frames[sprite] == 4) {
              // Standard 4 tiles animation
              spr_tile[sprite] = game_tile_cnt;
              if ((spr_init_tile[class[sprite]] & 1) == 0) {
                spr_init_tile[class[sprite] - 64] = game_tile_cnt;
                spr_init_tile[class[sprite] - 64 + 1] = game_tile_cnt;
              } else {
                spr_init_tile[class[sprite] - 64] = game_tile_cnt;
                spr_init_tile[class[sprite] - 64 - 1] = game_tile_cnt;
              }

              game_tile_cnt =
                  game_copy_tile_std(spr_init[v0 + 1], game_tile_cnt);

            } else {
              // EUGENE (1 FRAME) TODO 2 FRAMES
              spr_tile[sprite] = game_tile_cnt;
              spr_init_tile[class[sprite] - 64] = game_tile_cnt;
              game_copy_tile(spr_init[v0 + 1], game_tile_cnt,0);
              ++game_tile_cnt;
            }
          } else {
            // Read Value
            spr_tile[sprite] = spr_init_tile[class[sprite] - 64];
          }
          // End Tile Paging

          lin[sprite] = (index1 >> 5) << 3;
          col[sprite] = index1 & 31;

          p_state = &state[sprite];
          p_state_a = &state_a[sprite];

          tile[sprite] = spr_get_tile(&sprite);

          ++spr_count;
          spr_timer[sprite] = zx_clock();
          last_time[sprite] = 0;
          break;
        } else {
          // increment
          v0 = v0 + 5; // N variables on spr_init
        }
      }
      break;
    } else {
      f_sprite++;
    }
  }
}
