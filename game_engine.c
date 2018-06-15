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
#include "game_menu.h"
#include "game_player.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include <arch/zx.h>
#include <arch/zx/nirvana+.h>
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

/* Main Game Loop  */

void game_loop(void) {
  unsigned char flag;
  flag = 0;
  game_round_init();

  while (!game_over) {

    enemy_init(56, 7, GUARDIAN_HOR1, DIR_RIGHT); //TODO
    value_a[0] = 7;//TODO
    value_b[0] = 14;//TODO
    while (!game_round_up && !game_over) {

      /*Enemies turn*/
      enemy_turn();
      /*Player 1 turn*/
      // sprite = SPR_P1;
      player_turn();

      /*Play animatios*/
      if (game_check_time(&anim_time, TIME_ANIM)) {
        //TODO
        // zx_border(INK_BLACK);
        anim_time = zx_clock();
        // if (anim_count)
        //  spr_play_anim();
        switch (flag) {
        case 0:
          zx_print_str(10, 7, "rrrrrrrrrrrrrrrrrrrrr");
          flag = 1;
          break;
        case 1:
          zx_print_str(10, 7, "qqqqqqqqqqqqqqqqqqqqq");
          flag = 2;
          break;
        case 2:
          zx_print_str(10, 7, "ppppppppppppppppppppp");
          flag = 3;
          break;
        case 3:
          zx_print_str(10, 7, "lllllllllllllllllllll");
          flag = 0;
          break;
        }
      }

      /*Each second aprox - update fps/score/phase left/phase advance*/
      if (game_check_time(&frame_time, TIME_EVENT)) {
        frame_time = zx_clock();
        // intrinsic_halt();
        if (game_fps_show)
          game_fps();
      }
      // INGAME

      ++loop_count;
      ++fps;
      game_rotate_attrib();

      if (obj_count == player_coins) {
        game_over = 1;
        zx_border(INK_BLACK);
      }
    }
    if (game_round_up) {
      game_round_up = 0;
      game_world++;
      game_boss_alive = 1;
      game_boss = 0;
      scr_curr = 255;
      player_lin_scr = 0;
      player_col_scr = 0;
      player_col_scr = 2;
      player_lin_scr = 128;
      game_round_init();
    }
  }
}

void game_fps(void) {
  zx_print_ink(INK_WHITE);
  zx_print_int(23, 24, fps);
  fps = 0;
}

void game_draw_screen(void) {

  intrinsic_halt();
  game_boss = 0;
  game_boss_fix = 0;

  spr_count = 0;
  spr_init_effects();
  while (spr_count < SPR_P1) { // TODO SINGLE CLEAR FUNC
    // Clear enemies related tables
    class[spr_count] = 0;
    obj_col[spr_count] = 0;
    obj_lin[spr_count] = 0;
    ++spr_count;
  }
  index1 = 16;
  s_lin1 = 0;
  s_col1 = 2;
  // intrinsic_ei();
  spr_count = 0;

  index1 = 0;
  s_row1 = 1;
  s_lin1 = 16;
  s_col1 = 0;
  obj_count = 0;

  NIRVANAP_stop();
  zx_print_ink(INK_BLACK);
  while (index1 < GAME_SCR_MAX_INDEX) {
    if (scr_map[index1] == TILE_OBJECT) {
      obj_col[obj_count] = s_col1;
      obj_lin[obj_count] = s_lin1;
      ++obj_count;
    }
    game_cell_paint();
    ++index1;
    ++s_col1;
    if ((index1 & 31) == 0) {
      ++s_row1;
      s_lin1 = s_lin1 + 8;
      s_col1 = 0;
    }
  }
  NIRVANAP_start();
  game_update_stats();
}

void game_cell_paint() {
  unsigned char *f_attrib;
  switch (scr_map[index1]) {
  case TILE_EMPTY:
    zx_print_str(s_row1, s_col1, "a");
    f_attrib = attrib0;
    break;
  case TILE_FLOOR:
    zx_print_str(s_row1, s_col1, "b");
    f_attrib = attrib1;
    break;
  case TILE_FLOOR0:
    zx_print_str(s_row1, s_col1, "c");
    f_attrib = attrib2;
    break;
  case TILE_FLOOR1:
    zx_print_str(s_row1, s_col1, "d");
    f_attrib = attrib2;
    break;
  case TILE_FLOOR2:
    zx_print_str(s_row1, s_col1, "e");
    f_attrib = attrib2;
    break;
  case TILE_FLOOR3:
    zx_print_str(s_row1, s_col1, "f");
    f_attrib = attrib2;
    break;
  case TILE_FLOOR4:
    zx_print_str(s_row1, s_col1, "g");
    f_attrib = attrib2;
    break;
  case TILE_FLOOR5:
    zx_print_str(s_row1, s_col1, "h");
    f_attrib = attrib2;
    break;
  case TILE_FLOOR6:
    zx_print_str(s_row1, s_col1, "i");
    f_attrib = attrib2;
    break;
  case TILE_FLOOR7:
    zx_print_str(s_row1, s_col1, "j");
    f_attrib = attrib2;
    break;
  case TILE_WALL:
    zx_print_str(s_row1, s_col1, "k");
    f_attrib = attrib4;
    break;
  case TILE_CHAIN:
    zx_print_str(s_row1, s_col1, "l");
    f_attrib = attrib5;
    break;
  case TILE_DEADLY1:
    zx_print_str(s_row1, s_col1, "m");
    f_attrib = attrib6;
    break;
  case TILE_DEADLY2:
    zx_print_str(s_row1, s_col1, "n");
    f_attrib = attrib7;
    break;
  case TILE_OBJECT:
    zx_print_str(s_row1, s_col1, "o");
    f_attrib = attrib8;
    break;
  }

  NIRVANAP_paintC(f_attrib, s_lin1, s_col1);
}

void game_end() {}

void game_add_enemy(unsigned char enemy_tile_index) __z88dk_fastcall {

  if (enemy_tile_index != INDEX_ENEMY_BOSS1) {
    tmp0 = 0;
    while (tmp0 <= GAME_TOTAL_INDEX_CLASSES) {
      tmp1 = tmp0 * 3;
      if (spr_init[tmp1] == enemy_tile_index) {
        enemy_init(s_lin1, s_col1, spr_init[tmp1 + 1], spr_init[tmp1 + 2]);
        tmp0 = 0xFF;
      } else {
        ++tmp0;
      }
    }
  } else {
    if (game_boss == 0 && game_boss_alive) {
      boss_lin = s_lin1;
      boss_col = s_col1;
      // zx_print_ink(INK_WHITE);
      // z80_delay_ms(200);
      switch (game_world) {
      case 0:
        boss_tile = TILE_ENEMY_BOSS1;
        break;
      case 1:
        boss_tile = TILE_ENEMY_BOSS1 + 4 + 12;
        break;
      case 2:
        boss_tile = TILE_ENEMY_BOSS1 + 8 + 12;
        break;
      case 3:
        boss_tile = TILE_ENEMY_BOSS1 + 12 + 12;
        break;
      }
      // boss_tile = TILE_ENEMY_BOSS1 + (game_world * 4);
      boss_stat = 0;
      spr_set_up();
      game_boss = 1;
      game_boss_hit = 6;
      game_song_play_start = 0;
      ay_reset();
      audio_ingame();
    } else {
      if (!game_boss)
        game_boss_fix = 1;
    };
  }
}

void game_print_footer(void) {

  if (game_fps_show) {
    /* phase osd bottom*/
    zx_print_ink(INK_WHITE);
    zx_print_str(23, 20, "LPS:");
  }
  game_update_stats();
}

void game_print_score(void) {
  /*
  zx_print_ink(INK_WHITE);
  zx_print_paper(PAPER_BLACK);
  zx_print_int(20, 23, player_score);
  zx_print_int(0, 14, game_score_top); // SCORE TOP
  */
}

void game_cls() {
  NIRVANAP_stop();
  zx_paper_fill(INK_BLACK | PAPER_BLACK);
  zx_print_ink(INK_WHITE);
  NIRVANAP_start();
}

void game_update_stats(void) {
  /*
  zx_print_ink(INK_WHITE);
  zx_print_chr(20, 3, player_lives);
  zx_print_chr(22, 9, player_coins);

  zx_print_ink(INK_RED);
  zx_print_chr(20, 10, player_vita);

  if (game_boss) {
    zx_print_ink(INK_MAGENTA);
    for (tmp0 = 0; tmp0 < 6; ++tmp0) {
      if (tmp0 < game_boss_hit) {
        zx_print_str(19, 13 + tmp0, "*");
      } else {
        zx_print_str(19, 13 + tmp0, " ");
      }
    }

    // zx_print_chr(1, 16, game_boss_hit);
  }

  if (player_keys[0]) {
    zx_print_ink(INK_WHITE);
    zx_print_str(22, 2, "]");
  }
  if (player_keys[1]) {
    zx_print_ink(INK_RED);
    zx_print_str(22, 3, "]");
  }
  if (player_keys[2]) {
    zx_print_ink(INK_GREEN);
    zx_print_str(22, 4, "]");
  }
  if (player_keys[3]) {
    zx_print_ink(INK_CYAN);
    zx_print_str(22, 5, "]");
  }
  */
}

void game_tick(void) {
  ++curr_time;
  zx_isr();
}

void game_start_timer(void) {
  NIRVANAP_ISR_HOOK[0] = 205;                                // call
  z80_wpoke(&NIRVANAP_ISR_HOOK[1], (unsigned int)game_tick); // game_tick
}

void game_round_init(void) {

  /* screen init */
  /*PHASE INIT*/

  loop_count = 0;
  zx_set_clock(0);
  frame_time = 0;

  /* Phase Draw Start */
  // spr_draw_clear();
  /*Draw Platforms*/

  spr_init_effects();
  game_print_header();
  game_print_footer();
  spr_page_map();
  ay_reset();
  // audio_level_start();
  game_draw_screen();

  game_paint_attrib(&attrib_hl, 0, 32, 144);

  zx_print_str(17, 9, "CENTRAL CAVERN");

  game_paint_attrib(&attrib_red, 0, 10, 144 + 8);
  game_paint_attrib(&attrib_osd, 10, 32, 144 + 8);
  zx_print_str(18, 0, "AIR ----------------------------");
  zx_print_ink(INK_YELLOW);

  zx_print_str(20, 0, "HIGH SCORE 000000   SCORE 000000");
  zx_print_ink(INK_MAGENTA);
  //zx_print_str(22, 0, "DEMO PARA EL CANAL DE JAVI ORTIZ");

  game_song_play_start = 0;

  // ay_reset();
  // ay_fx_play(ay_effect_12);
  /* Player(s) init */
  if (!game_over) {
    player_init(player_lin_scr, player_col_scr, TILE_P1_RIGHT);
  }

  game_update_stats();
  // z80_delay_ms(50);

  if (!game_debug) {
    zx_print_str(12, 6, "ROUND START");
    tmp0 = 11;
    game_paint_attrib(&attrib_osd, 6, (6 + tmp0), (12 << 3) + 8);
    game_colour_message(12, 6, 6 + tmp0, 200, 0);
  }
  audio_ingame();
}

void game_print_header(void) {

  // zx_print_ink(INK_RED);
  // zx_print_str(0, 11, "$%|");
  // zx_print_ink(INK_WHITE);
  /* Print score */
  game_print_score();
}

unsigned char game_check_map(unsigned char f_lin, unsigned char f_col) {
  // TODO A SINGLE FUNCTION TO SAVE BYTES
  index1 = spr_calc_index(f_lin, f_col);
  return game_check_cell(&index1);
}

unsigned char game_check_cell(unsigned int *f_index) __z88dk_fastcall {
  unsigned char f_tile;

  // OUT OFF SCREEN
  if (*f_index > GAME_SCR_MAX_INDEX) {
    return 1;
  }

  f_tile = scr_map[*f_index];

  f_tile = player_pick_deadly(f_tile);
  f_tile = player_pick_item(f_tile, *f_index);



  if (f_tile != TILE_WALL) {
    return 0;
  } else {
    return f_tile;
  }
}

void game_paint_attrib(unsigned char *f_attrib[], char f_start,
                       unsigned char f_end, unsigned char f_lin) {
  for (tmp_uc = f_start; tmp_uc < f_end; ++tmp_uc) {
    NIRVANAP_paintC(f_attrib, f_lin, tmp_uc);
  }
}

void game_colour_message(unsigned char f_row, unsigned char f_col,
                         unsigned char f_col2, unsigned int f_microsecs,
                         unsigned char skip) {
  unsigned int entry_time;

  tmp = 1;
  frame_time = zx_clock();
  entry_time = zx_clock();
  while (tmp && !game_check_time(&entry_time, f_microsecs)) {
    if (game_check_time(&frame_time, 5)) {
      // ROTATE ATTRIB ARRAY
      frame_time = zx_clock();
      if (game_over) {
        game_paint_attrib(&attrib_hl, f_col, f_col2, (f_row << 3) + 8);
        game_rotate_attrib();
      } else {
        game_paint_attrib(&attrib_osd, f_col, f_col2, (f_row << 3) + 8);
        game_rotate_attrib_osd();
      }
    }
    if (skip) {
      while ((joyfunc1)(&k1) != 0)
        tmp = 0;
    };
  };
  if (game_over) {
    // Clear Message
    tmp1 = f_col2 - f_col;
    for (tmp0 = 0; tmp0 < tmp1; tmp0++) {
      zx_print_str(f_row, f_col + tmp0, " "); // SPACE
    }
  } else {
    tmp1 = f_col2 - f_col;
    s_lin0 = f_row << 3;        //* 8; // TODO OPTIMIZE
    s_col1 = (f_col >> 1) << 1; // (x*2)/2
    for (tmp0 = 0; tmp0 < tmp1; tmp0 = tmp0 + 2) {
      s_col0 = s_col1 + tmp0;
      spr_back_repaint();
    }
  }
}

unsigned char game_check_time(unsigned int *start, unsigned char lapse) {
  if (zx_clock() - *start > lapse) {
    return 1;
  } else {
    return 0;
  }
}

void game_boss_clear() {}

void game_rotate_attrib(void) {

  tmp_uc = attrib8[3];
  attrib8[3] = attrib8[2];
  attrib8[2] = attrib8[1];
  attrib8[1] = attrib8[0];
  attrib8[0] = tmp_uc;
  for (tmp_uc = 0; tmp_uc < 8; ++tmp_uc) {
    if (obj_col[tmp_uc] != 0) {
      NIRVANAP_paintC(attrib8, obj_lin[tmp_uc], obj_col[tmp_uc]);
    }
  }
}

void game_rotate_attrib_osd(void) {

  tmp_uc = attrib_osd[3];
  attrib_osd[3] = attrib_osd[2];
  attrib_osd[2] = attrib_osd[1];
  attrib_osd[1] = attrib_osd[0];
  attrib_osd[0] = tmp_uc;
}

void game_attribs() {
  // TODO ESTO DEBE QUEDAR EN BANK6
  attrib0[0] = map_paper | BRIGHT | INK_BLUE | PAPER_BLACK;
  attrib0[1] = map_paper | BRIGHT | INK_BLUE | PAPER_BLACK;
  attrib0[2] = map_paper | BRIGHT | INK_BLUE | PAPER_BLACK;
  attrib0[3] = map_paper | BRIGHT | INK_MAGENTA | PAPER_BLACK;

  attrib1[0] = map_paper | INK_RED;
  attrib1[1] = map_paper | BRIGHT | INK_RED;
  attrib1[2] = map_paper | INK_MAGENTA;
  attrib1[3] = map_paper | BRIGHT | INK_MAGENTA;

  attrib2[0] = map_paper | INK_RED;
  attrib2[1] = map_paper | BRIGHT | INK_RED;
  attrib2[2] = map_paper | INK_MAGENTA;
  attrib2[3] = map_paper | BRIGHT | INK_MAGENTA;

  attrib3[0] = map_paper | BRIGHT | INK_RED;
  attrib3[1] = map_paper | INK_RED;
  attrib3[2] = map_paper | INK_MAGENTA;
  attrib3[3] = map_paper | BRIGHT | INK_MAGENTA;

  attrib4[0] = map_paper | BRIGHT | INK_WHITE | PAPER_RED;
  attrib4[1] = map_paper | BRIGHT | INK_YELLOW | PAPER_RED;
  attrib4[2] = map_paper | BRIGHT | INK_WHITE | PAPER_RED;
  attrib4[3] = map_paper | BRIGHT | INK_YELLOW | PAPER_RED;

  attrib5[0] = map_paper | BRIGHT | INK_GREEN;
  attrib5[1] = map_paper | BRIGHT | INK_GREEN;
  attrib5[2] = map_paper | BRIGHT | INK_CYAN;
  attrib5[3] = map_paper | BRIGHT | INK_CYAN;
  // TILE_DEADLY1
  attrib6[0] = map_paper | BRIGHT | INK_YELLOW;
  attrib6[1] = map_paper | BRIGHT | INK_GREEN;
  attrib6[2] = map_paper | BRIGHT | INK_GREEN;
  attrib6[3] = map_paper | BRIGHT | INK_GREEN;
  // TILE_DEADLY2
  attrib7[0] = map_paper | BRIGHT | INK_BLUE;
  attrib7[1] = map_paper | INK_CYAN;
  attrib7[2] = map_paper | BRIGHT | INK_CYAN;
  attrib7[3] = map_paper | BRIGHT | INK_WHITE;

  attrib8[0] = map_paper | BRIGHT | INK_YELLOW | PAPER_BLACK;
  attrib8[1] = map_paper | BRIGHT | INK_RED | PAPER_BLACK;
  attrib8[2] = map_paper | BRIGHT | INK_YELLOW | PAPER_BLACK;
  attrib8[3] = map_paper | BRIGHT | INK_RED | PAPER_BLACK;

  // ATTRIB NORMAL
  attrib[0] = map_paper | BRIGHT | INK_WHITE;
  attrib[1] = map_paper | BRIGHT | INK_WHITE;
  attrib[2] = map_paper | BRIGHT | INK_WHITE;
  attrib[3] = map_paper | BRIGHT | INK_WHITE;

  // ATTRIB HIGHLIGHT
  attrib_hl[0] = map_paper | INK_BLACK | PAPER_YELLOW;
  attrib_hl[1] = map_paper | INK_BLUE | PAPER_YELLOW;
  attrib_hl[2] = map_paper | INK_MAGENTA | PAPER_YELLOW;
  attrib_hl[3] = map_paper | INK_BLACK | PAPER_YELLOW;

  // ATTRIB OSD
  attrib_osd[0] = map_paper | INK_WHITE | PAPER_GREEN;
  attrib_osd[1] = map_paper | INK_WHITE | PAPER_GREEN;
  attrib_osd[2] = map_paper | INK_WHITE | PAPER_GREEN;
  attrib_osd[3] = map_paper | INK_WHITE | PAPER_GREEN;

  attrib_red[0] = map_paper | INK_WHITE | PAPER_RED;
  attrib_red[1] = map_paper | INK_WHITE | PAPER_RED;
  attrib_red[2] = map_paper | INK_WHITE | PAPER_RED;
  attrib_red[3] = map_paper | INK_WHITE | PAPER_RED;
}
