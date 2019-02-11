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
#include "game_banks.h"
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
  nirv_sprite_index = 0xFF;
  while (sprite < spr_count) {

    ++nirv_sprite_index;
    if (nirv_sprite_index == NIRV_SPRITE_P1) {
      // This Nirvana Version have only 7 sprites -1 door -1 player
      NIRVANAP_halt();
      // z80_delay_ms(2);
      nirv_sprite_index = 0;
    }
    if (class[sprite] != 0 && spr_chktime()) {
      s_lin0 = lin[sprite];
      s_col0 = col[sprite];
      enemy_move();
    }

    ++sprite;
  }
  if (nirv_sprite_index != 0xFF && spr_count >= NIRV_SPRITE_P1) {
    NIRVANAP_halt();
  }
}

void enemy_move(void) {

  switch (spr_kind[sprite]) {
  case E_STATIC:
    enemy_static();
    spr_paint();
    // if (spr_clr)
    //  spr_clear_fast();
    break;
  case E_HORIZONTAL:
    enemy_horizontal();
    spr_paint();
    if (spr_clr) {
      if (scr_curr >= 20) { // ON PIETRO MODE
        spr_back_repaint();
      } else {
        spr_clear_fast_hor();
      }
    }
    break;
  case E_WALK:
    enemy_horizontal();
    spr_paint();
    if (spr_clr)
      spr_clear_fast();
    break;
  case E_VERTICAL:
    enemy_vertical();

    spr_paint();

    if (spr_clr) {
      spr_clear_fast_vert();
    }
    break;
  case E_EUGENE:
    enemy_eugene();
    spr_paint();
    if (spr_clr) {
      spr_clear_fast_vert();
    }
    break;
  case E_SKYLAB:
    enemy_gota();
    spr_paint();
    if (spr_clr) {
      spr_clear_fast();
    }
    break;
  case E_FALL:
    enemy_fall();
    spr_paint();
    if (spr_clr) {
      spr_clear_fast_vert();
    }
    break;
  case E_ZIGZAG:
    enemy_zigzag();
    spr_paint();
    // NIRVANAP_drawT(SPRITE_EMPTY, s_lin0 + GAME_OFFSET_Y, s_col0);
    spr_back_repaint();

    break;
  }
}

void enemy_zigzag() {

  if (BIT_CHK(state[sprite], STAT_JUMP)) {
    lin[sprite] = lin[sprite] + SPRITE_LIN_INC;
    v0 = abs(lin[sprite] - value_c[sprite]);
    if (v0 == 8) {
      BIT_CLR(state[sprite], STAT_JUMP);
    }
  } else {
    lin[sprite] = lin[sprite] - SPRITE_LIN_INC;
    v0 = abs(lin[sprite] - value_c[sprite]);
    if (v0 == 8) {
      BIT_SET(state[sprite], STAT_JUMP);
    }
  }

  enemy_horizontal();
}

void enemy_gota() {
  // unsigned char l_found;

  if (colint[sprite] == 0) {

    index1 = spr_calc_index(lin[sprite] + 16, col[sprite]);
    if (scr_map[index1] == TILE_EMPTY) {
      spr_move_down_f();
    } else {
      colint[sprite] = 1;
    }

  } else {
    if (game_check_time(&last_time_a[sprite], spr_speed_a[sprite])) {
      colint[sprite]++;
      if (colint[sprite] == spr_frames[sprite]) {

        v2 = s_lin0 + GAME_OFFSET_Y;
        v3 = s_col0 + 1;
        NIRVANAP_paintC(attrib_hl, v2, s_col0);
        NIRVANAP_paintC(attrib_hl, v2, v3);
        v2 = v2 + 8;
        NIRVANAP_paintC(attrib_hl, v2, s_col0);
        NIRVANAP_paintC(attrib_hl, v2, v3);

        lin[sprite] = value_a[sprite];
        if (scr_curr == 13) {
          // Skylab Respawn Hack!
          lin[sprite] = 0;
          col[sprite] = col[sprite] + 8;
          if (col[sprite] > 31) {
            col[sprite] = value_a[sprite]; // Initial Value
          }
        }

        colint[sprite] = 0;
      }
      last_time_a[sprite] = zx_clock();
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
  index1 = spr_calc_index(s_lin0, s_col0 + 1);
  game_cell_paint_index();
  if (lin[sprite] >= 104) {
    class[sprite] = 0;
    NIRVANAP_spriteT(sprite, 0, 0, 0);
  }
}

void enemy_static() {

  ++colint[sprite];
  if (colint[sprite] == spr_frames[sprite]) {
    colint[sprite] = 0;
  }
}

void enemy_horizontal() {

  if (BIT_CHK(state[sprite], STAT_DIRR)) {
    spr_move_right_f();
    if ((col[sprite] >= value_b[sprite] &&
         colint[sprite] == 3)) { // HACK spr_frames[sprite] - 1
      spr_set_left();
      tile[sprite] = spr_get_tile(&sprite);
    }

  } else {
    spr_move_left_f();
    if ((col[sprite] <= value_a[sprite] && colint[sprite] == 0)) {
      spr_set_right();
      tile[sprite] = spr_get_tile(&sprite);
    }
  }
}

void enemy_vertical() {

  if (game_check_time(&last_time_a[sprite], spr_speed_a[sprite])) {
    ++colint[sprite];
    if (colint[sprite] == spr_frames[sprite]) {
      colint[sprite] = 0;
    }
    last_time_a[sprite] = zx_clock();
  }
  if (game_check_time(&last_time_b[sprite], spr_speed_b[sprite])) {
    if (BIT_CHK(state[sprite], STAT_JUMP)) {
      spr_move_up_f();
      if (lin[sprite] == value_a[sprite]) {
        spr_clear_fast_vert();
        spr_set_down();
      }
    } else {
      spr_move_down_f();
      if (lin[sprite] == value_b[sprite]) {
        spr_clear_fast_vert();
        spr_set_up();
      }
    }
    last_time_b[sprite] = zx_clock();
  }
}

void enemy_eugene() {

  if (obj_count != 255) {
    spr_frames[sprite] = 1;
    enemy_vertical();
  } else {
    if (game_check_time(&last_time_b[sprite], spr_speed_b[sprite])) {
      // spr_frames[sprite] = 4;
      ++colint[sprite];
      if (colint[sprite] > 3) {
        colint[sprite] = 0;
      }
      if (lin[sprite] < 88) {
        BIT_SET(state[sprite], STAT_FALL);
        spr_move_down_f();
      }
      last_time_b[sprite] = zx_clock();
    }
  }
}

void enemy_init() {

  unsigned char f_tot_class;
  unsigned char f_class;
  unsigned char f_variant;
  unsigned char f_basetile;
  unsigned int f_pos;

  unsigned char *f_init;

  // Defines
  f_class = scr_map[index1];

  f_variant = 0;
  if (f_class & 1) {
    --f_class;
    f_variant = 1;
  }

  // Found an empty sprite slot
  f_tot_class = 0;
  sprite = 0;
  while (sprite < GAME_MAX_ENEMIES) {
    if (class[sprite] == 0) {
      break;
    }
    if (class[sprite] == f_class) {
      ++f_tot_class;
    }
    ++sprite;
  }

  f_init = &spr_init[0];

  // Search for Sprite Attribs on spr_init
  f_pos = 0;
  // Size of spr_init attributes
  while (f_pos <= (GAME_TOTAL_INDEX_CLASSES * GAME_SPR_INIT_SIZE)) {

    if (*(f_init + f_pos) == f_class) {

      // Read from ARRAY
      f_basetile = *(f_init + f_pos + 1); //base tile
      spr_frames[sprite] = *(f_init + f_pos + 2); //frames
      spr_altset[sprite] = *(f_init + f_pos + 3);
      spr_kind[sprite] = *(f_init + f_pos + 4); //Class of sprite

      // Color Alternates
      spr_init_cin0 = *(f_init + f_pos + 5);

      switch (f_tot_class) {
      case 0:
        spr_init_cin0 = 0;
        spr_init_cout0 = 0;
        break;
      case 1:
        spr_init_cout0 = *(f_init + f_pos + 6);
        break;
      case 2:
        spr_init_cout0 = *(f_init + f_pos + 7);
        break;
      case 3:
        spr_init_cout0 = *(f_init + f_pos + 8);
        break;
      }

      // Read From Map
      class[sprite] = f_class;
      spr_speed[sprite] = scr_map[index1 + 1];
      value_a[sprite] = scr_map[index1 + 32];
      value_b[sprite] = scr_map[index1 + 33];

      // Clear Adjancent tiles
      scr_map[index1] = TILE_EMPTY;
      scr_map[index1 + 1] = TILE_EMPTY; // NO USE
      scr_map[index1 + 32] = TILE_EMPTY;
      scr_map[index1 + 33] = TILE_EMPTY;
      // Class Found!
      state[sprite] = 0;
      state_a[sprite] = 0;
      // Position
      lin[sprite] = (index1 >> 5) << 3;
      col[sprite] = index1 & 31;
      colint[sprite] = 0;
      if (scr_curr == 1) {
        // HACK el pinguino queda bajo la salida
        col[sprite] = col[sprite] + 2;
      }
      switch (spr_kind[sprite]) {
      case E_SKYLAB:
        // value_b[sprite] = col[sprite];
        spr_speed_a[sprite] = spr_altset[sprite];
        break;
      case E_HORIZONTAL:
        if (f_variant) {
          BIT_SET(state[sprite], STAT_DIRR);
        } else {
          BIT_SET(state[sprite], STAT_DIRL);
          colint[sprite] = 3;
        }
        break;
      case E_VERTICAL:

        spr_speed_a[sprite] = spr_altset[sprite];
        spr_speed_b[sprite] = spr_speed[sprite];
        spr_speed[sprite] = 2; // CAUTION USE 2 MULTIPLES
        if (f_variant) {
          BIT_SET(state[sprite], STAT_JUMP);
        } else {
          BIT_SET(state[sprite], STAT_FALL);
        }
        break;
      case E_EUGENE:
        spr_speed_a[sprite] = spr_altset[sprite];
        spr_speed_b[sprite] = spr_speed[sprite];
        spr_speed[sprite] = 2; // CAUTION USE 2 MULTIPLES
        if (f_variant) {
          BIT_SET(state[sprite], STAT_JUMP);
        } else {
          BIT_SET(state[sprite], STAT_FALL);
        }

        break;
      case E_ZIGZAG:
        value_c[sprite] = lin[sprite];
        BIT_SET(state[sprite], STAT_JUMP);
        if (f_variant) {
          BIT_SET(state[sprite], STAT_DIRR);
        } else {
          BIT_SET(state[sprite], STAT_DIRL);
          colint[sprite] = 3;
        }
        break;
      }

      // Tile paging
      if (spr_frames[sprite] == 4) {
        // Standard 4 tiles animation
        spr_tile[sprite] = game_tile_cnt;
        game_tile_cnt = game_copy_sprite_std(f_basetile, game_tile_cnt);
      } else {
        // Others animations
        i = 0;
        spr_tile[sprite] = game_tile_cnt;
        while (i < spr_frames[sprite]) {
          game_copy_sprite(f_basetile + i, game_tile_cnt, 0);
          ++game_tile_cnt;
          ++i;
        }
      }

      // End Tile Paging
      tile[sprite] = spr_get_tile(&sprite);

      // Hack Kong
      if (f_class == 82) {
        spr_frames[sprite] = 2;
      }
      ++spr_count;

      last_time[sprite] = 0;

      break;
    } else {
      // Next Class increment
      f_pos = f_pos + GAME_SPR_INIT_SIZE; // N variables on spr_init
    }
  }
}
