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
#include <input.h>
#include <stdlib.h>

void main(void) {
  unsigned int counter;

  // DEBUG
  zx_border(INK_BLACK);
  z80_delay_ms(666);
  game_inmune = 0;    // GAME_INMUNE;
  game_inf_lives = 0; // GAME_INF_LIVES;
  game_debug = 1;
  game_fps_show = 1;
  game_world = 0;
  scr_curr = 0;
  nirv_sprite_index = 0;

  game_song_play = 1;
  game_tileset = scr_curr * 8;

  // INTERRUPTS ARE DISABLED
  // RESET AY CHIP
  ay_reset();
  // ENABLE SOUND BASED ON DETECTED MODEL
  game_sound = spec128 ? (GAME_SOUND_AY_FX_ON | GAME_SOUND_AY_MUS_ON)
                       : (GAME_SOUND_48_FX_ON | GAME_SOUND_48_MUS_ON);
  game_gravity = 98;    // GAME_GRAVITY;
  player_vel_y0 = -690; // GAME_VELOCITY;

  // Keyboard Handling
  k1.fire = IN_KEY_SCANCODE_m;
  // TODO k1.fire1 = IN_KEY_SCANCODE_SPACE;
  k1.left = IN_KEY_SCANCODE_o;
  k1.right = IN_KEY_SCANCODE_p;
  k1.up = IN_KEY_SCANCODE_q;   // must be defined otherwise up is always true
  k1.down = IN_KEY_SCANCODE_a; // must be defined otherwise down is always true

  // Wait for Keypress and Randomize //
  /* Default Values for menu */
  joyfunc1 = (uint16_t(*)(udk_t *))(in_stick_sinclair1);
  joyfunc2 = (uint16_t(*)(udk_t *))(in_stick_keyboard);

  if (!game_debug) {
    in_wait_nokey();
    for (counter = 31416; !in_test_key(); counter += 10061)
      ;
    srand(counter);
  }
  // Clear Screen and init Nirvana
  game_cls();
  // Init Game
  game_start_timer();
  // Load Tiles
  NIRVANAP_tiles(_btiles);
  game_attribs();
  game_over = 1;
  zx_print_ink(INK_WHITE);

  // Init Screen
  frame_time = zx_clock();

  menu_curr_sel = 1;
  map_paper_last = PAPER_BLACK; // DEFAULT PAPER ON BTILE FILE

  while (1) {

    // MENU
    if (!game_debug) {
      menu_main();
    }

    // GAME
    game_loop();
    // GAME OVER
    // spr_flatten();
    // game_update_stats();
    zx_print_str(11, 12, "DEMO OVER");
    z80_delay_ms(250);
    game_over = 0; // Hack game_colour_message to render background
    // game_colour_message(12, 12, 12 + 9, 250, 0);

    // spr_clear_scr();

    for (i = 0; i < NIRV_TOTAL_SPRITES; ++i) {
      NIRVANAP_spriteT(i, 0, 0, 0);
    }
    // game_cls();
  }
}

void test_proc() {


}

unsigned char test_func() { return 0; }

void game_copy_tile() {
  // copy a btile from upper memory to first 48
  unsigned char btile[48];
  unsigned char i;

  GLOBAL_ZX_PORT_7FFD = 0x10 + 6;
  IO_7FFD = 0x10 + 6;
  i = 0;
  while (i < 48) {
    btile[i] = btile[i];
    ++i;
  }
  // Page in BANK 00
  GLOBAL_ZX_PORT_7FFD = 0x10 + 0;
  IO_7FFD = 0x10 + 0;
}
