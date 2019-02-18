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
#include <intrinsic.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>

void menu_main() {
  unsigned char f_input;
  unsigned char s_col;
  unsigned char s_col_e;
  unsigned char s_row;
  unsigned char c;

  unsigned int start_time;

  f_input = 1;
  s_col = 12;
  s_col_e = 12 + 9;

  c = 0;
  game_code = 0;
  map_paper = PAPER_BLACK;

  audio_menu();
  game_cls();
  menu_logo();
  game_attribs();

  menu_main_print();
  i = 0;
  start_time = zx_clock();
  color = 0;
  while (f_input) {
    z80_delay_ms(20);
    // in_wait_key();
    c = in_inkey();
    // in_wait_nokey();

    s_row = ((10 + menu_curr_sel) << 3) + 8;
    // ROTATE PAPER
    // v0 = attrib_hl[0];
    v0 = color << 3;
    ++color;
    if (color == INK_CYAN) {
      ++color;
    }
    if (color > 6) {
      color = 0;
    }

    attrib_hl[0] = (attrib_hl[0] & 0xC7) | v0;
    attrib_hl[1] = (attrib_hl[1] & 0xC7) | v0;
    attrib_hl[2] = (attrib_hl[2] & 0xC7) | v0;
    attrib_hl[3] = (attrib_hl[3] & 0xC7) | v0;
    game_paint_attrib(&attrib_hl, s_col + 1, s_col_e, s_row);
    // 48
    c = c - 48;
    switch (c) {
    case 1: // SINCLAIR
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_sinclair1);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 1;
      break;
    case 2: // KEYBOARD
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_keyboard);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 2;
      break;
    case 3: // KEMPSTON
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_kempston);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 3;
      break;
    /*
    case 4: // CURSOR
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_cursor);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 4;
      break;
    */
    case 4: // DEFINE
      menu_redefine();
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_keyboard);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 2;
      start_time = zx_clock();
      break;
    case 5: // CODES
      game_attribs();
      v0 = menu_read_code();
      ay_reset();
      if (v0) {
        intrinsic_halt();
        game_tune = 0; // default
        f_input = 0; // Exit Loop
      }
      start_time = zx_clock();
      zx_border(INK_BLACK);
      break;
    /* OUT OF MEM
    case 7: // INTRO
      ay_reset();
      game_intro();
      game_cls();
      menu_logo();
      menu_clear();
      menu_main_print();
      audio_menu();
      break;
    */
    case 0:
      intrinsic_halt();
      game_cls();
      ay_reset();
      audio_game_start();
      z80_delay_ms(300);
      game_tune = 0; // default
      f_input = 0; // Exit Loop
      break;
    }
    if (c > 0 && c < 5)
      game_paint_attrib(&attrib, s_col, s_col_e, s_row);
    ++i;

    if (game_check_time(&start_time, 1600)) {

      ay_reset();
      game_atrac = 1;
      c = 0;
      while (!c) {

        c = in_inkey();
        // spr_clear_scr();
        // game_cls();
        game_round_init();
        start_time = zx_clock();
        while (!game_check_time(&start_time, 500) && !c) {
          enemy_turn();
          if (game_check_time(&time_key, 4)) {
            time_key = zx_clock();
            game_key_paint();
            zx_print_ink((map_border << 3) | color);
            zx_print_str(0, 10, "MANIC PIETRO");
            menu_rotcolor();
          }
          if (game_conveyor_col0 > 0) {
            if (game_check_time(&time_conv, 5)) {
              time_conv = zx_clock();
              game_anim_conveyor();
            }
          }
          c = in_inkey();
        }
        scr_curr++;
        if (scr_curr == 20 || scr_curr == 40) {
          c = 0xFF;
        }
      }
      map_paper = PAPER_BLACK;
      if (game_mode) {
        scr_curr = 20;
      } else {
        scr_curr = 0;
      }
      start_time = zx_clock();
      // spr_clear_scr();
      zx_border(INK_BLACK);
      game_attribs();
      game_cls();
      menu_logo();
      menu_clear();
      menu_main_print();
      audio_menu();
      game_atrac = 0;
    }
  }
}

void menu_main_print(void) {

  unsigned char s_row;
  unsigned char s_col;
  unsigned char s_col_e;
  s_row = 11;
  s_col = 11;
  s_col_e = 21;
  // intrinsic_halt();
  menu_clear();

  zx_print_str(s_row, s_col, "1 SINCLAIR");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "2 KEYBOARD");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "3 KEMPSTON");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
/*
  ++s_row;
  zx_print_str(s_row, s_col, "4 CURSOR");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
*/
  ++s_row;
  ++s_row;
  zx_print_str(s_row, s_col, "4 DEFINE");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "5 CODES");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);

  /* OUT OF MEM
  zx_print_str(s_row, s_col, "7 INTRO");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  */
  ++s_row;
  ++s_row;
  zx_print_ink(INK_CYAN);

  zx_print_str(s_row, s_col, "0 START");
  s_row = s_row + 5;
  zx_print_ink(INK_WHITE);
  zx_print_str(s_row, 9, "2019 NOENTIENDO");
}

void menu_clear() {
  zx_print_ink(PAPER_BLACK | INK_WHITE | BRIGHT);
  for (v0 = 9; v0 < 20; ++v0) {
    game_paint_attrib(&attrib, 0, 31, (v0 << 3) + 8);
  }
  for (v0 = 9; v0 < 23; ++v0) {
    game_fill_row(v0, 32);
  }
}

void menu_redefine() {
  s_col1 = 12;
  s_row1 = 11;

  menu_clear();
  zx_print_str(s_row1, s_col1 - 2, "PRESS A KEY");
  game_paint_attrib(&attrib, 0, 31, (s_row1 << 3) + 8);
  s_row1++;
  s_row1++;

  zx_print_str(s_row1, s_col1, "LEFT");
  k1.left = menu_define_key();

  s_row1++;
  zx_print_str(s_row1, s_col1, "RIGHT");
  k1.right = menu_define_key();

  s_row1++;
  zx_print_str(s_row1, s_col1, "JUMP");
  k1.fire = menu_define_key();

  k1.up = IN_KEY_SCANCODE_DISABLE;
  k1.down = IN_KEY_SCANCODE_DISABLE;
  z80_delay_ms(255);
  // game_fill_row(12, 32);
  menu_main_print();
}

unsigned int menu_define_key() {

  while (1) {
    in_wait_key();
    v0 = in_inkey();
    v1 = v0;
    in_wait_nokey();
    if (v1 >= 61 && v1 <= 122) {
      v1 = v1 - 32; // TO UPPER
    }
    if ((v1 >= 30 && v1 <= 39) || (v1 >= 65 && v1 <= 90)) {
      zx_print_char(s_row1, s_col1 + 10, v1);
    }
    if (v1 == 13) {
      zx_print_str(s_row1, s_col1 + 10, "ENTER");
    }
    if (v1 == 32) {
      zx_print_str(s_row1, s_col1 + 10, "SPACE");
    }
    return in_key_scancode(v0);
  }
}

unsigned char menu_read_key(unsigned char row, unsigned char col) {
  unsigned char key;

  while (1) {
    in_wait_key();
    key = in_inkey();
    in_wait_nokey();
    // if (key >= 48 && key <=57) {
    // if (key >= 65 && key <= 90) {
    if (key >= 97 && key <= 122) {
      key = key - 32; // UPPERCASE
      zx_print_char(row, col, key);
      return key;
    }
  }
}

unsigned char menu_mcodechk(unsigned char *s) {

  unsigned char valid;

  i = 0;
  valid = 1;
  while (i < 8) {
    if (code0[i] != *(s + i)) {
      valid = 0;
    }
    ++i;
  }
  return valid;
}

unsigned char menu_read_code() {

  unsigned char checksum0;
  unsigned char checksum1;
  unsigned char valid;
  menu_clear();
  s_row1 = 13;
  s_col1 = 11;
  zx_print_str(s_row1, s_col1, "ENTER CODE");
  game_paint_attrib(&attrib, 0, 31, (s_row1 << 3) + 8);
  zx_print_str(s_row1 + 1, s_col1 + 1, "________");
  game_paint_attrib(&attrib, 0, 31, ((s_row1 + 1) << 3) + 8);

  // Read Key
  v1 = 0;

  valid = 1;

  while (v1 < GAME_CODELEN) {
    code0[v1] = 0;
    v0 = menu_read_key(s_row1 + 1, s_col1 + 1 + v1);
    v2 = 0;
    while ((v2 < 10) && (game_encode[v2] != v0)) {
      ++v2;
    }
    code0[v1] = v0;
    if (v2 == 10) {
      valid = 0;
    } else {
      // REAL CODE!
      code1[v1] = v2; // game_encode[v2];
    }

    v1++;
  }

  if (valid) {

    // Regular numeric code
    // parse code
    v1 = 0;
    v2 = 0; // Check Sum
    v0 = 100;

    game_start_scr = (10 * code1[5]) + code1[2];

    player_lives = (100 * code1[3]) + (10 * code1[6]) + code1[0];

    checksum0 = (100 * code1[7]) + (10 * code1[4]) + code1[1];

    checksum1 = code1[5] + code1[2] + code1[0] + code1[6] + code1[3];

    if (checksum0 == checksum1) {
      if (game_start_scr <= 40 && player_lives > 0) {
        if (game_start_scr < 20) {
          game_mode = 0;
        } else {
          game_mode = 1;
        }
        ay_song_stop();
        audio_coin_noentiendo();
        zx_border(INK_GREEN);
        z80_delay_ms(350);
        scr_curr = game_start_scr;
        game_code = 1;
        return 1;
      };
    } else {
      game_start_scr = 20;
      player_lives = GAME_START_LIVES;
    }

  } else {
    // Could be a magic code...

    if (menu_mcodechk("DEPURAME")) {

      ay_song_stop();
      audio_coin_noentiendo();
      zx_border(INK_MAGENTA);
      z80_delay_ms(350);

      if (game_debug) {
        game_debug = 0;
      } else {
        game_debug = 1;
      }

      game_code = 0;
      menu_main_print();
      return 0;
    }
    if (menu_mcodechk("MATTHEWS")) {
      ay_song_stop();
      audio_coin_noentiendo();
      zx_border(INK_MAGENTA);
      z80_delay_ms(350);

      if (game_mode) {
        game_mode = 0;
        scr_curr = 0;
      } else {
        game_mode = 1;
        scr_curr = 20;
      }
      menu_logo();
      game_code = 0;
      menu_main_print();
      return 0;
    }
    if (menu_mcodechk("CHIMUELO")) {
      ay_song_stop();
      audio_coin_noentiendo();
      zx_border(INK_MAGENTA);
      z80_delay_ms(350);

      player_lives = 255;
      game_code = 1;
      menu_main_print();
      return 0;
    }
  }
  ay_song_stop();
  s_row1 = 16;
  s_col1 = 13;
  zx_print_str(s_row1, s_col1, "NOPSIE");
  game_paint_attrib(&attrib_hl, s_col1, s_col1 + 6, (s_row1 << 3) + 8);
  z80_delay_ms(350);
  zx_border(INK_RED);
  menu_main_print();

  return 0;
}

void menu_logo() {
  game_cls();
  if (game_mode) {
    // zx_print_str(3, 10, "MANIC PIETRO");
    game_logo1(1, 16, 4, 8, 4);
  } else {
    // zx_print_str(3, 10, "MANIC  MINER");
    game_logo1(2, 16, 0, 16, 4);
  }
}

void menu_rotcolor() {
  ++color;
  if (color > 7) {
    color = 0;
  }
}
