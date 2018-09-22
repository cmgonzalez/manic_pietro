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
void game_tick(void) {
  ++curr_time;
  zx_isr();
  game_highlight_coins();
}

void game_loop(void) {
  player_lives = GAME_START_LIVES;
  player_score = 0;
  game_over = 0;
  game_round_up = 0;
  game_world = 0;
  // scr_curr = 0xFF;
  map_paper = PAPER_BLACK;
  player_lin_scr = GAME_LIN_FLOOR - 24;
  player_col_scr = 2;

  game_round_init();

  while (!game_over) {
    /*Player Init*/

    dirs = 0x00;
    game_playing = 1;
    while (!game_round_up && !game_over) {
      zx_border(INK_RED);
      /// Enemies turn
      enemy_turn();
      // Player 1 turn
      player_turn();

      // NIRVANAP_halt();

      /*Play animatios*/
      if (game_check_time(&anim_time, 3)) {
        // TODO
        anim_time = zx_clock();
        // CONVEYOR ANIM TODO OPTIMIZE ONE FOR SCREEN
        if (game_conveyor_col0 > 0) {
          game_anim_conveyor();
        }
        // game_rotate_attrib_osd();
        // game_paint_attrib(&attrib_osd, 0, 32, 144);
      }

      // game_highlight_coins();

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

      // PRINT EXIT TODO PLAYER BEHIND DOOR
      NIRVANAP_spriteT(6, 79, game_exit_lin, game_exit_col);
      // NIRVANAP_spriteT(7, 79, game_exit_lin, game_exit_col);

      if (obj_count == player_coins) {
        game_flash_exit();
        zx_border(INK_BLACK);
        obj_count = 0xFF;
      }
    }
    if (game_round_up) {
      game_playing = 0;
      game_round_init();
      game_round_up = 0;
      game_playing = 1;
    }
  }
}

void game_fps(void) {
  zx_print_ink(INK_WHITE);
  zx_print_int(23, 24, fps);
  fps = 0;
}

void game_draw_map(void) {

  intrinsic_halt();

  v0 = 0;
  spr_init_effects();
  while (v0 < GAME_INDEX_P1) { // TODO SINGLE CLEAR FUNC
    // Clear enemies related tables
    class[v0] = 0;
    obj_col[v0] = 0;
    obj_lin[v0] = 0;
    ++v0;
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
  // game_cls();
  NIRVANAP_stop();
  zx_print_ink(INK_BLACK);

  game_conveyor_lin = 0;
  game_conveyor_col0 = 0;
  game_conveyor_col1 = 0;
  game_conveyor_tile = 17;

  while (index1 < GAME_SCR_MAX_INDEX) {

    if (scr_map[index1] < ENEMY_START_INDEX) {
      if (scr_map[index1] == TILE_OBJECT) {
        obj_col[obj_count] = s_col1;
        obj_lin[obj_count] = s_lin1;
        ++obj_count;
      }
      // DRAW CONVEYORS
      if (game_conveyor_col0 > 0 && game_conveyor_col1 == 0 &&
          scr_map[index1] != TILE_CONVEYOR) {
        game_conveyor_col1 = index1 % 32; // TODO OPTIMIZE
      }
      if (scr_map[index1] == TILE_CONVEYOR) {
        if (game_conveyor_col0 == 0) {
          game_conveyor_col0 = index1 % 32;          // TODO OPTIMIZE
          game_conveyor_lin = 8 + (index1 / 32) * 8; // TODO OPTIMIZE

          if (scr_map[index1 + 1]) {
            game_conveyor_dir = DIR_LEFT;
          } else {
            game_conveyor_dir = DIR_RIGHT;
          }
          scr_map[index1 + 1] = TILE_CONVEYOR;
        }
      }

      if (scr_map[index1] == TILE_EXIT0) {
        game_exit_col = index1 % 32;
        game_exit_lin = 16 + (index1 / 32) * 8;
        scr_map[index1 + 1] = TILE_EXIT1;
        scr_map[index1 + 32] = TILE_EXIT2;
        scr_map[index1 + 33] = TILE_EXIT3;
      }
      // game_cell_paint();
      spr_draw8(scr_map[index1], s_row1 << 3, s_col1);
    } else {
      if (scr_map[index1] < 120) {
        enemy_init();

        value_a[sprite] = scr_map[index1 + 32];
        value_b[sprite] = scr_map[index1 + 33];
        scr_map[index1] = TILE_EMPTY;
        // Clear Adjancent tiles
        scr_map[index1] = TILE_EMPTY;
        scr_map[index1 + 1] = TILE_EMPTY; // NO USE
        scr_map[index1 + 32] = TILE_EMPTY;
        scr_map[index1 + 33] = TILE_EMPTY;
      } else {
        // Willy
        if (scr_map[index1] == 127) {
          player_lin_scr = s_lin1 - 16;
          player_col_scr = s_col1;
          scr_map[index1] = TILE_EMPTY;
          spr_draw8(TILE_EMPTY, s_row1 << 3, s_col1);
        }
      }
    }

    ++index1;
    ++s_col1;
    if ((index1 & 31) == 0) {
      ++s_row1;
      s_lin1 = s_lin1 + 8;
      s_col1 = 0;
    }
  }
  NIRVANAP_start();
  NIRVANAP_halt();
  game_update_stats();
}

void game_cell_paint_index() {

  s_col1 = index1 % 32;
  s_lin1 = (index1 / 32) * 8;
  s_row1 = (s_lin1 >> 3) + 1;
  s_lin1 = s_lin1 + 16;
  // game_cell_paint();
  spr_draw8(scr_map[index1], s_row1 << 3, s_col1);
}

void game_cell_paint() { spr_draw8(scr_map[index1], s_row1 << 3, s_col1); }

void game_end() {}

void game_print_footer(void) {

  if (game_fps_show) {
    /* phase osd bottom*/
    zx_print_ink(INK_WHITE | PAPER_BLACK);
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

void game_update_stats(void) {}

void game_start_timer(void) {
  NIRVANAP_ISR_HOOK[0] = 205;                                // call
  z80_wpoke(&NIRVANAP_ISR_HOOK[1], (unsigned int)game_tick); // game_tick
}

void game_anim_conveyor() {
  unsigned char *f_byte;
  unsigned char *f_byte_src;

  unsigned char f_col;
  unsigned char f_lin1;

  f_byte_src = &btiles[0] + (48 * 2); // TODO CALCULATE ONCE ALL THIS
  i = game_conveyor_lin;
  f_lin1 = i + 8;
  // Rotate
  if (game_conveyor_dir == DIR_LEFT) {
    *f_byte_src = (*f_byte_src << 1) | (*f_byte_src >> 7);
    *f_byte_src = (*f_byte_src << 1) | (*f_byte_src >> 7);
  } else {
    *f_byte_src = (*f_byte_src << 7) | (*f_byte_src >> 1);
    *f_byte_src = (*f_byte_src << 7) | (*f_byte_src >> 1);
  }

  while (i < f_lin1) {
    f_col = game_conveyor_col0;
    f_byte = zx_py2saddr(i) + f_col; // Calculate screen Line

    while (f_col < game_conveyor_col1) {
      *f_byte = *f_byte_src;
      ++f_byte;
      ++f_col;
    }

    f_byte_src = f_byte_src +
                 2; // Increment btile by 2 to get next line of the 8x8 sprite
    i++;
    if (i == game_conveyor_lin + 2) {
      if (game_conveyor_dir == DIR_LEFT) {
        *f_byte_src = (*f_byte_src << 7) | (*f_byte_src >> 1);
        *f_byte_src = (*f_byte_src << 7) | (*f_byte_src >> 1);
      } else {
        *f_byte_src = (*f_byte_src << 1) | (*f_byte_src >> 7);
        *f_byte_src = (*f_byte_src << 1) | (*f_byte_src >> 7);
      }
    }
  }
}

void game_round_init(void) {
  unsigned const char *map_names[] = {
      "Central Cavern",
      "The Cold Room",
      "Menagerie",
      "Abandoned Uranium Workings",
      "Eugene's Lair",
      "Processing Plant",
      "The Vat",
      "Wacky Amoebatrons",
      "The Endorian Forest",
      "Attack of the Mutant Telephones",
      "Ore Refinery",
      "The Warehouse",
      "Solar Power Generator",
      "Amoebatrons Revenge",
      "Miner Willy meets the Kong Beast",
      "Return of the Alien Kong Beast",
      "The Final Barrier",
      "Skylab Landing Bay",
      "The Bank",
      "The Sixteenth Cavern",
  };
  unsigned const char map_lens[] = {
      14, 13, 9,  26, 13, 16, 7,  17, 19, 31,
      12, 13, 21, 19, 32, 30, 17, 18, 9,  20,
  };

  /* screen init */
  /*PHASE INIT*/

  loop_count = 0;
  zx_set_clock(0);
  frame_time = 0;
  player_coins = 0;
  game_conveyor_flag = 0;
  // Fill top LINE
  for (i = 0; i < NIRV_TOTAL_SPRITES; i++) {
    NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
  }

  i = 0;
  while (i < GAME_MAX_OBJECTS) {
    obj_ink[i] = 0;
    ++i;
  }

  i = 0;
  tmp_ui = 0;
  while (i < scr_curr) {
    tmp_ui = tmp_ui + map_lens[i];
    ++i;
  }
  // TODO REMOVE
  zx_print_str(22, 0, "                               ");

  zx_print_ink(INK_BLACK | PAPER_YELLOW);
  zx_print_str(17, 0, "                                ");
  zx_print_str(17, ((32 - map_lens[scr_curr]) >> 1) - 1, map_names[scr_curr]);
  zx_print_ink(INK_WHITE | PAPER_RED | BRIGHT);
  zx_print_str(18, 0, "AIR ------");
  zx_print_ink(INK_WHITE | PAPER_GREEN | BRIGHT);
  zx_print_str(18, 10, "----------------------");
  zx_print_ink(INK_YELLOW);

  zx_print_str(20, 0, "HIGH SCORE 000000   SCORE 000000");
  spr_init_effects();
  ay_reset();
  game_page_map();
  spr_btile_paint_back();

  game_paint_attrib(&attrib_osd, 0, 32, 144);

  game_draw_map();

  game_song_play_start = 0;

  if (!game_over) {
    player_init(player_lin_scr, player_col_scr, TILE_P1_RIGHT);
  }

  game_update_stats();

  audio_ingame();

  zx_print_ink(INK_RED | PAPER_RED);

  zx_print_str(0, 0, "                                ");
  zx_border(INK_RED);
  zx_print_ink(INK_WHITE | PAPER_BLACK);
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
  index1 = *f_index;
  // f_tile = player_pick_deadly(f_tile);
  // f_tile = player_pick_item(f_tile, *f_index);

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
    // TODO REMOVE PAINTC
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
        game_highlight_coins();
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

void game_highlight_coins(void) {
  if (game_playing) {
    i = 0;
    while (i < GAME_MAX_OBJECTS) {
      if (obj_col[i] > 0) {
        attrib_key[0] = obj_ink[i] | game_attrib_osd;
        attrib_key[1] = attrib_key[0];
        attrib_key[2] = attrib_key[0];
        attrib_key[3] = attrib_key[0];
        NIRVANAP_paintC(attrib_key, obj_lin[i], obj_col[i]);
        ++obj_ink[i];
        if (obj_ink[i] >= 8) {
          obj_ink[i] = 0;
        }
        ++i;
      }
    }
  }

  /*
      // TODO PERFOMANCE (ASM)
      if (last_rotated > obj_count) {
        last_rotated = 0;
      }

      for (tmp_uc = last_rotated; tmp_uc < 8; ++tmp_uc) {
        if (obj_col[tmp_uc] > 0) {
          last_rotated = tmp_uc + 1;
          NIRVANAP_paintC(attrib_key, obj_lin[tmp_uc], obj_col[tmp_uc]);
          if (game_attrib_osd == 7) {
            game_attrib_osd = 1;
          }
          ++game_attrib_osd;
          attrib_key[0] = map_paper | game_attrib_osd;
          attrib_key[1] = attrib_key[0];
          attrib_key[2] = attrib_key[0];
          attrib_key[3] = attrib_key[0];

          tmp_uc = 8; // Exit loop
        } else {
          ++last_rotated;
        }
      }
  */
}

void game_rotate_attrib_osd(void) {

  if (game_attrib_osd == 7) {
    game_attrib_osd = 1;
  }
  ++game_attrib_osd;
  tmp = attrib_osd[0];
  attrib_osd[0] = attrib_osd[3];
  attrib_osd[1] = attrib_osd[2];
  attrib_osd[2] = attrib_osd[1];
  attrib_osd[3] = tmp;
}

void game_attribs() {

  // TODO ESTO DEBE QUEDAR EN BANK6

  attrib_key[0] = map_paper | BRIGHT | INK_YELLOW | PAPER_BLACK;
  attrib_key[1] = map_paper | BRIGHT | INK_RED | PAPER_BLACK;
  attrib_key[2] = map_paper | BRIGHT | INK_YELLOW | PAPER_BLACK;
  attrib_key[3] = map_paper | BRIGHT | INK_RED | PAPER_BLACK;

  // ATTRIB NORMAL
  attrib[0] = map_paper | BRIGHT | INK_WHITE;
  attrib[1] = map_paper | BRIGHT | INK_WHITE;
  attrib[2] = map_paper | BRIGHT | INK_WHITE;
  attrib[3] = map_paper | BRIGHT | INK_WHITE;

  // ATTRIB HIGHLIGHT
  attrib_hl[0] = map_paper | INK_MAGENTA | PAPER_YELLOW;
  attrib_hl[1] = map_paper | INK_BLACK | PAPER_YELLOW;
  attrib_hl[2] = map_paper | INK_BLUE | PAPER_YELLOW;
  attrib_hl[3] = map_paper | INK_BLACK | PAPER_YELLOW;

  // ATTRIB OSD
  attrib_osd[0] = PAPER_YELLOW | INK_BLACK | BRIGHT;
  attrib_osd[1] = PAPER_YELLOW | INK_BLUE | BRIGHT;
  attrib_osd[2] = PAPER_YELLOW | INK_BLUE | BRIGHT;
  attrib_osd[3] = PAPER_YELLOW | INK_MAGENTA | BRIGHT;
}

void game_page_map(void) {

  unsigned char lv0;
  unsigned char lv1;
  unsigned char vr;
  unsigned int li;
  unsigned int lj;
  unsigned int lk;
  unsigned int l_start;

  unsigned int add_index;
  unsigned int start_index;
  unsigned char l_world;
  // unsigned char l_world_w;
  // unsigned char l_world_h;
  unsigned char l_paper;
  unsigned char l_scr;
  unsigned char l_scr_map;

  // btile page
  unsigned char l_btile[48];

  // Clear Screen
  spr_clear_scr();

  intrinsic_di();
  // Get Map from Bank 6
  l_world = game_world;
  l_scr = scr_curr;
  l_scr_map = (l_world << 4) + l_scr;

  // Read Player start screen on world map
  GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
  IO_7FFD = 0x10 + 6;

  if (l_scr == 255) {
    // l_scr = start_scr0[l_world];
    l_scr_map = (l_world << 4) + l_scr;
  }
  l_paper = paper0[l_scr_map];
  // l_world_w = world0_w[l_world];
  // l_world_h = world0_h[l_world];

  GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
  IO_7FFD = 0x10 + 0;

  // Get Background btiles from Bank 3

  lk = 0;
  l_start = (l_scr >> 1) * 48 * 4;

  while (lk < 48 * 5) {
    GLOBAL_ZX_PORT_7FFD = 0x10 + 3;
    IO_7FFD = 0x10 + 3;
    li = 0;
    while (li < 48) {
      l_btile[li] = hibtiles[l_start + li + lk];
      ++li;
    }
    // TODO SPEED UP BANK3? (WHICH BANK IS THE BEST -> BTILE LOW MEM
    GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
    IO_7FFD = 0x10 + 0;
    // Store btile

    if ((l_scr % 2) == 0) {

      // Pixels
      li = 0;
      while (li < 16) {
        btiles[li + lk] = l_btile[li];
        ++li;
      }
      // Atribs 1 column
      li = 32;
      while (li < 36) {
        btiles[li + lk] = l_btile[li];
        ++li;
      }
      // Atribs 2 column
      li = 40;
      while (li < 44) {
        btiles[li + lk] = l_btile[li];
        ++li;
      }
    } else {

      // Pixels
      li = 0;
      while (li < 16) {
        btiles[li + lk] = l_btile[li + 16];
        ++li;
      }
      // Atribs 1 column
      li = 32;
      while (li < 36) {
        btiles[li + lk] = l_btile[li + 4];
        ++li;
      }
      // Atribs 2 column
      li = 40;
      while (li < 44) {
        btiles[li + lk] = l_btile[li + 4];
        ++li;
      }
    }

    lk = lk + 48; // Next btile
  }
  // Clear Key n Crumb upper row
  li = 192;
  while (li < 384) {
    v0 = li % 48;
    // clean uper pixels col1 n col2
    if (v0 < 16) {
      btiles[li] = 0;
    }
    // Clear upper attibs col1
    if (v0 >= 32 && v0 < 36) {
      btiles[li] = l_paper;
    }
    // Clear upper attibs col2
    if (v0 >= 40 && v0 < 44) {
      btiles[li] = l_paper;
    }
    ++li;
  }
  /*
  // Paint Paper
  lk = 192 + 32;
  while (lk < 384) {
    li = 0;
    while (li < 16) {
      btiles[lk + li] = INK_WHITE | l_paper;
      ++li;
    }
    lk = lk + 48;
  }
  */
  // Move Key
  li = 0;
  while (li < 16) {
    btiles[192 + li] = btiles[li];
    li = li + 2;
  }
  // Clear Empty Tile
  li = 0;
  while (li < 16) {
    btiles[li] = 0;
    li = li + 2;
  }

  // CRUMB 1
  btiles[192 + 5] = btiles[48 + 0];
  btiles[192 + 7] = btiles[48 + 2];
  btiles[192 + 9] = btiles[48 + 4];
  btiles[192 + 11] = btiles[48 + 6];
  btiles[192 + 13] = btiles[48 + 8];
  btiles[192 + 15] = btiles[48 + 10];
  // Atribs
  btiles[192 + 41] = btiles[48 + 32];
  btiles[192 + 42] = btiles[48 + 33];
  btiles[192 + 43] = btiles[48 + 34];
  // CRUMB 2
  btiles[240 + 10] = btiles[48 + 0];
  btiles[240 + 12] = btiles[48 + 2];
  btiles[240 + 14] = btiles[48 + 4];
  btiles[240 + 16] = btiles[48 + 6];
  // Atribs
  btiles[240 + 34] = btiles[48 + 32];
  btiles[240 + 35] = btiles[48 + 33];
  // CRUMB 3
  // Pixels
  btiles[240 + 13] = btiles[48 + 0];
  btiles[240 + 15] = btiles[48 + 2];
  // Atribs
  btiles[240 + 43] = btiles[48 + 32];

  // TODO Optimize Draw Crumb mover llave al ultimo tile y asi aplicar un loop
  /* 1 PX but with more tiles...
    // Pixels
    btiles[192 + 3] = btiles[48 + 0];
    btiles[192 + 5] = btiles[48 + 2];
    btiles[192 + 7] = btiles[48 + 4];
    btiles[192 + 9] = btiles[48 + 6];
    btiles[192 + 11] = btiles[48 + 8];
    btiles[192 + 13] = btiles[48 + 10];
    // Atribs
    btiles[192 + 40] = btiles[48 + 32];
    btiles[192 + 41] = btiles[48 + 33];
    btiles[192 + 42] = btiles[48 + 34];
    btiles[192 + 43] = btiles[48 + 35];
    // Pixels
    btiles[240 + 4] = btiles[48 + 0];
    btiles[240 + 6] = btiles[48 + 2];
    btiles[240 + 8] = btiles[48 + 4];
    btiles[240 + 10] = btiles[48 + 6];
    btiles[240 + 12] = btiles[48 + 8];
    // Atribs
    btiles[240 + 33] = btiles[48 + 32];
    btiles[240 + 34] = btiles[48 + 33];
    btiles[240 + 35] = btiles[48 + 34];
    // Pixels
    btiles[240 + 7] = btiles[48 + 0];
    btiles[240 + 9] = btiles[48 + 2];
    btiles[240 + 11] = btiles[48 + 4];
    btiles[240 + 13] = btiles[48 + 6];
    // Atribs
    btiles[240 + 41] = btiles[48 + 32];
    btiles[240 + 42] = btiles[48 + 33];
    btiles[240 + 43] = btiles[48 + 34];
    // Pixels
    btiles[288 + 8] = btiles[48 + 0];
    btiles[288 + 10] = btiles[48 + 2];
    btiles[288 + 12] = btiles[48 + 4];
    // Atribs
    btiles[288 + 34] = btiles[48 + 32];
    btiles[288 + 35] = btiles[48 + 33];
    // Pixels
    btiles[288 + 11] = btiles[48 + 0];
    btiles[288 + 13] = btiles[48 + 2];
    // Atribs
    btiles[288 + 42] = btiles[48 + 32];
    btiles[288 + 43] = btiles[48 + 33];
    // Pixels
    btiles[336 + 12] = btiles[48 + 0];
    // Atribs
    btiles[336 + 35] = btiles[48 + 32];
    // Calculate the current screen start index in the world map
  */
  lj = 0;
  lk = 0;

  start_index = 0;
  add_index = 0;

  while (lj < l_scr_map) {
    GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
    IO_7FFD = 0x10 + 6;
    add_index = lenght0[lj]; // TODO n LEVELS
    GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
    IO_7FFD = 0x10 + 0;
    start_index = start_index + add_index;
    ++lj;
  }

  for (li = 0; li < GAME_SCR_MAX_INDEX; ++li) {

    // Page in BANK 06 - Note that global variables are in page 0
    GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
    IO_7FFD = 0x10 + 6;
    lv0 = world0[start_index + li];     // TODO n LEVELS
    lv1 = world0[start_index + li + 1]; // TODO n LEVELS
    // Page in BANK 00
    GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
    IO_7FFD = 0x10 + 0;

    if (lv0 < 128) {
      scr_map[lk] = lv0;
      ++lk;
    } else {
      vr = lv0 - 128; // Repeat counter Should be < 128!!
      for (lj = 0; lj < vr; lj++) {
        scr_map[lk] = lv1;
        ++lk;
        if (lk >= GAME_SCR_MAX_INDEX) {
          break;
        }
      }
      ++li;
    }
    if (lk >= GAME_SCR_MAX_INDEX) {
      break;
    }
  }
  scr_curr = l_scr;
  map_paper = l_paper;

  intrinsic_ei();
}

void game_flash_exit() {
  unsigned int li;
  li = 192;
  while (li < 384) {
    v0 = li % 48;
    // Set flash bit on 8x8 exit tiles
    if ((v0 >= 36 && v0 < 40) || (v0 >= 44 && v0 < 48)) {
      btiles[li] = btiles[li] | FLASH;
    }
    ++li;
  }

  spr_draw8(28, game_exit_lin - 8, game_exit_col);
  spr_draw8(29, game_exit_lin - 8, game_exit_col + 1);
  spr_draw8(30, game_exit_lin, game_exit_col);
  spr_draw8(31, game_exit_lin, game_exit_col + 1);
}
