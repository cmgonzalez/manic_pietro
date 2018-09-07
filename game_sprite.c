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

unsigned char spr_chktime(unsigned char *sprite) __z88dk_fastcall {
  if (zx_clock() - last_time[*sprite] >= spr_speed[*sprite]) {
    last_time[*sprite] = zx_clock();
    return 1;
  }
  return 0;
}

void spr_set_up() {
  BIT_SET(*p_state, STAT_JUMP);
  BIT_CLR(*p_state, STAT_FALL);
}
void spr_set_down() {
  BIT_SET(*p_state, STAT_FALL);
  BIT_CLR(*p_state, STAT_JUMP);
}

void spr_set_left() {
  BIT_CLR(*p_state_a, STAT_LDIRR);
  BIT_SET(*p_state_a, STAT_LDIRL);

  BIT_CLR(*p_state, STAT_DIRR);
  BIT_SET(*p_state, STAT_DIRL);
}
void spr_set_right() {
  BIT_CLR(*p_state_a, STAT_LDIRL);
  BIT_SET(*p_state_a, STAT_LDIRR);

  BIT_CLR(*p_state, STAT_DIRL);
  BIT_SET(*p_state, STAT_DIRR);
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
      if (colint[sprite] == spr_frames[sprite] - 1) {
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
      if (colint[sprite] == spr_frames[sprite] - 1) {
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

  unsigned char l_ret;

  if (BIT_CHK(*p_state, STAT_DIRR)) {
    l_ret = spr_move_right();
  }

  if (BIT_CHK(*p_state, STAT_DIRL)) {
    l_ret = spr_move_left();
  }
  /* Stop horizontal if player hit walls */
  /*
  if (sprite == SPR_P1 && l_ret) {

    if ( BIT_CHK(*p_state, STAT_JUMP) || BIT_CHK(*p_state, STAT_FALL) ) {
      NIRVANAP_spriteT(SPR_P1,tile[SPR_P1]+colint[SPR_P1],0,0);
      BIT_CLR(*p_state, STAT_DIRL);
      BIT_CLR(*p_state, STAT_DIRR);
    }
  }
  */

  return l_ret;
}

unsigned char spr_move_right_f(void) {
  unsigned char *f_colint;
  unsigned char *f_col;
  f_colint = &colint[sprite];
  f_col = &col[sprite];

  ++*f_colint;
  if (*f_colint >= spr_frames[sprite]) {

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
  if (*f_colint >= spr_frames[sprite]) {

    s_lin1 = lin[sprite];
    if (*f_col < 31) {

      if (game_check_map(s_lin1 + 14, *f_col + 2)) {
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
  if (colint[sprite] >= spr_frames[sprite]) {
    s_lin1 = lin[sprite];
    if (col[sprite] > 0) {

      if (game_check_map(s_lin1 + 14, col[sprite] - 1)) {
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

unsigned char spr_move_left_f(void) {
  --colint[sprite];
  if (colint[sprite] >= spr_frames[sprite]) {
    s_lin1 = lin[sprite];
    if (col[sprite] > 0) {
      --col[sprite];
      colint[sprite] = spr_frames[sprite] - 1;

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

unsigned char spr_paint_player(void) {
  /* Parpadeo
    if (player_hit) {
      if ((loop_count & 1) == 0) {
        spr_back_repaint();
        NIRVANAP_spriteT(sprite, 0, 0, 0);
        return 1;
      }
    }
  */
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

unsigned char spr_get_tile(unsigned char *f_sprite) __z88dk_fastcall {
  // Search enemy class associated Values
  return spr_get_tile_dir(&spr_tile[*f_sprite], &spr_frames[*f_sprite]);
}

unsigned char spr_get_tile_dir(unsigned char *f_tile, unsigned char *f_inc) {

  if (BIT_CHK(*p_state, STAT_DIRR)) {
    return *f_tile;
  }
  if (BIT_CHK(*p_state, STAT_DIRL)) {
    return *f_tile + *f_inc;
  }
  if (BIT_CHK(*p_state_a, STAT_LDIRR)) {
    return *f_tile;
  }
  if (BIT_CHK(*p_state_a, STAT_LDIRL)) {
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
  // game_cell_paint();
  game_sprite_draw8(scr_map[index1], s_row1 << 3, s_col1);
  index1++;
  s_col1++;
  // game_cell_paint();
  game_sprite_draw8(scr_map[index1], s_row1 << 3, s_col1);
  s_lin1 = s_lin1 + 8;
  s_row1++;
  index1 = index1 + 32;
  // game_cell_paint();
  game_sprite_draw8(scr_map[index1], s_row1 << 3, s_col1);
  index1--;
  s_col1--;
  // game_cell_paint();
  game_sprite_draw8(scr_map[index1], s_row1 << 3, s_col1);
  // TODO PERFORMANCE
  // if ( (s_lin0 >> 3) != 0 ) {
  s_lin1 = s_lin1 + 8;
  s_row1++;
  index1 = index1 + 32;
  // game_cell_paint();
  game_sprite_draw8(scr_map[index1], s_row1 << 3, s_col1);
  index1++;
  s_col1++;
  // game_cell_paint();
  game_sprite_draw8(scr_map[index1], s_row1 << 3, s_col1);
  //}
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
  if (BIT_CHK(*p_state, STAT_DIRR)) {
    spr_set_left();
  } else {
    spr_set_right();
  }
  // state[sprite] = *p_state;
  tile[sprite] = spr_get_tile(&sprite);
}

void spr_btile_paint_back() {
  unsigned char *f_char;

  tmp_ui = 0;
  map_paper_clr = map_paper | (map_paper >> 3);

  while (index0 < (48 * 9 * 8)) { // 12*20 btiles

    i = 0;

    // Internal
    while (i < 16) {
      //tmp = PEEK(&btiles + tmp_ui + tmp0); // TODO PERFOMANCE C POINTERS
      f_char = &btiles[0] + index0 + i + 32;
      if ( (*f_char & 0x38) == map_paper_last ) {   // 00111000
        *f_char = *f_char & 0xC7;                   // 11000111
        *f_char = *f_char | map_paper;
      }

/*
      if ((tmp & 0x38) == map_paper_last) {     // 00111000
        tmp = tmp & 0xC7;                  // 11000111
        tmp = tmp | map_paper; // TODO we can hava a map array for ink to
                               // prevent using the same paper n ink
        POKE(&btiles + tmp_ui + tmp0, tmp); // TODO PERFOMANCE C POINTERS
      }
*/
      ++i;
      // if ( f_half & tmp0 == 8 ) tmp0 = 12;
    }

    index0 = index0 + 48;

  }
  map_paper_last = map_paper & !BRIGHT;
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
