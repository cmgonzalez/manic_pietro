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
#include <arch/zx/nirvana+.h>
#include <arch/zx.h>
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

  f_input = 1;
  s_col = 11;
  s_col_e = 11 + 9;
  s_row = 7;
  c = 0;

  map_paper = PAPER_BLACK;

  audio_menu();
  menu_main_print();

  while (f_input) {
    z80_delay_ms(40);
    // in_wait_key();
    c = in_inkey();
    // in_wait_nokey();

    s_row = ((6 + menu_curr_sel) << 3) + 8;
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
    case 4: // CURSOR
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_cursor);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 4;
      break;
    case 5: // DEFINE
      menu_redefine();
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_keyboard);
      // game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
      menu_curr_sel = 2;
      break;
    case 0:
      intrinsic_halt();
      game_cls();
      ay_reset();
      audio_game_start();
      z80_delay_ms(300);
      f_input = 0; // Exit Loop
      break;
    }
    if (c > 0 && c < 5)
      game_paint_attrib(&attrib, s_col, s_col_e, s_row);
  }
}

void menu_main_print(void) {

  unsigned char s_row;
  unsigned char s_col;
  unsigned char s_col_e;
  s_row = 7;
  s_col = 10;
  s_col_e = 20;
  // intrinsic_halt();
  game_cls();
  audio_menu();
  // Manic Pietro Logo
  //NIRVANAP_spriteT(0, TILE_TITLE, 32, 11);
  //NIRVANAP_spriteT(1, TILE_TITLE + 1, 32, 13);
  //NIRVANAP_spriteT(2, TILE_TITLE + 2, 32, 15);
  //NIRVANAP_spriteT(3, TILE_TITLE + 3, 32, 17);
  game_attribs();

  zx_print_str(3, 9, "MANIC PIETRO");
  game_paint_attrib(&attrib, 9, 21, (3 << 3) + 8);





  zx_print_str(s_row, s_col, "1 SINCLAIR");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "2 KEYBOARD");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "3 KEMPSTON");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "4 CURSOR");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  zx_print_str(s_row, s_col, "5 DEFINE");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  ++s_row;
  game_paint_attrib(&attrib, s_col, s_col_e + 4, (s_row << 3) + 8);
  s_row = s_row + 2;
  zx_print_str(s_row, s_col, "0 START");
  game_paint_attrib(&attrib, s_col, s_col_e, (s_row << 3) + 8);
  s_row = s_row + 8;
  zx_print_ink(INK_CYAN);

  zx_print_str(s_row, 8, "2018 NOENTIENDO");
  game_paint_attrib(&attrib, 0, 31, (s_row << 3) + 8);

  zx_print_str(23, 2, "INT.RELEASE DON'T DISTRIBUTE");
}

void menu_redefine() {

  for (v0 = 5; v0 < 14; ++v0) {
    game_paint_attrib(&attrib, 0, 31, (v0 << 3) + 8);
  }
  zx_paper_fill(INK_BLACK | PAPER_BLACK);
  intrinsic_halt();

  zx_print_str(10, 10, "LEFT");
  k1.left = menu_define_key();
  zx_print_str(11, 10, "RIGHT");
  k1.right = menu_define_key();
  zx_print_str(12, 10, "JUMP");
  k1.fire = menu_define_key();

  k1.up = IN_KEY_SCANCODE_DISABLE;
  k1.down = IN_KEY_SCANCODE_DISABLE;
  //game_fill_row(12, 32);
  menu_main_print();
}

unsigned int menu_define_key() {

  while (1) {
    in_wait_key();
    v1 = in_inkey();
    in_wait_nokey();
    v0 = 0;
    while (v0 < 38) {
      if (v1 == key_map[v0]) {
        return scan_map[v0];
      }
      ++v0;
    }
  }
  // return 0;
}
