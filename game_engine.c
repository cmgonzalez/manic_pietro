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
}

void game_loop(void) {

  player_lives = GAME_START_LIVES;
  player_score = 0;
  game_over = 0;
  game_round_up = 0;
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
      zx_border(map_border);
      /// Enemies turn
      enemy_turn();
      // Player 1 turn
      player_turn();

      // Cicling events

      if ((loop_count & 3) == 0) {
        game_key_paint();
      }
      if ((loop_count & 15) == 0) {
        if (game_conveyor_col0 > 0) {
          game_anim_conveyor();
        }
        NIRVANAP_spriteT(NIRV_SPRITE_DOOR, SPRITE_DOOR, game_exit_lin,
                         game_exit_col);
      }

      if (game_check_time(&air_time, 25)) {
        game_anim_air();
        air_time = zx_clock();
        NIRVANAP_halt();
      }
      // Each second aprox - update fps/score/phase left/phase advance
      if (game_check_time(&frame_time, TIME_EVENT)) {

        frame_time = zx_clock();
        // intrinsic_halt();
        if (game_debug)
          game_fps();
      }

      ++loop_count;
      ++fps;
    }
    if (game_round_up) {
      game_playing = 0;
      game_round_init();
      NIRVANAP_spriteT(NIRV_SPRITE_DOOR, SPRITE_DOOR, game_exit_lin,
                       game_exit_col);
      NIRVANAP_halt();
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
  unsigned char val0;
  unsigned char val1;
  unsigned char f_exit;

  intrinsic_halt();
  zx_print_ink(INK_WHITE); // For Debug
  v0 = 0;
  while (v0 < GAME_INDEX_P1) {
    // TODO SINGLE CLEAR FUNC
    // Clear enemies related tables
    class[v0] = 0;
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

  game_conveyor_lin = 0;
  game_conveyor_col0 = 0;
  game_conveyor_col1 = 0;
  NIRVANAP_stop();
  intrinsic_di();
  attrib_hl[0] = map_paper | (map_paper >> 3);
  attrib_hl[1] = attrib_hl[0];
  attrib_hl[2] = attrib_hl[0];
  attrib_hl[3] = attrib_hl[0];
  f_exit = 0;
  while (index1 < GAME_SCR_MAX_INDEX) {

    val0 = scr_map[index1];
    val1 = scr_map[index1 + 1];

    if (val0 < 32) {
      // Tiles
      spr_draw8(val0, s_row1 << 3, s_col1);
      val0 = tile_class[val0];
      val1 = tile_class[val1];

      // Keys
      if (val0 == TILE_OBJECT) {
        obj_col[obj_count] = s_col1;
        obj_lin[obj_count] = s_lin1;
        ++obj_count;
      }

      // Conveyor Continue
      if (game_conveyor_col0 > 0 && game_conveyor_col1 == 0 &&
          val0 != TILE_CONVEYOR) {
        game_conveyor_col1 = s_col1;
      }
      // Conveyor Start
      if (val0 == TILE_CONVEYOR &&
          (val1 == TILE_EMPTY || val1 == TILE_CONVEYOR)) {
        if (game_conveyor_col0 == 0) {
          game_conveyor_col0 = s_col1;
          game_conveyor_lin = s_lin1 - 8;
          if (val1) {
            game_conveyor_dir = DIR_LEFT;
          } else {
            game_conveyor_dir = DIR_RIGHT;
          }
          scr_map[index1 + 1] = TILE_CONVEYOR;
        }
      }
    } else {
      // Sprites 16x16
      // Door
      if (val0 == SPRITE_EXIT && !f_exit) {
        f_exit = 1;
        game_exit_col = s_col1; //(index1 & 31); // index1 % 32;
        game_exit_lin = s_lin1; // 16 + ((index1 >> 5) << 3);
        scr_map[index1 + 1] = SPRITE_EXIT;
        scr_map[index1 + 32] = SPRITE_EXIT;
        scr_map[index1 + 33] = SPRITE_EXIT;
      }
      // Enemy
      if (val0 >= ENEMY_START_INDEX && val0 <= 120) {
        spr_draw8(TILE_EMPTY, s_row1 << 3, s_col1); // Draw a Block with Paper
        enemy_init();
        scr_map[index1] = TILE_EMPTY;
      }
      // Player
      if (val0 == 127) {
        player_lin_scr = s_lin1 - 16;
        player_col_scr = s_col1;
        scr_map[index1] = TILE_EMPTY;
        spr_draw8(TILE_EMPTY, s_row1 << 3, s_col1);
      }
    }
    // Increment Counters
    ++index1;
    ++s_col1;
    // if ((index1 & 31) == 0) {
    if (s_col1 == 32) {
      ++s_row1;
      s_lin1 = s_lin1 + 8;
      s_col1 = 0;
    }
  }
  if (scr_curr == 19) {
    game_img1();
  }
  intrinsic_ei();
  NIRVANAP_start();
}

void game_img1() {
  unsigned char btile[48];
  unsigned int li;
  unsigned char i;
  unsigned char k;

  k = 0;
  v0 = 16;
  v1 = 0;

  while (k < (16 * 4)) {

    i = 0;
    li = k * 48;
    page(1);
    while (i < 48) {
      btile[i] = img1[li];
      ++i;
      ++li;
    }
    page(0);
    NIRVANAP_halt();
    i = 0;
    li = 48 * 64;
    while (i < 48) {
      btiles[li] = btile[i];
      ++i;
      ++li;
    }

    NIRVANAP_drawT_raw(64, v0, v1);
    if ( v1 == 30 ) {
      v0 = v0 + 16;
      v1 = 0;
    } else {
      v1 = v1 + 2;
    }

    ++k;
  }
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
}

void game_print_score(void) {

  zx_print_ink(INK_YELLOW);
  zx_print_paper(PAPER_BLACK);
  zx_print_int(20, 27, player_score);
  zx_print_int(20, 12, game_score_top); // SCORE TOP
}
void game_print_lives(void) {
  v0 = 0;

  game_fill_row(22, 32);

  while (v0 < player_lives) {
    zx_print_ink(INK_CYAN);
    zx_print_str(22, v0, "<");
    ++v0;
  }
}

void game_cls() {
  NIRVANAP_stop();
  /*
    p_byte = 0x5800;
    while( p_byte < ( 0x5800 + 768 ) )  {
      *p_byte = 0;
      ++p_byte;
    }
  */
  zx_paper_fill(INK_BLACK | PAPER_BLACK);
  zx_border(INK_BLACK);
  intrinsic_di();
  v0 = 0;
  while (v0 <= 8) {
    v1 = 0;
    while (v1 < 31) {
      NIRVANAP_drawT_raw(SPRITE_EMPTY, (v0 * 16) + 8, v1);
      ++v1;
      ++v1;
    }
    ++v0;
  }
  intrinsic_ei();
  NIRVANAP_start();
}

void game_start_timer(void) {
  NIRVANAP_ISR_HOOK[0] = 205;                                // call
  z80_wpoke(&NIRVANAP_ISR_HOOK[1], (unsigned int)game_tick); // game_tick
}

void game_anim_air() {
  if (*air_curr_byte) {
    v0 = *air_curr_byte << 1;
    *air_curr_byte = v0;
    *(air_curr_byte + 256) = v0;
    *(air_curr_byte + 512) = v0;
    *(air_curr_byte + 768) = v0;
  } else {
    if ((unsigned int)air_curr_byte > (unsigned int)air_end_byte) {
      air_curr_byte = air_curr_byte - 1;
    } else {
      player_killed = 1;
    }
  }
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

// TODO move to 128 bank's
unsigned const char game_borders[] = {
    INK_RED,     // 0
    INK_RED,     // 1
    INK_RED,     // 2
    INK_RED,     // 3
    INK_BLUE,    // 4
    INK_RED,     // 5
    INK_GREEN,   // 6
    INK_RED,     // 7
    INK_BLUE,    // 8
    INK_RED,     // 9
    INK_RED,     // 10
    INK_RED,     // 11
    INK_BLUE,    // 12
    INK_YELLOW,  // 13
    INK_RED,     // 14
    INK_RED,     // 15
    INK_RED,     // 16
    INK_BLUE,    // 17
    INK_MAGENTA, // 18
    INK_RED,     // 19
    INK_BLACK,   // 0
    INK_BLUE,    // 1
    INK_BLACK,   // 2
    INK_BLACK,   // 3
    INK_GREEN,   // 4
    INK_BLACK,   // 5
    INK_BLACK,   // 6
    INK_BLACK,   // 7
    INK_BLACK,   // 8
    INK_BLACK,   // 9
    INK_BLACK,   // 10
    INK_BLACK,   // 11
    INK_BLACK,   // 12
    INK_BLACK,   // 13
    INK_BLACK,   // 14
    INK_BLACK,   // 15
    INK_BLACK,   // 16
    INK_BLACK,   // 17
    INK_BLACK,   // 18
    INK_BLACK,   // 19
};

void game_round_init(void) {
  unsigned const char *map_names[] = {
      "         Central Cavern",          // 0
      "          The Cold Room",          // 1
      "          The Menagerie",          // 2
      "   Abandoned Uranium Workings",    // 3
      "         Eugene's Lair",           // 4
      "       Processing Plant",          // 5
      "            The Vat",              // 6
      "Miner Willy meets the Kong Beast", // 7
      "        Wacky Amoebatrons",        // 8
      "       The Endorian Forest",       // 9
      "Attack of the Mutant Telephones",  // 10
      " Return of the Alien Kong Beast",  // 11
      "          Ore Refinery",           // 12
      "       Skylab Landing Bay",        // 13
      "             The Bank",            // 14
      "      The Sixteenth Cavern",       // 15
      "         The Warehouse",           // 16
      "      Amoebatrons Revenge",        // 17
      "     Solar Power Generator",       // 18
      "        The Final Barrier",        // 19
      "       Frozen Central Pipe",
      "        Super Luizo Bro",
      "    Pietro meets Monkey Kong",
      "         Chuntey Crew",
      "     Kong Jr Watch the Game",
      "          Dr. Errazking",
      "         No tengo nombre",
      "            Stage 27",
      "            Stage 28",
      "            Stage 29",
      "            Stage 30",
      "            Stage 31",
      "            Stage 32",
      "            Stage 33",
      "            Stage 34",
      "            Stage 35",
      "            Stage 36",
      "            Stage 37",
      "            Stage 38",
      "           Noentiendo",

  };

  // TODO Just use start to simplify

  ay_reset();
  spr_clear_scr();
  for (i = 0; i < NIRV_TOTAL_SPRITES; i++) {
    NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
  }
  NIRVANAP_halt();
  NIRVANAP_stop();

  /* screen init */
  /*PHASE INIT*/
  map_border = game_borders[scr_curr];
  loop_count = 0;
  zx_set_clock(0);
  frame_time = 0;
  player_coins = 0;
  air_curr_byte = (unsigned int)air_start_byte; // Remaing Air anim
  // Init Sprites

  // Read Tiles from bank 3

  // Page Player from bank 3

  // Coin HIGHLIGHT init
  key_last = 0;
  key_ink = INK_YELLOW;
  i = 0;
  while (i < GAME_MAX_OBJECTS) {
    obj_col[i] = 0xFF;
    obj_lin[i] = 0xFF;
    ++i;
  }

  zx_border(map_border);
  zx_print_ink(map_border | (map_border << 3));
  game_fill_row(0, 32);
  if (scr_curr > 0) {
    zx_print_ink(INK_BLACK | PAPER_YELLOW);
    game_fill_row(17, 32);
  }

  if (scr_curr < 20) {
    game_tileset = 0;
    game_mode = 0;
  } else {
    game_tileset = 1;
    game_mode = 1;
  }
  game_page_map();

  // Copy player tile

  // Hack player colors TODO  Configureable

  game_copy_sprite_color_reset();

  if (scr_curr < 20) {
    if (map_paper == PAPER_RED) {
      spr_init_cin0 = 2;
      spr_init_cout0 = 3;
    }
  } else {
    if (map_paper == PAPER_BLUE) {
      spr_init_cin0 = PAPER_BLACK | INK_BLUE;
      spr_init_cout0 = PAPER_BLACK | INK_WHITE;
      spr_init_cin1 = PAPER_BLACK | INK_RED;
      spr_init_cout1 = PAPER_BLACK | INK_GREEN;
      spr_init_cin2 = PAPER_RED | INK_YELLOW;
      spr_init_cout2 = PAPER_GREEN | INK_WHITE;
    }

    // if (map_paper == (PAPER_WHITE | BRIGHT)) {

    if (map_paper == PAPER_WHITE) {
      // spr_init_bright = BRIGHT;

      spr_init_cin0 = PAPER_BLACK | INK_YELLOW;
      spr_init_cout0 = PAPER_BLACK | INK_BLACK;

      spr_init_cin1 = PAPER_BLACK | INK_BLUE;
      spr_init_cout1 = PAPER_BLACK | INK_BLUE;

      spr_init_cin2 = PAPER_BLACK | INK_RED;
      spr_init_cout2 = PAPER_BLACK | INK_MAGENTA;

      spr_init_cin3 = PAPER_RED | INK_YELLOW;
      spr_init_cout3 = PAPER_BLUE | INK_BLACK;

      // spr_init_cin3 = PAPER_BLACK | INK_YELLOW;
      // spr_init_cout3 = PAPER_BLACK | INK_BLACK;
    }
  }
  game_tile_cnt = game_copy_sprite_std(0, 8);

  spr_btile_paint_back();
  key_attrib[0] = map_paper | key_ink;
  key_attrib[1] = key_attrib[0];
  key_attrib[2] = key_attrib[0];
  key_attrib[3] = key_attrib[0];

  game_song_play_start = 0;
  audio_ingame();
  NIRVANAP_start();
  game_draw_map();
  game_flash_exit(!FLASH);

  if (!game_over) {
    player_init(player_lin_scr, player_col_scr, TILE_P1_RIGHT);
    player_killed = 0;
  }

  zx_print_ink(INK_BLACK | PAPER_YELLOW);
  game_fill_row(17, 32);
  zx_print_str(17, 0, map_names[scr_curr]);

  zx_print_ink(INK_WHITE | PAPER_RED | BRIGHT);
  zx_print_str(18, 0, "AIR ------");
  zx_print_ink(INK_WHITE | PAPER_GREEN | BRIGHT);
  zx_print_str(18, 10, "----------------------");
  zx_print_ink(INK_YELLOW);
  zx_print_str(20, 0, "High Score         Score      ");
  game_print_score();
  game_print_lives();

  zx_print_ink(INK_WHITE | PAPER_BLACK);
  NIRVANAP_halt();
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

  f_tile = tile_class[scr_map[*f_index]];
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

  unsigned char li;
  for (li = f_start; li < f_end; ++li) {
    NIRVANAP_paintC(f_attrib, f_lin, li);
    // TODO REMOVE PAINTC
  }
}

void game_colour_message(unsigned char f_row, unsigned char f_col,
                         unsigned char f_col2, unsigned int f_microsecs,
                         unsigned char skip) {
  unsigned int entry_time;

  v0 = 1;
  frame_time = zx_clock();
  entry_time = zx_clock();
  while (v0 && !game_check_time(&entry_time, f_microsecs)) {
    if (game_check_time(&frame_time, 5)) {
      // ROTATE ATTRIB ARRAY
      frame_time = zx_clock();
      if (game_over) {
        game_paint_attrib(&attrib_hl, f_col, f_col2, (f_row << 3) + 8);
      } else {
        game_paint_attrib(&attrib_osd, f_col, f_col2, (f_row << 3) + 8);
        game_rotate_attrib_osd();
      }
    }
    if (skip) {
      while ((joyfunc1)(&k1) != 0)
        v0 = 0;
    };
  };
  if (game_over) {
    // Clear Message
    v1 = f_col2 - f_col;
    for (v0 = 0; v0 < v1; v0++) {
      zx_print_str(f_row, f_col + v0, " "); // SPACE
    }
  } else {
    v1 = f_col2 - f_col;
    s_lin0 = f_row << 3;        //* 8; // TODO OPTIMIZE
    s_col1 = (f_col >> 1) << 1; // (x*2)/2
    for (v2 = 0; v2 < v1; v2 = v2 + 2) {
      s_col0 = s_col1 + v2;
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

void game_key_paint(void) {
  unsigned char li;

  if (game_playing && obj_count > 0) {
    // TODO PERFOMANCE (ASM)
    li = key_last;

    while (li < GAME_MAX_OBJECTS) {
      if (obj_col[li] != 0xFF) {

        NIRVANAP_paintC(key_attrib, obj_lin[li], obj_col[li]);

        key_attrib[0] = map_paper | key_ink;
        key_attrib[1] = key_attrib[0];
        key_attrib[2] = key_attrib[0];
        key_attrib[3] = key_attrib[0];

        ++key_ink;
        if (key_ink > 6) {
          key_ink = 3;
        }
        key_last = li + 1;
        break;
      }
      ++li;
    }

    if (li >= GAME_MAX_OBJECTS) {
      if (obj_count == 4) {

        ++key_ink;
        if (key_ink > 6) {
          key_ink = 3;
        }
      }
      key_last = 0;
    }
  }
}

void game_rotate_attrib_osd(void) {

  if (game_attrib_osd == 7) {
    game_attrib_osd = 1;
  }
  ++game_attrib_osd;
  v0 = attrib_osd[0];
  attrib_osd[0] = attrib_osd[3];
  attrib_osd[1] = attrib_osd[2];
  attrib_osd[2] = attrib_osd[1];
  attrib_osd[3] = v0;
}

void game_attribs() {

  // ATTRIB NORMAL
  attrib[0] = map_paper | INK_YELLOW;
  attrib[1] = map_paper | INK_YELLOW | BRIGHT;
  attrib[2] = map_paper | INK_WHITE;
  attrib[3] = map_paper | INK_YELLOW;

  // ATTRIB HIGHLIGHT
  attrib_hl[0] = map_paper | INK_CYAN | PAPER_RED;
  attrib_hl[1] = map_paper | INK_CYAN | PAPER_RED | BRIGHT;
  attrib_hl[2] = map_paper | INK_WHITE | PAPER_RED;
  attrib_hl[3] = map_paper | INK_CYAN | PAPER_RED;

  // ATTRIB OSD
  attrib_osd[0] = PAPER_YELLOW | INK_BLACK | BRIGHT;
  attrib_osd[1] = PAPER_YELLOW | INK_BLUE | BRIGHT;
  attrib_osd[2] = PAPER_YELLOW | INK_BLUE | BRIGHT;
  attrib_osd[3] = PAPER_YELLOW | INK_MAGENTA | BRIGHT;
}

void game_page_map(void) {
  // TODO review and encapsulate inside logic
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
  unsigned char l_scr0;
  unsigned char l_scr_map;
  unsigned char l_tileset;

  // btile page
  unsigned char l_btile[48];

  intrinsic_di();

  // Get Map from Bank 6
  l_scr = scr_curr;
  l_scr0 = scr_curr;
  if (l_scr > 19) {
    l_scr0 = l_scr - 20;
  }

  l_scr_map = (l_world << 4) + l_scr;
  l_tileset = game_tileset;

  // Read default start screen from world map
  page(6);
  if (l_scr == 255) {
    l_scr_map = (l_world << 4) + l_scr;
  }
  l_paper = paper0[l_scr_map];
  page(0);

  // Store
  index1 = 48 * SPRITE_DOOR;
  li = 0;
  while (li < 48) {
    btiles[index1 + li] = l_btile[li];
    ++li;
  }
  ++lk;

  // Tiles (8x8) @Bank7
  l_start = l_scr0 * 48 * 8;

  lk = 0;
  while (lk < 48 * 8) {
    page(7);

    li = 0;
    while (li < 48) {
      if (l_tileset == 0) {
        l_btile[li] = hitiles1[l_start + li + lk];
      } else {
        l_btile[li] = hitiles2[l_start + li + lk];
      }
      ++li;
    }
    page(0);
    // Store btile in low mem
    li = 0;
    while (li < 48) {
      btiles[lk + li] = l_btile[li];
      ++li;
    }
    lk = lk + 48; // Next btile
  }

  // Get Map data
  lj = 0;
  lk = 0;

  start_index = 0;
  add_index = 0;
  page(6);
  while (lj < l_scr_map) {
    add_index = lenght0[lj]; // TODO n LEVELS
    start_index = start_index + add_index;
    ++lj;
  }
  page(0);
  // TODO IF I REMOVE THESE LINES THE ENGINE FAILS!!! WHY???
  scr_curr = l_scr;
  map_paper = l_paper;
  // END TODO
  for (li = 0; li < GAME_SCR_MAX_INDEX; ++li) {

    // Page in BANK 06 - Note that global variables are in page 0
    page(6);                            // TODO SINGLE READ TO speedup
    lv0 = world0[start_index + li];     // TODO n LEVELS
    lv1 = world0[start_index + li + 1]; // TODO n LEVELS
    // Page in BANK 00
    page(0);

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

  intrinsic_ei();
}

void game_flash_exit(unsigned char f_attrib) {
  unsigned int li;
  // Set Flash bits on Exit Sprite
  li = (48 * (SPRITE_DOOR)) + 32; // 80 ==> TILE EXIT
  i = 0;
  while (i < 16) {
    btiles[li] = btiles[li] & 127;
    btiles[li] = btiles[li] | f_attrib;
    ++i;
    ++li;
  }
  NIRVANAP_spriteT(NIRV_SPRITE_DOOR, SPRITE_DOOR, game_exit_lin, game_exit_col);
  NIRVANAP_halt();
}

void game_copy_sprite_color_reset(void) {
  spr_init_bright = 0;
  spr_init_cin0 = 0;
  spr_init_cout0 = 0;
  spr_init_cin1 = 0;
  spr_init_cout1 = 0;
  spr_init_cin2 = 0;
  spr_init_cout2 = 0;
  spr_init_cin3 = 0;
  spr_init_cout3 = 0;
}

unsigned char game_copy_sprite_std(unsigned char f_hi_sprite,
                                   unsigned char f_low_sprite) {

  game_copy_sprite(f_hi_sprite + 0, f_low_sprite + 0, 0);
  game_copy_sprite(f_hi_sprite + 1, f_low_sprite + 1, 0);
  game_copy_sprite(f_hi_sprite + 2, f_low_sprite + 2, 0);
  game_copy_sprite(f_hi_sprite + 3, f_low_sprite + 3, 0);

  game_copy_sprite(f_hi_sprite + 3, f_low_sprite + 4, 1);
  game_copy_sprite(f_hi_sprite + 2, f_low_sprite + 5, 1);
  game_copy_sprite(f_hi_sprite + 1, f_low_sprite + 6, 1);
  game_copy_sprite(f_hi_sprite + 0, f_low_sprite + 7, 1);
  game_copy_sprite_color_reset();
  return f_low_sprite + 8;
}

void game_copy_sprite(unsigned char f_hi_sprite, unsigned char f_low_sprite,
                      unsigned char f_flip) {
  // copy a btile from bank3
  unsigned char btile[48];
  unsigned int li;
  unsigned char i;
  unsigned char l_tileset;

  l_tileset = game_tileset;

  // Page a Tile
  intrinsic_di();
  page(3);

  li = (48 * f_hi_sprite);
  i = 0;
  while (i < 48) {
    if (l_tileset == 0) {
      btile[i] = hisprites1[li];
    } else {
      btile[i] = hisprites2[li];
    }

    if (i > 31 && btile[i] == spr_init_cin0) {
      btile[i] = spr_init_cout0;
    }
    ++i;
    ++li;
  }
  // Page in BANK 00
  page(0);
  intrinsic_ei();

  // Paint Variants
  i = 32;
  while (i < 48) {
    if (btile[i] == spr_init_cin0) {
      btile[i] = spr_init_cout0;
    } else {
      if (btile[i] == spr_init_cin1) {
        btile[i] = spr_init_cout1;
      } else {
        if (btile[i] == spr_init_cin2) {
          btile[i] = spr_init_cout2;
        } else {
          if (btile[i] == spr_init_cin3) {
            btile[i] = spr_init_cout3;
          }
        }
      }
    }
    btile[i] = (btile[i] | spr_init_bright);
    ++i;
  }

  // Write Local
  li = (48 * f_low_sprite);
  i = 0;
  while (i < 48) {
    if (f_flip) {
      if (i < 32) {
        if ((i & 1) == 0) {
          btiles[li] = reverse(btile[i + 1]);
        } else {
          btiles[li] = reverse(btile[i - 1]);
        }

      } else {
        if (i < 40) {
          btiles[li] = btile[i + 8];
        } else {
          btiles[li] = btile[i - 8];
        }
      }

    } else {
      btiles[li] = btile[i];
    }

    ++i;
    ++li;
  }
}

unsigned char reverse(unsigned char b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

void page(unsigned char bank) {
  GLOBAL_ZX_PORT_7FFD = 0x10 + bank;
  IO_7FFD = 0x10 + bank;
}
