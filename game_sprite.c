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

unsigned int spr_calc_index(unsigned char f_lin, unsigned char f_col) {
  // Mapeo a indice en scr_map 32x16 mapa
  return f_col + ((f_lin >> 3) << 5);
  ;
}

unsigned char spr_chktime(void) {
  if (zx_clock() - last_time[sprite] >= spr_speed[sprite]) {
    last_time[sprite] = zx_clock();
    return 1;
  }
  return 0;
}

void spr_set_up() {
  BIT_SET(state[sprite], STAT_JUMP);
  BIT_CLR(state[sprite], STAT_FALL);
}
void spr_set_down() {
  BIT_SET(state[sprite], STAT_FALL);
  BIT_CLR(state[sprite], STAT_JUMP);
}

void spr_set_left() {
  BIT_CLR(state_a[sprite], STAT_LDIRR);
  BIT_SET(state_a[sprite], STAT_LDIRL);

  BIT_CLR(state[sprite], STAT_DIRR);
  BIT_SET(state[sprite], STAT_DIRL);
}

void spr_set_right() {
  BIT_CLR(state_a[sprite], STAT_LDIRL);
  BIT_SET(state_a[sprite], STAT_LDIRR);

  BIT_CLR(state[sprite], STAT_DIRL);
  BIT_SET(state[sprite], STAT_DIRR);
}

unsigned char spr_move_up_f(void) {

  if (lin[sprite] != 0) {
    lin[sprite] = lin[sprite] - SPRITE_LIN_INC;
    spr_clr = 1;
  }
  return 0;
}

unsigned char spr_move_down_f(void) {

  if (lin[sprite] <= GAME_LIN_FLOOR) {
    lin[sprite] = lin[sprite] + SPRITE_LIN_INC;
    spr_clr = 1;
  }
  return 0;
}

void spr_draw8(unsigned char f_spr8, unsigned char f_lin, unsigned char f_col) {
  unsigned char *f_byte_src0;
  unsigned char f_spr16;

  // f_byte_src0 = &btiles[0] + (((f_spr8 >> 2) + game_tileset) * 48);
  if (f_spr8 < 16) {
    // si es mayor que 16 puedo sacar los graficos de la segunda linea
    f_spr16 = f_spr8 >> 1;
    // f_spr8 = f_spr8 % 2;
    f_spr8 = f_spr8 & 1;
  } else {
    // si es mayor que 16 puedo sacar los graficos de la segunda linea
    f_spr8 = f_spr8 - 16;
    f_spr16 = f_spr8 >> 1;
    // f_spr8 = 2 + (f_spr8 % 2);
    f_spr8 = 2 + (f_spr8 & 1);
  }

  f_byte_src0 = &btiles[0] + (48 * f_spr16);

  switch (f_spr8 & 3) {
  case 0:
    NIRVANAP_printQ(f_byte_src0, f_byte_src0 + 32, f_lin + 8, f_col);
    break;
  case 1:
    NIRVANAP_printQ(f_byte_src0 + 1, f_byte_src0 + 40, f_lin + 8, f_col);
    break;
  case 2:
    NIRVANAP_printQ(f_byte_src0 + 16, f_byte_src0 + 36, f_lin + 8, f_col);
    break;
  case 3:
    NIRVANAP_printQ(f_byte_src0 + 17, f_byte_src0 + 44, f_lin + 8, f_col);
    break;
  }
}

void spr_move_horizontal(void) {
  if (BIT_CHK(state[sprite], STAT_DIRR)) {
    spr_move_right();
  } else {
    if (BIT_CHK(state[sprite], STAT_DIRL)) {
      spr_move_left();
    }
  }

}

unsigned char spr_horizontal_check(unsigned char f_col) __z88dk_fastcall {
  // TODO ESTO FALLA CUANDO EL COLINT ES 0 Y ESTA JUSTO AL LADO UN
  s_lin1 = lin[sprite];
  if (BIT_CHK(state[sprite], STAT_JUMP) || BIT_CHK(state[sprite], STAT_FALL)) {
    return game_check_map(s_lin1, f_col) || game_check_map(s_lin1 + 8, f_col) ||
           game_check_map(s_lin1 + 16, f_col);
  } else {

    return game_check_map(s_lin1, f_col) || game_check_map(s_lin1 + 8, f_col);
  }
}

unsigned char spr_move_right(void) {
  unsigned char *f_colint;
  unsigned char *f_col;
  f_colint = &colint[sprite];
  f_col = &col[sprite];
  if (!spr_horizontal_check(*f_col + 1)) {
    ++*f_colint;
    if (*f_colint >= spr_frames[sprite]) {

      if (*f_col < 31) {
        if (spr_horizontal_check(*f_col + 2)) {
          --*f_colint;
          return 1;
        } else {
          ++*f_col;
          *f_colint = 0;
          if (*f_col > SCR_COLS_M) {
            *f_col = SCR_COLS_M;
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

unsigned char spr_move_left(void) {
  unsigned char *f_colint;
  unsigned char *f_col;
  f_colint = &colint[sprite];
  f_col = &col[sprite];
  if (!spr_horizontal_check(*f_col)) {
    --*f_colint;
    if (*f_colint >= spr_frames[sprite]) {

      if (*f_col < 31) {
        if (spr_horizontal_check(*f_col - 1)) {
          ++*f_colint;
          return 1;
        } else {
          --*f_col;
          *f_colint = spr_frames[sprite] - 1;
          ;
          if (*f_col == 255) {
            *f_col = 0;
            return 1;
          }
        }
      }
    }
  }

  return 0;
}
/*
unsigned char spr_move_left(void) {
  --colint[sprite];
  if (colint[sprite] >= spr_frames[sprite]) {
    s_lin1 = lin[sprite];
    if (col[sprite] > 0) {

      s_col1 = col[sprite] - 1;
      if (s_lin1 % 8 == 0) {
        v0 = game_check_map(s_lin1, s_col1) ||
             game_check_map(s_lin1 + 8, s_col1);
      } else
         {
          v0 = game_check_map(s_lin1, s_col1) ||
               game_check_map(s_lin1 + 8, s_col1) ||
               game_check_map(s_lin1 + 16, s_col1);
        }

      if (v0) {
        ++colint[sprite];
        return 1;
      } else {
        --col[sprite];
        colint[sprite] = spr_frames[sprite] - 1;

        if (col[sprite] == 255) {
          col[sprite] = 0;
          return 1;
        }
      }
    }
  }
  return 0;
}
*/
unsigned char spr_move_left_f(void) {
  --colint[sprite];
  if (colint[sprite] >= spr_frames[sprite]) {
    --col[sprite];
    spr_clr = 1;
    colint[sprite] = spr_frames[sprite] - 1;
    return 1;
  }
  return 0;
}

unsigned char spr_move_right_f(void) {

  ++colint[sprite];
  if (colint[sprite] >= spr_frames[sprite]) {
    ++col[sprite];
    spr_clr = 1;
    colint[sprite] = 0;
    return 1;
  }
  return 0;
}

void spr_clear_scr() {
  unsigned char i;
  unsigned char j;
  for (i = 0; i < NIRV_TOTAL_SPRITES; ++i) {
    NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
  }
  intrinsic_halt();

  intrinsic_di();
  for (i = 0; i < 8; ++i) {

    for (j = 1; j < 10; ++j) {
      NIRVANAP_fillT_raw(INK_BLACK || PAPER_BLACK, j << 4, i << 1);
    }

    for (j = 1; j < 10; ++j) {
      NIRVANAP_fillT_raw(INK_BLACK || PAPER_BLACK, j << 4, 30 - (i << 1));
    }

    intrinsic_ei();
    intrinsic_halt();
    intrinsic_di();
  }
  intrinsic_ei();
}

unsigned char spr_paint_player(void) {

  NIRVANAP_spriteT(NIRV_SPRITE_P1, tile[INDEX_P1] + colint[INDEX_P1],
                   lin[INDEX_P1] + 16, col[INDEX_P1]);
  spr_back_repaint();
  return 0;
}

void spr_paint(void) {

  NIRVANAP_spriteT(nirv_sprite_index, tile[sprite] + colint[sprite],
                   lin[sprite] + GAME_OFFSET_Y, col[sprite]);
  if (spr_clr) {
    // Method0 Using printC
    v2 = s_lin0 + GAME_OFFSET_Y;
    v3 = s_col0 + 1;
    NIRVANAP_paintC(attrib_hl, v2, s_col0);
    NIRVANAP_paintC(attrib_hl, v2, v3);
    v2 = v2 + 8;
    NIRVANAP_paintC(attrib_hl, v2, s_col0);
    NIRVANAP_paintC(attrib_hl, v2, v3);

    /*
    //Method1 Should be faster but not
     NIRVANAP_fillT(map_paper_clr, s_lin0 + GAME_OFFSET_Y, s_col0);
    */

    /*
    //Method2 Using background repaint, restores background
    spr_back_repaint();
    */

    /*
    //Method3 Using PrintQ we don't need to clear the pixels if we paint with
    the same paper+ink f_byte_src0 = &btiles[0]; f_byte_src1 = &btiles[0]+32;
        index1 = index0+32;
        v2 = s_lin0 + GAME_OFFSET_Y;
        v3 = s_col0 + 1;
        NIRVANAP_printQ(f_byte_src0 , f_byte_src1,  v2, s_col0);
        NIRVANAP_printQ(f_byte_src0 , f_byte_src1,  v2, v3);


        v2 = v2+8;
        NIRVANAP_printQ(f_byte_src0 , f_byte_src1,  v2, s_col0);
        NIRVANAP_printQ(f_byte_src0 , f_byte_src1,  v2, v3);
    */
  }
}

void spr_destroy(unsigned char f_sprite) __z88dk_fastcall {
  spr_count--;
  s_lin0 = lin[f_sprite];
  s_col0 = col[f_sprite];
  index0 = spr_calc_index(s_lin0, s_col0);
  NIRVANAP_spriteT(f_sprite, TILE_EMPTY, 0, 0);
  spr_back_repaint();

  tile[f_sprite] = TILE_EMPTY;
  col[f_sprite] = 0;
  lin[f_sprite] = 0;
  class[f_sprite] = 0;
  state[f_sprite] = 0;
  state_a[f_sprite] = 0;
}

unsigned char spr_get_tile(unsigned char *f_sprite) __z88dk_fastcall {
  // Search enemy class associated Values
  return spr_get_tile_dir(&spr_tile[*f_sprite], &spr_frames[*f_sprite]);
}

unsigned char spr_get_tile_dir(unsigned char *f_tile, unsigned char *f_inc) {

  if (BIT_CHK(state[sprite], STAT_DIRR)) {
    return *f_tile;
  }
  if (BIT_CHK(state[sprite], STAT_DIRL)) {
    return *f_tile + *f_inc;
  }
  if (BIT_CHK(state_a[sprite], STAT_LDIRR)) {
    return *f_tile;
  }
  if (BIT_CHK(state_a[sprite], STAT_LDIRL)) {
    return *f_tile + *f_inc;
  }
  return *f_tile;
}

void spr_back_repaint(void) {
  index1 = spr_calc_index(s_lin0, s_col0);
  s_lin1 = s_lin0 >> 3;
  s_lin1 = s_lin1 << 3;
  s_lin1 = s_lin1 + 16;

  s_row1 = (s_lin0 >> 3) + 1;
  s_col1 = s_col0;
  spr_draw8(scr_map[index1], s_row1 << 3, s_col1);
  index1++;
  s_col1++;
  spr_draw8(scr_map[index1], s_row1 << 3, s_col1);
  s_lin1 = s_lin1 + 8;
  s_row1++;
  index1 = index1 + 32;
  spr_draw8(scr_map[index1], s_row1 << 3, s_col1);
  index1--;
  s_col1--;
  spr_draw8(scr_map[index1], s_row1 << 3, s_col1);
  // TODO PERFORMANCE  esto no deberia ser necesaro si estoy sobre plaforma
  s_lin1 = s_lin1 + 8;
  s_row1++;
  index1 = index1 + 32;
  spr_draw8(scr_map[index1], s_row1 << 3, s_col1);
  index1++;
  s_col1++;
  spr_draw8(scr_map[index1], s_row1 << 3, s_col1);
}

void spr_turn_horizontal(void) {
  if (BIT_CHK(state[sprite], STAT_DIRR)) {
    spr_set_left();
  } else {
    spr_set_right();
  }
  // state[sprite] = state[sprite];
  tile[sprite] = spr_get_tile(&sprite);
}

void spr_btile_paint_back() {
  unsigned char *f_char;

  map_paper_clr = map_paper | (map_paper >> 3);

  index0 = 48 * 16;
  while (index0 < (48 * 9 * 8)) { // 12*20 btiles

    i = 0;

    // Internal
    while (i < 16) {
      f_char = &btiles[0] + index0 + i + 32; // TODO SPEED UP INC MEMORY ADDRESS
      if ((*f_char & 0x38) ==
          PAPER_BLACK) {          // 00111000 DEFAULT ON HI MEM BTILES
        *f_char = *f_char & 0xC7; // 11000111
        *f_char = *f_char | map_paper;
      }
      ++i;
    }

    index0 = index0 + 48;
  }
  map_paper_last = map_paper;
  game_attribs();
}

void spr_flatten(void) {
  unsigned char i;
  for (i = 0; i < NIRV_TOTAL_SPRITES; ++i) {
    NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
    NIRVANAP_drawT(*SPRITEVAL(i), *SPRITELIN(i), *SPRITELIN(i));
  }
}

void spr_unflattenP1(void) {
  // Only for INDEX_P1
  NIRVANAP_drawT(tile[INDEX_P1], lin[INDEX_P1], col[INDEX_P1]);
}
