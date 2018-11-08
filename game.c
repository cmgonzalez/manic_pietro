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


        Manic Pietro - Cristian Gonzalez - cmgonzalez@gmail.com
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
#include <input.h>
#include <stdlib.h>

void main(void) {
  unsigned int counter;

  zx_border(INK_BLACK);
  zx_print_ink(INK_WHITE);

  // DEBUG
  game_debug = 0;

  game_gravity = 100; // GAME_GRAVITY;
  game_inmune = 0;    // GAME_INMUNE;
  game_inf_lives = 0; // GAME_INF_LIVES;
  game_fps_show = 1;
  scr_curr = 0;
  nirv_sprite_index = 0;
  game_song_play = 1;

  // Nirvana Attributes lookup tables
  attribs = (unsigned char * ) 0xFCC8;
  deltas = (unsigned char *) 0xFF01;

  if (!game_debug) {
    scr_curr = 0;
    game_fps_show = 0;
    z80_delay_ms(666); // SATANIC DELAY
  }
  // INTERRUPTS ARE DISABLED
  // RESET AY CHIP
  ay_reset();

  // Init Game
  game_start_timer();

  // ENABLE SOUND BASED ON DETECTED MODEL
  game_sound = spec128 ? (GAME_SOUND_AY_FX_ON | GAME_SOUND_AY_MUS_ON)
                       : (GAME_SOUND_48_FX_ON | GAME_SOUND_48_MUS_ON);

  // Keyboard Handling
  k1.fire = IN_KEY_SCANCODE_m;
  // TODO k1.fire1 = IN_KEY_SCANCODE_SPACE;
  k1.left = IN_KEY_SCANCODE_o;
  k1.right = IN_KEY_SCANCODE_p;
  // must be defined otherwise up is always true
  k1.up = IN_KEY_SCANCODE_DISABLE;
  k1.down = IN_KEY_SCANCODE_DISABLE;

  // Wait for Keypress and Randomize
  // Default Values for menu
  joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_sinclair1);
  // joyfunc2 = (uint16_t(*)(udk_t *))(in_stick_keyboard);

  // NIRVANAP_stop();
  if (!game_debug) {
    in_wait_nokey();
    for (counter = 31416; !in_test_key(); counter += 10061)
      ;
  }
  srand(counter);


  // Clear Screen and init Nirvana
  NIRVANAP_tiles(_btiles);
  NIRVANAP_start();
  game_attribs();

  game_over = 1;

  // Init Screen
  frame_time = zx_clock();
  menu_curr_sel = 1;
  // GAME INTRO
  game_intro();
  zx_border(INK_WHITE);

  while (1) {
    game_cls();
    // MENU
    if (!game_debug) {
      menu_main();
    }
    // GAME
    spr_clear_scr();
    player_next_extra = 10000;
    game_loop();
    spr_clear_scr();
    game_cls();
    game_paint_attrib(&attrib, 0, 31, (11 << 3) + 8);
    game_shoe();

    game_over = 0; // Hack game_colour_message to render background
    // GAME OVER
    scr_curr = 0; // Hack for debug...
  }
}

void test_proc() {}

unsigned char test_func() { return 0; }
