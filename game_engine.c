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

  if (!game_code) {
    player_lives = GAME_START_LIVES;
  }
  game_lives = player_lives;
  player_score = 0;
  game_over = 0;

  // scr_curr = 0xFF;
  map_paper = PAPER_BLACK;
  player_lin_scr = GAME_LIN_FLOOR - 24;
  player_col_scr = 2;
  game_round_up = 1; //Display presentation on 1 round
  game_round_init();
  game_round_up = 0;
  while (!game_over) {
    /*Player Init*/
    // dirs = 0x00;

    while (!game_round_up && !game_over) {
      // zx_border(map_border);
      /// Enemies turn
      
      enemy_turn();
      // Player 1 turn
      player_turn();

      // Cicling events
      // if (game_check_time(&time_crumb, lapse_crumb)) {
      //  time_crumb = zx_clock();
      //  game_crumble();
      //}
      if (game_check_time(&time_key, 4)) {
        time_key = zx_clock();
        game_key_paint();
        if (!dirs) game_aux_key();
      }

      if (game_conveyor_col0 > 0) {
        if (game_check_time(&time_conv, 5)) {
          time_conv = zx_clock();
          game_anim_conveyor();
        }
      }

      if (game_check_time(&time_air, 30)) {
        game_anim_air();
        time_air = zx_clock();
        // Each second aprox - update fps
        if (game_debug && game_check_time(&frame_time, TIME_EVENT)) {
          game_fps();
          frame_time = zx_clock();
        }

      }

      if (scr_curr == 18 && !game_atrac) {
        game_solar_ray();
      }
      ++loop_count;
      ++fps;


    }

    if (game_round_up) {
      zx_print_ink(INK_BLACK | PAPER_YELLOW);
      // Store start of level lives to give on the code...
      game_lives = player_lives;
      game_round_init();
      NIRVANAP_spriteT(NIRV_SPRITE_DOOR, SPRITE_DOOR, game_exit_lin,
                       game_exit_col);
      NIRVANAP_halt();
      game_round_up = 0;
    }
  }
}

void game_fps(void) {
  zx_print_ink(INK_WHITE);
  zx_print_int(22, 24, fps);
  fps = 0;
}

void game_draw_map(void) {
  unsigned char val0;
  unsigned char val1;
  unsigned char f_exit;

  NIRVANAP_stop();
  intrinsic_di();
  zx_print_ink(INK_WHITE); // For Debug
  v0 = 0;
  while (v0 < INDEX_P1) {
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
  obj_count = 0;

  index1 = 0;
  s_row1 = 1;
  s_lin1 = 16;
  s_col1 = 0;

  game_conveyor_lin = 0;
  game_conveyor_col0 = 0;
  game_conveyor_col1 = 0;

  attrib_hl[0] = map_clear;
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
        if (obj_count < GAME_MAX_OBJECTS) {
          obj_col[obj_count] = s_col1;
          obj_lin[obj_count] = s_lin1;
          ++obj_count;
        }
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
      if (val0 >= ENEMY_START_INDEX && val0 <= 123) {
        spr_draw8(TILE_EMPTY, s_row1 << 3, s_col1); // Draw a Block with Paper
        if (spr_count < GAME_MAX_ENEMIES)
          enemy_init();
        scr_map[index1] = TILE_EMPTY;
      }
      // Player
      if (val0 == 127) {
        player_lin_scr = s_lin1 - 16;
        player_col_scr = s_col1;
        value_a[INDEX_P1] = scr_map[index1 + 1];  // Facing Right/Left
        value_b[INDEX_P1] = scr_map[index1 + 32]; // Initial Colint

        scr_map[index1] = TILE_EMPTY;
        scr_map[index1 + 1] = TILE_EMPTY;
        scr_map[index1 + 32] = TILE_EMPTY;
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

void game_solar_ray() {
  unsigned char f_lin;
  unsigned char f_lin_ray;

  f_lin_ray = 0;
  if (col[0] == 23) {
    f_lin_ray = lin[0];
  } else {
    if (col[1] == 23) {
      f_lin_ray = lin[1];
    } else {
      if (col[4] == 23) {
        f_lin_ray = lin[4];
      } else {
        if (col[6] == 23) {
          f_lin_ray = lin[6];
        }
      }
    }
  }

  // Borro rayo
  if (f_lin_ray == 0 && g_ray1) {
    game_solar_ray0();
    g_ray1 = 0;
    f_lin = 24;
    while (f_lin < 128) {
      // NIRVANAP_paintC(attrib_sol1, f_lin + GAME_OFFSET_Y, 23);
      spr_draw8(17, f_lin, 23);
      f_lin = f_lin + 8;
    }
  }

  if (f_lin_ray && !g_ray1) {
    f_lin = 24;
    g_ray1 = f_lin_ray;
    while (f_lin < f_lin_ray) {
      spr_draw8(17, f_lin, 23);
      f_lin = f_lin + 8;
    }
    game_solar_ray1();

    f_lin = f_lin + 8;
    while (f_lin < 128) {
      spr_draw8(0, f_lin, 23);
      f_lin = f_lin + 8;
    }
  }
}

void game_solar_ray1() {

  unsigned char f_col;
  unsigned char f_lin;
  unsigned char f_col0;

  f_col = 22;
  f_col0 = 0;
  f_lin = g_ray1 + 8;
  NIRVANAP_halt();
  while (f_col > 0) {
    // DIBUJO RAYO DESDE EL INICIO
    spr_draw8(16, f_lin, f_col);
    switch (f_col) {
    case 18: // Choca con sprite 5
      if (abs(lin[5] - g_ray1) < 16) {
        f_col = 1;
      }
      break;

    case 13: // Choca con sprite 2
      if (abs(lin[2] - g_ray1) < 16) {
        f_col = 1;
      }
      break;

    case 7: // Choca con sprite 3
      if (abs(lin[3] - g_ray1) < 16) {
        f_col = 1;
      }
      break;
    }
    --f_col;
  }
}
void game_solar_ray0() {

  unsigned char f_col;
  unsigned char f_col0;

  f_col = 22;
  f_col0 = 0;

  while (f_col > 0) {
    // LIMPIO RAYO DESDE EL INICIO
    spr_draw8(0, g_ray1 + 8, f_col);
    --f_col;
  }
}

void game_img( unsigned char *f_img, unsigned char f_page,
               unsigned char f_lin, unsigned char f_col,
               unsigned char f_width, unsigned char f_height) {

  unsigned char i;
  unsigned char *src;
  unsigned char *dest;
  unsigned int len;
  len = f_width * f_height;
  // 8 x 4 = 32 logo1
  // Read Draw
  //NIRVANAP_halt(); /// DONT REMOVE CAUSE HANG!!!!

  i = 0;
  while (i < 8) {
    NIRVANAP_spriteT(i, 0, 0, 0);
    ++i;
  }

  src = f_img;
  dest = &btiles[0];
  intrinsic_di();
  page(f_page);
  memcpy(dest, src, 48 * len);
  page(0);
  intrinsic_ei();
  i = 0;
  s_col1 = f_col;
  s_lin1 = f_lin;

  while (i < (len)) {
    NIRVANAP_drawT(i, s_lin1, s_col1 * 2);
    ++i;
    ++s_col1;
    if ((i % f_width) == 0) {
      s_lin1 = s_lin1 + 16;
      s_col1 = f_col;
    }
  }


}


void game_img1() {

  unsigned int li;
  unsigned char i;
  unsigned char k;
  unsigned char *src;
  unsigned char *dest;

  k = 0;
  v0 = 16;
  v1 = 0;
  intrinsic_di();
  while (k < (16 * 4)) {

    i = 0;
    li = k * 48;

    dest = &btiles[SPRITE_TMP];
    src = &img1[li];

    page(3);
    memcpy(dest, src, 48);
    page(0);
    NIRVANAP_drawT_raw(96, v0, v1);
    if (v1 == 30) {
      v0 = v0 + 16;
      v1 = 0;
    } else {
      v1 = v1 + 2;
    }

    ++k;
  }
  intrinsic_ei();
}

void game_cell_paint_index() {

  s_col1 = index1 % 32;
  s_lin1 = (index1 / 32) * 8;
  s_row1 = (s_lin1 >> 3) + 1;
  s_lin1 = s_lin1 + 16;
  // game_cell_paint();
  //spr_draw8(scr_map[index1], s_row1 << 3, s_col1);
  game_cell_paint();
}

void game_cell_paint() { spr_draw8(scr_map[index1], s_row1 << 3, s_col1); }


void game_print_score(void) {

  zx_print_ink(INK_YELLOW);
  zx_print_paper(PAPER_BLACK);
  zx_print_int(20, 27, player_score);
  zx_print_int(20, 12, game_score_top); // SCORE TOP
}

void game_print_lives(void) {
  v0 = 0;

  game_fill_row(22, 32);
  if (player_lives <= 10) {
    while (v0 < (player_lives - 1)) {
      zx_print_ink(INK_CYAN);
      zx_print_str(22, v0, "<");
      ++v0;
    }
  } else {
    zx_print_ink(INK_CYAN);
    zx_print_str(22, 0, "< x");
    zx_print_chr(22, 4, player_lives);
  }
}

void game_cls() {
  NIRVANAP_halt();
  spr_clear_scr();
  zx_paper_fill(INK_BLACK | PAPER_BLACK);
  zx_border(INK_BLACK);
}

void game_start_timer(void) {
  NIRVANAP_ISR_HOOK[0] = 205;                                // call
  z80_wpoke(&NIRVANAP_ISR_HOOK[1], (unsigned int)game_tick); // game_tick
}

void game_anim_air() {

  if (!*air_curr_byte) {
    if ((unsigned int)air_curr_byte > (unsigned int)air_end_byte) {
      air_curr_byte = air_curr_byte - 1;
      if (!game_round_up && (unsigned int)air_curr_byte == 21065) {
        audio_time();
      }
    } else {
      BIT_SET(state[INDEX_P1], STAT_KILLED);
    }
  }

  v0 = *air_curr_byte << 1;
  *air_curr_byte = v0;
  *(air_curr_byte + 256) = v0;
  *(air_curr_byte + 512) = v0;
  *(air_curr_byte + 768) = v0;
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
  // TODO move to 128 bank's
  unsigned char i;
  unsigned char game_borders;
  unsigned char map_names[33];
  unsigned char *src;
  unsigned char *dest;

  // TODO Just use start to simplify
  ay_reset();

  // All Black
  zx_border(INK_BLACK);
  zx_print_ink(INK_BLACK | (INK_BLACK << 3));
  game_fill_row(0, 32);
  game_fill_row(17, 32);
  game_fill_row(18, 32);
  game_fill_row(20, 32);
  game_fill_row(22, 32);
  // game_fill_row(10, 32);
  spr_clear_scr();

  NIRVANAP_halt();
  NIRVANAP_stop();
  intrinsic_di();

  i = scr_curr; // local variable to be used during paging
  page(6);
  // Get Border Data
  game_borders = game_borders0[i];
  page(0);
  // Get screen Name
  src = &map_names0[i * 32];
  dest = &map_names[0];
  page(6);
  memcpy(dest, src, 32);
  page(0);
  // Tile Classes
  if (i < 20) {
    src = &tile_class1[0];
  } else {
    src = &tile_class2[0];
  }
  dest = &tile_class[0];
  page(6);
  memcpy(dest, src, 32);
  page(0);
  // Enemy attribs
  if (i < 20) {
    src = &spr_init1[0];
  } else {
    src = &spr_init2[0];
  }
  dest = &spr_init[0];
  page(6);
  memcpy(dest, src, GAME_ENEMY_MAX_CLASSES * GAME_ENEMY_CLASS_LEN);
  page(0);

  map_names[32] = '\0';
  map_border = game_borders;
  /* screen init */
  /*PHASE INIT*/
  loop_count = 0;

  zx_set_clock(0);
  frame_time = 0;
  player_coins = 0;
  air_curr_byte = (unsigned char *)air_start_byte; // Remaing Air anim

  // Coin HIGHLIGHT init
  key_last = 0;
  key_ink = INK_YELLOW;
  i = 0;
  while (i < GAME_MAX_OBJECTS) {
    obj_col[i] = 0xFF;
    obj_lin[i] = 0xFF;
    ++i;
  }

  if (scr_curr < 20) {
    // Willy Sets
    game_tileset = 0;
    game_mode = 0;

    game_crumb_start = 20;
    game_crumb_end = 26;
    lapse_crumb = 3;
  } else {
    // Pietro Sets
    game_tileset = 1;
    game_mode = 1;

    game_crumb_start = 9;
    game_crumb_end = 11;
    lapse_crumb = 4;
  }

  game_page_map();
  game_copy_sprite_color_reset();
  game_color_hacks();

  game_tile_cnt = game_copy_sprite_std(0, TILE_P1_RIGHT);

  key_attrib[0] = map_paper | key_ink;
  key_attrib[1] = key_attrib[0];
  key_attrib[2] = key_attrib[0];
  key_attrib[3] = key_attrib[0];

  // game_song_play_start = 0;
  intrinsic_ei();
  NIRVANAP_start();
  NIRVANAP_halt();
  // Round presentation
  if (!game_debug && !game_atrac && game_round_up) {

    audio_round_init();
    game_fill_row(10, 32);
    s_col0 = 1;
    v0 = 0;
    while (s_col0 < 30) {
      if (s_col0 > 0) {
        game_paint_attrib(&attrib, s_col0 - 1, s_col0, (10 << 3) + 8);
        game_paint_attrib(&attrib, s_col0 - 1, s_col0, (11 << 3) + 8);
      }

      if (scr_curr >= 20) {
        zx_print_chr(10, 17, scr_curr + 1 - 20);
        zx_print_str(10, 21, "A");
      } else {
        zx_print_chr(10, 17, scr_curr + 1);
        zx_print_str(10, 21, "B");
      }

      zx_print_str(10, 11, "ROUND");
      zx_print_str(11, 0, &map_names[0]);

      v0++;
      z80_delay_ms(1);
      NIRVANAP_halt();
      if (v0 == 3) {

        NIRVANAP_drawT(SPRITE_EMPTY, 88, s_col0);
        s_col0++;
        v0 = 0;
      }
      NIRVANAP_spriteT(0, 8 + v0, 88, s_col0);
    }

    ay_reset();
    spr_clear_scr();
  }

  // Draw Screen
  game_draw_map();
  // Start Tune
  if (!game_atrac) {
    audio_ingame();
  }

  // Remove Flash from Door
  game_flash_exit(!FLASH);
  zx_border(map_border);
  zx_print_ink(map_border | (map_border << 3));
  game_fill_row(0, 32);
  if (!game_over) {
    player_init(player_lin_scr, player_col_scr, TILE_P1_RIGHT);
    BIT_CLR(state[INDEX_P1], STAT_KILLED);
  }

  zx_print_ink(INK_BLACK | PAPER_YELLOW);
  game_fill_row(17, 32);
  zx_print_str(17, 0, &map_names[0]);

  zx_print_ink(INK_WHITE | PAPER_RED | BRIGHT);
  zx_print_str(18, 0, "AIR ------");
  zx_print_ink(INK_WHITE | PAPER_GREEN | BRIGHT);
  zx_print_str(18, 10, "----------------------");
  zx_print_ink(INK_YELLOW);
  zx_print_str(20, 0, "High Score         Score      ");
  game_print_score();
  if (!game_atrac)
    game_print_lives();

  zx_print_ink(INK_WHITE | PAPER_BLACK);
  fps = 0;
  // Hack
  if (scr_curr == 18) {
    v0 = 16;
    while (v0 < 128) {
      spr_draw8(17, v0, 23);
      v0 = v0 + 8;
    }
  }

  if (game_debug) {
    zx_print_chr(22, 20, scr_curr);
  }

  NIRVANAP_halt();
}

void game_color_hacks() {
  // TODO MOVE THIS TO A PROPER ARRAY

  if (map_paper & BRIGHT) {
    spr_init_bright = BRIGHT;
  }

  if (scr_curr < 20) {
    // Eugene's Lair
    if (map_paper == (PAPER_RED)) {
      spr_init_cin0 = 2;
      spr_init_cout0 = 3;
    }
    // Solar Power
    if (map_paper == PAPER_GREEN) {
      spr_init_cin0 = 5;
      spr_init_cout0 = 1;
    }
  } else {

    /*
      if (map_paper == PAPER_CYAN) {
        spr_init_cin0 = PAPER_BLACK | INK_YELLOW;
        spr_init_cout0 = PAPER_BLACK | INK_WHITE;

        spr_init_cin1 = PAPER_RED | INK_YELLOW;
        spr_init_cout1 = PAPER_BLUE | INK_WHITE;
      }

      */

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

unsigned char game_check_time(unsigned int *start, unsigned int lapse) {
  if (zx_clock() - *start > lapse) {
    return 1;
  } else {
    return 0;
  }
}

void game_key_paint(void) {
  unsigned char *p_col;
  unsigned char *p_lin;
  // TODO SPEEDUP ASM ROUTINE
  v0 = 0;
  p_col = &obj_col[v0];
  p_lin = &obj_lin[v0];
  while (v0 < GAME_MAX_OBJECTS) {

    if (*p_col != 0xFF) {
      v1 = map_paper | key_ink;
      key_attrib[0] = v1;
      key_attrib[1] = v1;
      key_attrib[2] = v1;
      key_attrib[3] = v1;
      NIRVANAP_paintC(key_attrib, *p_lin, *p_col);
      // game_set_attr(*p_lin, *p_col, v1);
      // game_set_attr(*p_lin + 2, *p_col, v1);
      // game_set_attr(*p_lin + 4, *p_col, v1);
      // game_set_attr(*p_lin + 6, *p_col, v1);
    }
    ++v0;
    ++p_col;
    ++p_lin;
    ++key_ink;
    if (key_ink > 6) {
      key_ink = 3;
    }
  }
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
  attrib_osd[0] = PAPER_BLACK | INK_RED | BRIGHT;
  attrib_osd[1] = PAPER_BLACK | INK_MAGENTA | BRIGHT;
  attrib_osd[2] = PAPER_BLACK | INK_CYAN | BRIGHT;
  attrib_osd[3] = PAPER_BLACK | INK_YELLOW | BRIGHT;
  /*
    attrib_sol0[0] = PAPER_GREEN | INK_GREEN;
    attrib_sol0[1] = PAPER_GREEN | INK_GREEN;
    attrib_sol0[2] = PAPER_GREEN | INK_GREEN;
    attrib_sol0[3] = PAPER_GREEN | INK_GREEN;

    attrib_sol1[0] = PAPER_YELLOW | INK_YELLOW;
    attrib_sol1[1] = PAPER_YELLOW | INK_YELLOW | BRIGHT;
    attrib_sol1[2] = PAPER_YELLOW | INK_YELLOW | BRIGHT;
    attrib_sol1[3] = PAPER_YELLOW | INK_YELLOW;
    */
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

  // unsigned int add_index;
  unsigned int start_index;
  unsigned int end_index;

  unsigned char l_scr;
  unsigned char l_scr0;
  // unsigned char l_scr_map;

  // btile page
  // unsigned char l_btile[48];

  unsigned char *src;
  unsigned char *dest;

  // Get Map from Bank 6
  l_scr = scr_curr;
  l_scr0 = scr_curr;
  if (l_scr > 19) {
    l_scr0 = l_scr - 20;
  }

  // Get Map data
  lj = 0;
  lk = 0;

  // TODO IF I REMOVE THESE LINES THE ENGINE FAILS!!! WHY???
  // scr_curr = l_scr;

  // END TODO

  page(6); // TODO SINGLE READ TO speedup
  start_index = lenght0[l_scr];
  end_index = lenght0[l_scr + 1];
  dest = &btiles[0];
  src = &world0[start_index];
  memcpy(dest, src, end_index - start_index);
  page(0);

  for (li = 0; li < GAME_SCR_MAX_INDEX; ++li) {

    lv0 = btiles[li];
    lv1 = btiles[li + 1];

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

  // l_scr_map = (l_world << 4) + l_scr;

  // Tiles (8x8) @Bank7
  l_start = l_scr0 * 48 * 8;

  lk = 0;
  while (lk < 48 * 8) {
    dest = &btiles[SPRITE_TMP];
    if (scr_curr < 20) {
      src = &hitiles1[l_start + lk];
    } else {
      src = &hitiles2[l_start + lk];
    }

    page(7);
    // Retrieve the data from bank7
    memcpy(dest, src, 48);
    page(0);
    // Store btile in low mem
    dest = &btiles[lk];
    src = &btiles[SPRITE_TMP];
    memcpy(dest, src, 48);
    lk = lk + 48; // Next btile
  }
  map_paper = btiles[32] & 0xF8;
  map_clear = map_paper | (map_paper >> 3);
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

  unsigned int li;
  unsigned char i;

  unsigned char *src;
  unsigned char *dest;
  unsigned char *p_btile_tmp;

  // Page a Tile
  intrinsic_di();
  if (scr_curr < 20) {
    src = &hisprites1[48 * f_hi_sprite];
  } else {
    src = &hisprites2[48 * f_hi_sprite];
  }

  // dest = &btile[0];
  p_btile_tmp = &btiles[SPRITE_TMP];
  dest = p_btile_tmp;
  page(3);
  memcpy(dest, src, 48);
  page(0);
  intrinsic_ei();

  // Atrribs Changes
  i = 32;
  while (i < 48) {
    if (*(p_btile_tmp + i) == spr_init_cin0) {
      *(p_btile_tmp + i) = spr_init_cout0;
    } else {
      if (*(p_btile_tmp + i) == spr_init_cin1) {
        *(p_btile_tmp + i) = spr_init_cout1;
      } else {
        if (*(p_btile_tmp + i) == spr_init_cin2) {
          *(p_btile_tmp + i) = spr_init_cout2;
        } else {
          if (*(p_btile_tmp + i) == spr_init_cin3) {
            *(p_btile_tmp + i) = spr_init_cout3;
          }
        }
      }
    }
    *(p_btile_tmp + i) = (*(p_btile_tmp + i) | spr_init_bright);
    ++i;
  }

  // Write Local
  li = (48 * f_low_sprite);
  i = 0;
  while (i < 48) {
    if (f_flip) {
      if (i < 32) {
        if ((i & 1) == 0) {
          v0 = i + 1;
        } else {
          v0 = i - 1;
        }
        btiles[li] = reverse(*(p_btile_tmp + v0));
      } else {
        if (i < 40) {
          v0 = i + 8;
        } else {
          v0 = i - 8;
        }
        btiles[li] = *(p_btile_tmp + v0);
      }

    } else {
      btiles[li] = *(p_btile_tmp + i);
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
//CAUTION BEAUTIFER SCEWS ALL!!!!!
  if (bank == 0) {
    __asm
    ld a, 0xFE
    ld i, a
    __endasm;
  } else {
    __asm
    ld a, 0
    ld i, a
    __endasm;
  }

  GLOBAL_ZX_PORT_7FFD = 0x10 + bank;
  IO_7FFD = 0x10 + bank;
}


void game_shoe() {

  game_cls();
  game_attribs();
  audio_game_over();
  if (scr_curr > 19) {
    NIRVANAP_drawT(90, 96, 15); // Pietro
  } else {
    NIRVANAP_drawT(91, 96, 15); // Willy
  }

  NIRVANAP_drawT(92, 112, 15); // Shoe
  v0 = 16;
  while (v0 <= 94) {
    /*
    v1 = v3;
    while (v1 == v3) {
      v1 = (rand() % 7) + 1;
      v2 = (rand() % 1);
      if (v2) {
        v1 = v1 | BRIGHT;
      }
    }
    v3 = v1;

    // zx_print_chr(22,0,v1);
    NIRVANAP_halt();
    game_set_attr(v0, 15, v1);
    game_set_attr(v0, 16, v1);
    */
    v0 = v0 + 2;
    NIRVANAP_spriteT(0, 93, v0, 15); // Zapato

    z80_delay_ms(25);
  }

  v0 = 1;
  time_conv = zx_clock();

  // CODE GEN
  // game_start_scr =  10 * code1[5] + code1[2];
  // player_lives = 100 * code1[0] + 10 * code1[6] + code1[3];
  // checksum0 = 100 * code1[7] + 10 * code1[4] + code1[1];
  // checksum1 = code1[0] + code1[2] + code1[3] + code1[5] + code1[6];
  // START SCREEN
  code0[5] = scr_curr / 10;
  code0[2] = scr_curr - (code0[5] * 10);
  // LIVES
  code0[3] = game_lives / 100;
  code0[6] = (game_lives - (code0[3] * 100)) / 10;
  code0[0] = game_lives - (code0[3] * 100 + code0[6] * 10);
  // CHECKSUM
  v3 = code0[5] + code0[2] + code0[0] + code0[6] + code0[3];
  code0[7] = v3 / 100;
  code0[4] = (v3 - (code0[7] * 100)) / 10;
  code0[1] = v3 - (code0[7] * 100 + code0[4] * 10);
  zx_print_ink(PAPER_BLACK | INK_WHITE | BRIGHT);
  zx_print_str(20, 8, "CODE : ");
  i = 0;
  while (i < 8) {
    zx_print_char(20, 8 + 7 + i,
                  game_encode[code0[i]]); // 48 to convert to ascii digit
    i++;
  }

  while (v0) {
    game_paint_attrib(&attrib_osd, 8, 12, (6 << 3) + 8);
    zx_print_str(6, 8, "Game");
    game_paint_attrib(&attrib_osd, 20, 24, (6 << 3) + 8);
    zx_print_str(6, 20, "Over");

    v1 = attrib_osd[3];
    attrib_osd[3] = attrib_osd[2];
    attrib_osd[2] = attrib_osd[1];
    attrib_osd[1] = attrib_osd[0];
    attrib_osd[0] = v1;

    v2 = in_inkey();
    if (v2) {
      v0 = 0;
    }
    z80_delay_ms(10);
  }
}
/*
void game_set_attr(unsigned char f_lin, unsigned char f_col,
                   unsigned char f_attrib) {
  unsigned char *pbyte;


  //colonel32 wrote: »
  //Hi Einar. I'm trying to figure out a formula that takes a NIRVANA+ line and
  //column, and returns an 8x2 attribute address. The attribute address is:
  //race_raster + (lin-16)*82 + deltas[col]
  //where 16 <= lin < 200 (lin must be even)
  //and 0 <= col <= 31
  //Since race_raster starts at address 56695:
  //56695 + (lin-16)*82 + deltas[col]
  //In practice, NIRVANA+ uses the lookup table attribs to avoid multiplication,
  //as follows: attribs[lin] + 256*attribs[lin+1] + deltas[col] The lookup table
  //attribs is located at address $fcc8 and deltas is located at address $ff01,
  //therefore: PEEK($fcc8+lin) + 256*PEEK($fcc8+lin+1) + PEEK($ff00+col+1) This
  //is exactly the calculation you see in the code.


  // val0 = 16 + ((f_lin - 16) << 1);
  pbyte = (unsigned char *)(*(attribs + f_lin)) +
          ((*(attribs + f_lin + 1)) << 8) + (*(deltas + f_col));

  *pbyte = f_attrib;
}
*/
void game_crumble() {

  if (!BIT_CHK(state[INDEX_P1], STAT_JUMP) &&
      !BIT_CHK(state[INDEX_P1], STAT_FALL)) {
    s_col1 = col[INDEX_P1];
    index1 = spr_calc_index(lin[INDEX_P1] + 16, s_col1);

    player_crumble();
    ++index1;
    ++s_col1;
    player_crumble();
  }
}

void zx_print_char(unsigned char ui_row, unsigned char ui_col,
                   unsigned char c) {
  unsigned char str[2];
  str[0] = c;
  str[1] = 0;
  zx_print_str(ui_row, ui_col, str);
}

void game_text(unsigned char f_index, unsigned char f_row) {
  unsigned char text_buff[32];
  unsigned char *src;
  unsigned char *dest;
  unsigned char k;
  if (game_lang) {
    src = &texts_en[32 * f_index];
    v0 = 7;
  } else {
    src = &texts_es[32 * f_index];
    v0 = 1;
  }

  dest = &text_buff[0];
  intrinsic_di();
  page(v0); //BANK 7 TEXTS
  memcpy(dest, src, 32);
  page(0);
  intrinsic_ei();

  k = 1;
  v0 = 0;
  s_col1 = 0;
  color = 1;
  while (k) {
    v3 = in_test_key();
    color = 1;
    while (color < 8) {
      zx_print_ink(PAPER_BLACK | color | BRIGHT);
      zx_print_char(f_row, s_col1, text_buff[v0]);
      if (!v3) {
        z80_delay_ms(1);
      }
      ++color;
    }
    if (!v3)
      z80_delay_ms(8);
    ++v0;
    ++s_col1;
    menu_rotcolor();

    //++color;
    //if (color > 7) {
    //  color = 1;
    //}
    if (v0 > 31) {
      k = 0;
    }
  }
}

void game_intro() {


  if (!game_debug) {
    audio_coin_noentiendo();
    // NOENTIENDO LOGO
    game_cls();
    game_attribs();

    v0 = 0;
    v2 = 80;
//    v1 = 0;
    while (v0 < 16) {
      NIRVANAP_drawT( v2, 96, v0 + 8);
      v0 = v0 + 2;
      ++v2;
    }
    zx_print_ink(INK_WHITE);
    game_paint_attrib(&attrib, 0, 31, (15 * 8) + 8);
    if (spec128) {
      //NIRVANAP_drawT( 89, 120, 10);
      gbyte = &btiles[89*48];



      zx_print_str(15,12,"1");
      NIRVANAP_printQ(gbyte, gbyte + 32, 128,13);

      zx_print_str(15,18,"2");
      NIRVANAP_printQ(gbyte+1, gbyte + 40, 128,19);

      v0 = 1;
      while (v0) {
        v1 = in_inkey();
        v1 = v1 - 48;

        if (v1 == 1 || v1 == 2) {
          v0 = 0;
          game_lang = 0xFF + v1;
        }
      }

      ay_song_play(AY_SONG_LOOP, 6, ay_song_mistery);
      // ADDR PAGE ROW COL
      // *f_addr, f_page, unsigned char f_lin, unsigned char f_col, unsigned char f_width, unsigned char f_height)
      game_cls();
      game_img(&btiles[0]    , 0, 32, 1, 8, 5);
      game_text(0, 17);
      game_text(1, 18);
      game_img(&btiles[40*48], 0, 48, 7, 8, 5);
      game_text(2, 21);
      game_text(3, 22);
      z80_delay_ms(100);
      game_cls();
      game_img(&cartoon0[0]    , 1, 16, 1, 4, 6);
      game_text(4, 18);
      game_text(5, 19);
      game_img(&cartoon1[0]    , 1, 40, 7, 9, 6);
      game_text(6, 21);
      game_text(7, 22);
      game_img(&cartoon2[0]    , 1, 20, 5, 6, 7);
      game_text(8, 23);
      ay_reset();
      z80_delay_ms(100);
    } else {
      zx_print_str(15,5,"THIS GAME NEEDS 128KB");

      while(1) {
        //Eternal loop
      }
    }


  }
}

void game_intro_willy() {
  ay_song_play(AY_SONG_LOOP, 6, ay_song_mistery);
  game_cls();

  game_img(&cartoon6[0]    , 1, 32, 1, 8, 5);
  game_text(14, 19);
  game_text(15, 20);
  game_img(&cartoon7[0]    , 1, 80, 8, 7, 4);
  game_text(16, 21);
  game_text(17, 22);
  game_pause0();
  game_cls();
}

void game_end_willy() {
  ay_song_play(AY_SONG_LOOP, 6, ay_song_medievo);
  game_cls();


  game_img(&cartoon9[0]    , 1, 32, 0, 7, 4);

  game_text(18, 19);
  game_text(19, 20);

  game_img(&cartoon8[0]    , 1, 48, 6, 8, 5);
  game_text(20, 21);
  game_text(21, 22);
  game_text(22, 23);   //CHIMUELO
  game_pause0();
  game_cls();
}

void game_end_pietro() {
  ay_song_play(AY_SONG_LOOP, 6, ay_song_medievo);
  game_cls();

  game_img(&cartoon3[0]    , 1, 16, 2, 3, 6);
  game_text( 9, 18);
  game_text(10, 19);
  game_img(&cartoon4[0]    , 1, 32, 10, 4, 6);
  game_text(11, 20);
  game_img(&cartoon5[0]    , 1, 48, 6, 4, 6);
  game_text(12, 21);
  game_text(13, 23); //MATTHEWS
  game_pause0();
  game_cls();
}

void game_pause0() {
  v0 = 0;
  while (v0 == 0) {
    v0 = in_test_key();
  }
}

void game_aux_key() {

  // CAPS y SYMBOL READ

  v0 = (joyfunc2)(&k2);
  if (v0 & IN_STICK_LEFT) { // CAPS
    ++game_tune;
    // 0 DEF
    // 1 Pietro
    // 2 Willy
    // 3 MENU
    // 4 Off
    if (game_tune > 4) {
      game_tune = 1;
    }
    zx_print_ink((map_border << 3) | INK_WHITE);
    zx_print_str(0, 13, "TUNE");
    switch (game_tune) {
    case 1:
      zx_print_str(0, 18, "A");
      break;
    case 2:
      zx_print_str(0, 18, "B");
      break;
    case 3:
      zx_print_str(0, 18, "C");
      break;
    case 4:
      zx_print_str(0, 18, "OFF");
      break;
    }
    in_wait_nokey();
    audio_ingame();
    z80_delay_ms(25);
    game_fill_row(0, 32);
  };

  if (v0 & IN_STICK_RIGHT) { // SYMBOL
    ay_song_stop();
    z80_delay_ms(100);
    ay_fx_play(4, ay_fx_pause);
    v0 = 1;
    while (v0) {
      v1 = (joyfunc2)(&k2);
      z80_delay_ms(10);
      if (v1 & IN_STICK_RIGHT) {
        v0 = 0;
      }

      menu_rotcolor();
      zx_print_ink((map_border << 3) | color);
      zx_print_str(0, 14, "PAUSE");
    }
    game_fill_row(0, 32);
    audio_ingame();
    z80_delay_ms(100);
  };
}
