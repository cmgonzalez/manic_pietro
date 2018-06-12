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
//#include "game_sound.h"
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

unsigned char spr_chktime(unsigned char *sprite) __z88dk_fastcall {
  if (zx_clock() - last_time[*sprite] >= sprite_speed[*sprite]) {
    last_time[*sprite] = zx_clock();
    return 1;
  }
  return 0;
}

void spr_set_up(unsigned char *f_state) __z88dk_fastcall {
  BIT_SET(*f_state, STAT_JUMP);
  BIT_CLR(*f_state, STAT_FALL);
}
void spr_set_down(unsigned char *f_state) __z88dk_fastcall {
  BIT_SET(*f_state, STAT_FALL);
  BIT_CLR(*f_state, STAT_JUMP);
}

void spr_set_left(unsigned char *f_state) __z88dk_fastcall {
  BIT_SET(*f_state, STAT_DIRL);
  BIT_CLR(*f_state, STAT_DIRR);
}
void spr_set_right(unsigned char *f_state) __z88dk_fastcall {
  BIT_SET(*f_state, STAT_DIRR);
  BIT_CLR(*f_state, STAT_DIRL);
}

unsigned char spr_move_up(void) {
  unsigned char f_check;

  if (lin[sprite] <= 16) {

    return 1;

  } else {
    s_lin1 = lin[sprite] - SPRITE_LIN_INC;
    f_check = (s_lin1 >> 3) != (s_lin0 >> 3);

    if (f_check) {

      if (colint[sprite] == 0) {
        if (game_check_map(s_lin1, col[sprite])) {
          return 1;
        }
      }
      if (colint[sprite] == sprite_frames[sprite] - 1) {
        if (game_check_map(s_lin1, col[sprite] + 1)) {
          return 1;
        }
      }

      if (game_check_map(s_lin1, col[sprite]) ||
          game_check_map(s_lin1, col[sprite] + 1)) {
        return 1;
      }
    }

    lin[sprite] = s_lin1;
    return 0;
  }
}

unsigned char spr_move_down(void) {
  unsigned char f_check;

  if (lin[sprite] >= GAME_LIN_FLOOR) {
    lin[sprite] = GAME_LIN_FLOOR;
    return 1;
  } else {
    s_lin1 = lin[sprite] + SPRITE_LIN_INC + 14;
    f_check = (s_lin1 >> 3) != (s_lin0 >> 3);
    if (f_check) {
      if (colint[sprite] == 0) {
        if (game_check_map(s_lin1, col[sprite])) {
          return 1;
        }
      }
      if (colint[sprite] == sprite_frames[sprite] - 1) {
        if (game_check_map(s_lin1, col[sprite] + 1)) {
          return 1;
        }
      }
      if (game_check_map(s_lin1, col[sprite]) ||
          game_check_map(s_lin1, col[sprite] + 1)) {
        return 1;
      }
    }
    lin[sprite] = s_lin1 - 14;
    return 0;
  }
}

unsigned char spr_move_horizontal(void) {
  if (BIT_CHK(state[sprite], STAT_DIRR)) {
    return spr_move_right();
  } else {
    if (BIT_CHK(state[sprite], STAT_DIRL)) {
      return spr_move_left();
    }
  }
  return 0;
}

unsigned char spr_move_right_f(void) {
  unsigned char *f_colint;
  unsigned char *f_col;
  f_colint = &colint[sprite];
  f_col = &col[sprite];

  ++*f_colint;
  if (*f_colint == sprite_frames[s_class]) {

    s_lin1 = lin[sprite];
    if (*f_col < 31) {
      ++*f_col;
      *f_colint = 0;
      if (*f_col > SCR_COLS_M) {
        *f_col = SCR_COLS_M;
        return 1;
      }
    }
  }

  return 0;
}

unsigned char spr_move_right(void) {
  unsigned char *f_colint;
  unsigned char *f_col;
  f_colint = &colint[sprite];
  f_col = &col[sprite];

  ++*f_colint;
  if (*f_colint == sprite_frames[s_class]) {

    s_lin1 = lin[sprite];
    if (*f_col < 31) {

      if (game_check_map(s_lin1 + 8, *f_col + 2)) {
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

  return 0;
}

unsigned char spr_move_left(void) {
  --colint[sprite];
  if (colint[sprite] == 255) {
    s_lin1 = lin[sprite];
    if (col[sprite] > 0) {

      if (game_check_map(s_lin1 + 8, col[sprite] - 1)) {
        ++colint[sprite];
        return 1;
      } else {
        --col[sprite];
        colint[sprite] = sprite_frames[s_class] - 1;

        if (col[sprite] == 255) {
          col[sprite] = 0;
          return 1;
        }
      }
    }
  }
  return 0;
}

unsigned char spr_move_left_f(void) {
  --colint[sprite];
  if (colint[sprite] == 255) {
    s_lin1 = lin[sprite];
    if (col[sprite] > 0) {
      --col[sprite];
      colint[sprite] = sprite_frames[s_class] - 1;

      if (col[sprite] == 255) {
        col[sprite] = 0;
        return 1;
      }
    }
  }
  return 0;
}

void spr_clear_scr() {
  unsigned char i;
  unsigned char j;
  for (i = 0; i < 8; ++i) {
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

void spr_page_map(void) {
  /*
    unsigned char v0;
    unsigned char v1;
    unsigned char vr;
    unsigned char i;
    unsigned char j;
    unsigned char k;
    unsigned int add_index;
    unsigned int start_index;
    unsigned char l_world;
    unsigned char l_world_w;
    unsigned char l_world_h;
    unsigned char l_paper;
    unsigned char l_scr;
    unsigned char l_scr_map;

    l_world = game_world;
    l_scr = scr_curr;
    l_scr_map = (l_world << 4) + l_scr;

    k = 16;

    spr_clear_scr();
    intrinsic_di();
    // Read Player start screen on world map
    GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
    IO_7FFD = 0x10 + 6;

    if (l_scr == 255) {
      l_scr = start_scr0[l_world];
      l_scr_map = (l_world << 4) + l_scr;
    }
    l_paper = paper0[l_scr_map];
    l_world_w = world0_w[l_world];
    l_world_h = world0_h[l_world];

    GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
    IO_7FFD = 0x10 + 0;

    // Calculate the current screen start index in the world map
    j = 0;
    start_index = 0;
    add_index = 0;

    while (j < l_scr_map) {
      GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
      IO_7FFD = 0x10 + 6;
      add_index = lenght0[j]; // TODO n LEVELS
      GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
      IO_7FFD = 0x10 + 0;
      start_index = start_index + add_index;
      ++j;
    }
    intrinsic_ei();

    intrinsic_di();
    for (i = 0; i < GAME_SCR_MAX_INDEX; ++i) {

      // Page in BANK 06 - Note that global variables are in page 0
      GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
      IO_7FFD = 0x10 + 6;
      v0 = world0[start_index + i];     // TODO n LEVELS
      v1 = world0[start_index + i + 1]; // TODO n LEVELS
      // Page in BANK 00
      GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
      IO_7FFD = 0x10 + 0;

      // REMEMBER OBJECT PICK
      if (v0 < 128) {
          scr_map[k + 1] = TILE_EMPTY;
          scr_map[k] = game_match_back(k); // TILE_EMPTY;
        ++k;
      } else {
        vr = v0 - 128; // Repeat counter Should be < 128!!

        for (j = 0; j < vr; j++) {

            scr_map[k + 1] = TILE_EMPTY;
            scr_map[k] = game_match_back(k); // TILE_EMPTY;


          ++k;
          if (k >= GAME_SCR_MAX_INDEX) {
            break;
          }
        }
        ++i;
      }
      if (k >= GAME_SCR_MAX_INDEX) {
        break;
      }
    }

    scr_curr = l_scr;
    map_width = l_world_w;
    map_heigth = l_world_h;
    map_paper = l_paper;

    spr_init_effects();
    if (map_paper_last != map_paper) {
      spr_btile_paint_back();
    }

    intrinsic_ei();
    NIRVANAP_start();
    zx_print_ink(INK_YELLOW);
    if (game_debug) {
      zx_print_str(23, 12, "SCR:");
      zx_print_chr(23, 12 + 4, scr_curr);
    }
    */
}

unsigned char spr_paint_player(void) {

  if (player_hit) {
    if ((loop_count & 1) == 0) {
      spr_back_repaint();
      NIRVANAP_spriteT(sprite, 0, 0, 0);
      return 1;
    }
  }

  s_col1 = col[SPR_P1];
  s_lin1 = lin[SPR_P1];

  if ((s_lin1 != s_lin0) || (s_col1 != s_col0)) {
    /* Column or lin Movement */
    s_tile1 = tile[SPR_P1] + colint[SPR_P1];

    // NIRVANAP_fillT_raw(map_paper_clr, s_lin0+16, s_col0);
    NIRVANAP_spriteT(sprite, s_tile1, s_lin1 + GAME_OFFSET_Y, s_col1);
    spr_back_repaint();
    return 1;
  } else {
    s_tile0 = *SPRITEVAL(sprite);
    s_tile1 = tile[SPR_P1] + colint[SPR_P1];

    if (s_tile1 != s_tile0) {
      /* Internal Movement, no clean needed */
      NIRVANAP_spriteT(sprite, s_tile1, s_lin1 + 16, s_col1);
      return 0;
    }
  }
  return 0;
}

unsigned char spr_paint(void) {

  s_col1 = col[sprite];
  s_lin1 = lin[sprite];

  if ((s_lin1 != s_lin0) || (s_col1 != s_col0)) {
    /* Column or lin Movement */
    // Speed UP hack
    s_tile1 = tile[sprite] + colint[sprite];
    /*
        if ((s_lin1 & 7) == 0) {
          spr_back_repaint();
        } else {
          NIRVANAP_fillT(map_paper_clr, s_lin0, s_col0);
        }
    */
    NIRVANAP_fillT(map_paper_clr, s_lin0 + GAME_OFFSET_Y, s_col0);
    NIRVANAP_spriteT(sprite, s_tile1, s_lin1 + GAME_OFFSET_Y, s_col1);
    return 1;
  } else {
    s_tile0 = *SPRITEVAL(sprite);
    s_tile1 = tile[sprite] + colint[sprite];
    if (s_tile1 != s_tile0) {
      /* Internal Movement, no clean needed */
      NIRVANAP_spriteT(sprite, s_tile1, s_lin1 + 16, s_col1);
      return 0;
    }
  }
  return 0;
}

void spr_destroy(unsigned char f_sprite) __z88dk_fastcall {
  spr_count--;
  s_lin0 = lin[f_sprite];
  s_col0 = col[f_sprite];
  index0 = spr_calc_index(s_lin0, s_col0);
  s_tile0 = TILE_EMPTY;
  NIRVANAP_spriteT(f_sprite, s_tile0, 0, 0);
  spr_back_repaint();

  tile[f_sprite] = s_tile0;
  col[f_sprite] = 0;
  lin[f_sprite] = 0;
  class[f_sprite] = 0;
  state[f_sprite] = 0;
  state_a[f_sprite] = 0;
}

unsigned char spr_tile(unsigned char *f_sprite) __z88dk_fastcall {

  tmp0 = 0;
  while (tmp0 < GAME_TOTAL_CLASSES) {
    tmp1 = tmp0 * 3;
    if (spr_map_tile[tmp1] == class[*f_sprite]) {
      return spr_tile_dir(&spr_map_tile[tmp1 + 1], f_sprite,
                          &spr_map_tile[tmp1 + 2]);
    }
    ++tmp0;
  }
  return 0;
}

unsigned char spr_tile_dir(unsigned char *f_tile, unsigned char *f_sprite,
                           unsigned char *f_inc) {

  if (BIT_CHK(state[*f_sprite], STAT_DIRR)) {
    return *f_tile;
  }
  if (BIT_CHK(state[*f_sprite], STAT_DIRL)) {
    return *f_tile + *f_inc;
  }
  if (BIT_CHK(state_a[*f_sprite], STAT_LDIRR)) {
    return *f_tile;
  }
  if (BIT_CHK(state_a[*f_sprite], STAT_LDIRL)) {
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
  game_cell_paint();
  index1++;
  s_col1++;
  game_cell_paint();
  s_lin1 = s_lin1 + 8;
  s_row1++;
  index1 = index1 + 32;
  game_cell_paint();
  index1--;
  s_col1--;
  game_cell_paint();
  if ((s_lin0 & 2) != 0) {
    s_lin1 = s_lin1 + 8;
    s_row1++;
    index1 = index1 + 32;
    game_cell_paint();
    index1++;
    s_col1++;
    game_cell_paint();
  }
}

void spr_init_effects(void) {
  unsigned char f_anim;
  for (f_anim = 0; f_anim <= SPR_P1; f_anim++) {
    anim_lin[f_anim] = 0xFF;
  }
  anim_count = 0;
}

void spr_add_anim(unsigned char f_lin, unsigned char f_col,
                  unsigned char f_tile, unsigned char f_end,
                  unsigned char f_loops) {
  unsigned char f_anim;

  if (f_col < 31 && f_lin < SCR_LINS) {
    for (f_anim = 0; f_anim < 8; f_anim++) {
      if (anim_lin[f_anim] == 0xFF) {
        intrinsic_halt();
        ++anim_count;
        anim_lin[f_anim] = f_lin;
        anim_col[f_anim] = f_col;
        anim_tile[f_anim] = f_tile;
        anim_loop[f_anim] = f_loops;
        anim_int[f_anim] = 0;
        anim_end[f_anim] = f_end;

        NIRVANAP_drawT_raw(anim_tile[f_anim], anim_lin[f_anim],
                           anim_col[f_anim]);

        index0 = spr_calc_index(f_lin, f_col);

        if (scr_map[index0] == TILE_EMPTY)
          scr_map[index0] = TILE_ANIM + f_anim; // 0xFF;
        break;
      }
    }
  }
}

void spr_play_anim(void) {
  unsigned char f_anim;
  unsigned char f_cnt;

  f_cnt = 0;
  for (f_anim = 0; f_anim < 8; f_anim++) {
    if (anim_lin[f_anim] != 0xFF) {

      if (anim_int[f_anim] < anim_end[f_anim]) {
        if ((f_cnt & 1) == 0) {
          intrinsic_halt();
        }
        NIRVANAP_drawT_raw(anim_tile[f_anim] + anim_int[f_anim],
                           anim_lin[f_anim], anim_col[f_anim]);
        ++f_cnt;
        ++anim_int[f_anim];

      } else {
        if (anim_loop[f_anim] == 0) {
          --anim_count;
          s_col0 = anim_col[f_anim];
          s_lin0 = anim_lin[f_anim];
          index0 = spr_calc_index(s_lin0, s_col0);
          if (scr_map[index0] >= TILE_ANIM) {

            scr_map[index0] = TILE_EMPTY;
          }

          spr_back_repaint();
          // End Animation
          anim_lin[f_anim] = 0xFF;

        } else {
          // loops animation
          --anim_loop[f_anim];
          anim_int[f_anim] = 0xFF;
        }
      }
    }
  }
}


void spr_turn_horizontal(void) {
  if (BIT_CHK(s_state, STAT_DIRR)) {
    spr_set_left(&s_state);
  } else {
    spr_set_right(&s_state);
  }
  state[sprite] = s_state;
  tile[sprite] = spr_tile(&sprite);
}

void spr_btile_paint_back() {
  unsigned char f_tile;
  unsigned char f_paper_c;

  f_tile = 0;
  tmp_ui = 32;
  map_paper_clr = map_paper | (map_paper >> 3);
  while (tmp_ui < (32 + (48 * 12 * 20))) { // 12*20 btiles
    if ((f_tile < 73 && f_tile != 13 && f_tile != 14) ||
        (f_tile >
         TILE_SPECIAL)) { // TODO AN ARRAY WILL BE A MORE ELEGANT SOLUTION

      // f_half = 0;
      tmp0 = 0;
      f_paper_c = map_paper_last;

      if ((f_tile > 56 && f_tile < 65) || (f_tile > 16 && f_tile < 20)) {
        if (map_paper == PAPER_RED) {
          tmp0 = 16;
          map_paper_last_a = map_paper_last;
        }
        if (map_paper_last == PAPER_RED) {
          f_paper_c = map_paper_last_a;
        }
      }

      while (tmp0 < 16) {
        tmp = PEEK(&btiles + tmp_ui + tmp0);
        if ((tmp & 0x38) == f_paper_c) { // 00111000
          tmp = tmp & 0xC7;              // 11000111
          tmp = tmp | map_paper; // TODO we can hava a map array for ink to
                                 // prevent using the same paper n ink
          POKE(&btiles + tmp_ui + tmp0, tmp);
        }
        ++tmp0;
        // if ( f_half & tmp0 == 8 ) tmp0 = 12;
      }
    }
    tmp_ui = tmp_ui + 48;
    ++f_tile;
  }
  map_paper_last = map_paper;
  game_attribs();
}

void spr_flatten(void) {
  unsigned char i;
  for (i = 0; i <= SPR_P1; ++i) {
    s_lin1 = *SPRITELIN(i);
    s_col1 = *SPRITECOL(i);
    s_tile1 = *SPRITEVAL(i);
    NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
    NIRVANAP_drawT(s_tile1, s_lin1, s_col1);
  }
}

void spr_unflattenP1(void) {
  // Only for SPR_P1
  NIRVANAP_drawT(s_tile1, lin[SPR_P1], col[SPR_P1]);
}
