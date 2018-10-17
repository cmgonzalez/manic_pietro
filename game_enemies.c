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
    if (s_class != 0) { //TODO UNIFY WITH SPR_KIND!!!
      if (spr_chktime(&sprite)) {

        s_lin0 = lin[sprite];
        s_col0 = col[sprite];
        s_tile0 = tile[sprite];
        p_state = &state[sprite];
        p_state_a = &state_a[sprite];

        enemy_move();
        spr_paint();
        //NIRVANAP_halt(); NO GLITCHES!! BUT SLOW

        last_time[sprite] = zx_clock();
      }
    }
    ++sprite;
    ++nirv_sprite_index;
    if (nirv_sprite_index >= NIRV_SPRITE_P1) {
      nirv_sprite_index = 0;
      NIRVANAP_halt();
    }
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
  case E_SKYLAB:
    enemy_gota();
    break;
  case E_FALL:
    enemy_fall();
    break;
  }
}

void enemy_gota() {
  unsigned char l_found;
  if (colint[sprite] == 0) {

    index1 = spr_calc_index(lin[sprite] + 16, col[sprite]);
    if (scr_map[index1] == TILE_EMPTY) {
      spr_move_down_f();
    } else {
      colint[sprite] = 1;
    }

  } else {
    colint[sprite]++;
    if (colint[sprite] == spr_frames[sprite]) {
      // Respawn Hack!
      lin[sprite] = value_a[sprite];
      l_found = 0;
      col[sprite] = 0xFF;
      while (!l_found) {
        v0 = ((rand() & 15) << 1) - 1;

        if (v0 == 0xFF) {
          v0 = 1;
        }
        if (v0 == 15) {
          v0 = 13;
        }
        if (v0 == 23) {
          v0 = 21;
        }

        if (col[0] != v0 && col[1] != v0 && col[2] != v0) {
          l_found = 1;
        }
      }

      col[sprite] = v0;
      colint[sprite] = 0;
    }
  }
}

void enemy_fall() {
  ++colint[sprite];
  if (colint[sprite] == spr_frames[sprite]) {
    colint[sprite] = 0;
  }
  spr_move_down_f();
  index1 = spr_calc_index(s_lin0, s_col0);
  game_cell_paint_index();
  index1 = spr_calc_index(s_lin0, s_col0+1);
  game_cell_paint_index();
  if (lin[sprite] >= 104) {
    class[sprite] = 0;
    NIRVANAP_spriteT(sprite,0,0,0);
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
    if ((col[sprite] >= value_b[sprite] && colint[sprite] == 3) ||
        col[sprite] > 30) { // HACK spr_frames[sprite] - 1
      spr_set_left();
      state[sprite] = *p_state;
      tile[sprite] = spr_get_tile(&sprite);
    }
  } else {

    spr_move_left_f();
    if ((col[sprite] <= value_a[sprite] && colint[sprite] == 0) ||
        col[sprite] <= 1) {
      spr_set_right();

      state[sprite] = *p_state;
      tile[sprite] = spr_get_tile(&sprite);
    }
  }
}

void enemy_vertical() {

  ++colint[sprite];
  if (colint[sprite] == spr_frames[sprite]) {
    colint[sprite] = 0;
  }

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

void enemy_init() {
  unsigned char f_sprite;
  unsigned char f_tot_class;
  unsigned char f_class;
  unsigned char f_variant;
  unsigned char f_basetile;

  // Defines
  f_class = scr_map[index1];

  f_variant = 0;
  if (f_class & 1) {
    --f_class;
    f_variant = 1;
  }

  // Found an empty sprite slot
  f_tot_class = 0;
  f_sprite = 0;
  while (f_sprite < GAME_MAX_ENEMIES) {
    if (class[f_sprite] == 0) {
      break;
    }
    if (class[f_sprite] == f_class) {
      ++f_tot_class;
    }
    ++f_sprite;
  }

  // Search for Sprite Attribs on spr_init
  v0 = 0;

  while (v0 <= (GAME_TOTAL_INDEX_CLASSES * 8)) {

    if (spr_init[v0] == f_class) {

      // Read from ARRAY
      f_basetile = spr_init[v0 + 1];
      spr_frames[f_sprite] = spr_init[v0 + 2];
      spr_kind[f_sprite] = spr_init[v0 + 3];

      // Color Alternates
      spr_init_cin0 = spr_init[v0 + 4];

      switch (f_tot_class) {
      case 0:
        spr_init_cin0 = 0;
        spr_init_cout0 = 0;
        break;
      case 1:
        spr_init_cout0 = spr_init[v0 + 5];
        break;
      case 2:
        spr_init_cout0 = spr_init[v0 + 6];
        break;
      case 3:
        spr_init_cout0 = spr_init[v0 + 7];
        break;
      }

      // Read From Map
      class[f_sprite] = f_class;
      spr_speed[f_sprite] = scr_map[index1 + 1];
      value_a[f_sprite] = scr_map[index1 + 32];
      value_b[f_sprite] = scr_map[index1 + 33];

      // Clear Adjancent tiles
      scr_map[index1] = TILE_EMPTY;
      scr_map[index1 + 1] = TILE_EMPTY; // NO USE
      scr_map[index1 + 32] = TILE_EMPTY;
      scr_map[index1 + 33] = TILE_EMPTY;
      // Class Found!
      state[f_sprite] = 0;
      state_a[f_sprite] = 0;
      // Position
      lin[f_sprite] = (index1 >> 5) << 3;
      col[f_sprite] = index1 & 31;
      colint[f_sprite] = 0;

      p_state = &state[f_sprite];
      p_state_a = &state_a[f_sprite];

      switch (spr_kind[f_sprite]) {
      case E_HORIZONTAL:
        if (f_variant) {
          BIT_SET(*p_state, STAT_DIRR);
        } else {
          BIT_SET(*p_state, STAT_DIRL);
          colint[f_sprite] = 3;
        }
        break;
      case E_VERTICAL:
        if (f_variant) {
          BIT_SET(*p_state, STAT_JUMP);
        } else {
          BIT_SET(*p_state, STAT_FALL);
        }
        break;
      }

      // Tile paging
      if (spr_frames[f_sprite] == 4) {
        // Standard 4 tiles animation
        spr_tile[f_sprite] = game_tile_cnt;
        game_tile_cnt = game_copy_sprite_std(f_basetile, game_tile_cnt);
      } else {
        // Others animations
        i = 0;
        spr_tile[f_sprite] = game_tile_cnt;
        while (i < spr_frames[f_sprite]) {
          game_copy_sprite(f_basetile + i, game_tile_cnt, 0);
          ++game_tile_cnt;
          ++i;
        }
      }

      // End Tile Paging
      tile[f_sprite] = spr_get_tile(&f_sprite);

      // Hack Kong
      if (f_class == 82) {
        spr_frames[f_sprite] = 2;
      }
      ++spr_count;

      spr_timer[f_sprite] = zx_clock();
      last_time[f_sprite] = 0;

      break;
    } else {
      // Next Class increment
      v0 = v0 + 8; // N variables on spr_init
    }
  }
}
