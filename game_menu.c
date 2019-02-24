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


  game_cls();
  menu_logo();
  audio_menu();
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
      if (menu_curr_sel != 1) {
        joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_sinclair1);
        menu_curr_sel = 1;
        audio_select();
        game_paint_attrib(&attrib, s_col, s_col_e, s_row);
      }
      break;
    case 2: // KEYBOARD
      if (menu_curr_sel != 2) {
        joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_keyboard);
        menu_curr_sel = 2;
        audio_select();
        game_paint_attrib(&attrib, s_col, s_col_e, s_row);
      }
      break;
    case 3: // KEMPSTON
      if (menu_curr_sel != 3) {
        joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_kempston);
        menu_curr_sel = 3;
        audio_select();
        game_paint_attrib(&attrib, s_col, s_col_e, s_row);
      }
      break;
    case 4: // DEFINE
      audio_select();
      menu_redefine();
      joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_keyboard);
      menu_curr_sel = 2;
      start_time = zx_clock();
      break;
    case 5: // CODES
      audio_select();
      game_attribs();
      v0 = menu_read_code();
      ay_reset();
      if (v0) {
        NIRVANAP_halt();
        game_tune = 0; // default
        f_input = 0; // Exit Loop
      }
      start_time = zx_clock();
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

      game_cls();
      ay_reset();
      audio_game_start();
      z80_delay_ms(300);
      game_tune = 0; // default
      f_input = 0; // Exit Loop
      break;
    }


    ++i;

    if (game_check_time(&start_time, 1700)) {

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

  menu_clear();
  attrib_osd[0] = PAPER_BLACK | INK_MAGENTA;
  attrib_osd[1] = PAPER_BLACK | INK_MAGENTA | BRIGHT;
  attrib_osd[2] = PAPER_BLACK | INK_RED;
  attrib_osd[3] = PAPER_BLACK | INK_MAGENTA;

  zx_print_str(11, 11, "1 SINCLAIR");
  game_paint_attrib(&attrib, 11, 11+10, (11 * 8) + 8);
  zx_print_str(12, 11, "2 KEYBOARD");
  game_paint_attrib(&attrib, 11, 11+10, (12 * 8) + 8);
  zx_print_str(13, 11, "3 KEMPSTON");
  game_paint_attrib(&attrib, 11, 11+10, (13 * 8) + 8);
  zx_print_str(14, 11, "4 DEFINE");
  game_paint_attrib(&attrib_osd, 11, 11+8, (14 * 8) + 8);
  zx_print_str(15, 11, "5 CODES");
  game_paint_attrib(&attrib_osd, 11, 11+7, (15 * 8) + 8);
  zx_print_ink(INK_CYAN);
  zx_print_str(17, 11, "0 START");
  zx_print_ink(INK_WHITE);
  zx_print_str(23, 9, "2019 NOENTIENDO");
}

void menu_clear() {
  zx_print_ink(PAPER_BLACK | INK_WHITE | BRIGHT);
  //for (v0 = 9; v0 < 23; ++v0) {
  //  game_fill_row(v0, 32);
  //}
  for (v0 = 9; v0 < 20; ++v0) {
    game_fill_row(v0, 32);
    game_paint_attrib(&attrib, 0, 31, (v0 << 3) + 8);
  }
}

void menu_redefine() {


  menu_clear();
  s_row1 = 11;
  zx_print_str(s_row1, 10, "PRESS A KEY");
  game_paint_attrib(&attrib, 0, 31, (s_row1 << 3) + 8);
  s_row1++;
  s_row1++;
  zx_print_str(s_row1, 12, "LEFT");
  k1.left = menu_define_key();
  s_row1++;
  zx_print_str(s_row1, 12, "RIGHT");
  k1.right = menu_define_key();
  s_row1++;
  zx_print_str(s_row1, 12, "JUMP");
  k1.fire = menu_define_key();
  //k1.up = IN_KEY_SCANCODE_DISABLE;
  //k1.down = IN_KEY_SCANCODE_DISABLE;
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
    if ((v1 >= 48 && v1 <= 57) || (v1 >= 30 && v1 <= 39) || (v1 >= 65 && v1 <= 90)) {
      zx_print_char(s_row1, 18, v1);
    }
    if (v1 == 13) {
      zx_print_str(s_row1, 18, "ENTER");
    }
    if (v1 == 32) {
      zx_print_str(s_row1, 18, "SPACE");
    }
    audio_tick();
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
      audio_tick();
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


  zx_print_str(13, 11, "ENTER CODE");
  game_paint_attrib(&attrib, 0, 31, (13 << 3) + 8);
  zx_print_str(13 + 1, 11 + 1, "________");
  game_paint_attrib(&attrib, 0, 31, ((13 + 1) << 3) + 8);

  // Read Key
  v1 = 0;

  valid = 1;

  while (v1 < GAME_CODELEN) {
    code0[v1] = 0;
    v0 = menu_read_key(13 + 1, 11 + 1 + v1);
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
    valid = 0;
    if (menu_mcodechk("DEPURAME")) {
      if (game_debug) {
        game_debug = 0;
      } else {
        game_debug = 1;
      }
      game_code = 0;
      valid = 1;
    }
    if (menu_mcodechk("MATTHEWS")) {
      if (game_mode) {
        game_mode = 0;
        scr_curr = 0;
      } else {
        game_mode = 1;
        scr_curr = 20;
      }
      game_code = 0;
      valid = 2;
    }
    if (menu_mcodechk("CHIMUELO")) {
      player_lives = 255;
      game_code = 1;
      valid = 1;
    }
    if (valid) {
      ay_song_stop();
      audio_coin_noentiendo();
      z80_delay_ms(350);
      if (valid == 2) {
        menu_logo();
      }
      menu_main_print();
      return 0;

    }
  }
  ay_song_stop();
  zx_print_str(16, 13, "NOPSIE");
  game_paint_attrib(&attrib_hl, 13, 13 + 6, (16 << 3) + 8);
  z80_delay_ms(350);
  menu_main_print();

  return 0;
}

void menu_logo() {
  game_cls();
  if (game_mode) {
    // zx_print_str(3, 10, "MANIC PIETRO");
    //game_logo1(1, 16, 4, 8, 4);
    game_img(&logo1[0],3, 16, 4, 8, 4);
  } else {
    // zx_print_str(3, 10, "MANIC  MINER");
    //game_logo1(6, 16, 0, 16, 4); //Back
    game_img(&img1[0],3, 16, 0, 16, 4);

    i = 26;
    while (i <= 30) {
    NIRVANAP_drawT(56,64,i);
      i  = i + 2;
    }

/*
    NIRVANAP_drawT(56,64,26);
    NIRVANAP_drawT(56,64,28);
    NIRVANAP_drawT(56,64,30);
*/
    i = 18;
    while (i <= 30) {
      NIRVANAP_drawT(40,48,i);
      i  = i + 2;
    }
/*
    NIRVANAP_drawT(40,48,18);
    NIRVANAP_drawT(40,48,20);
    NIRVANAP_drawT(40,48,22);
    NIRVANAP_drawT(40,48,24);
    NIRVANAP_drawT(40,48,26);
    NIRVANAP_drawT(40,48,28);
    NIRVANAP_drawT(40,48,30);
*/
    game_img(&logo2[0], 3, 64, 3, 10, 1); //Logo
    //game_logo1(2, 64, 3, 10, 1); //Logo
  }
}

void menu_rotcolor() {
  ++color;
  if (color > 7) {
    color = 0;
  }
}
