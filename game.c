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
  game_world = 0;
  scr_curr = 0xFF;
  game_song_play = 1;

  // INTERRUPTS ARE DISABLED
  // RESET AY CHIP
  ay_reset();
  // ENABLE SOUND BASED ON DETECTED MODEL
  game_sound = spec128 ? (GAME_SOUND_AY_FX_ON | GAME_SOUND_AY_MUS_ON)
                       : (GAME_SOUND_48_FX_ON | GAME_SOUND_48_MUS_ON);
  game_gravity = GAME_GRAVITY;
  player_vel_y0 = GAME_VELOCITY; // Velocity BUG 5 WILL TRIGGER SPRITE CLEAN BUG!!! NASTY

  // Keyboard Handling
  k1.fire = IN_KEY_SCANCODE_m;
  // TODO k1.fire1 = IN_KEY_SCANCODE_SPACE;
  k1.left = IN_KEY_SCANCODE_o;
  k1.right = IN_KEY_SCANCODE_p;
  k1.up = IN_KEY_SCANCODE_q;   // must be defined otherwise up is always true
  k1.down = IN_KEY_SCANCODE_a; // must be defined otherwise down is always true

  k2.left = IN_KEY_SCANCODE_DISABLE;
  k2.right = IN_KEY_SCANCODE_DISABLE;
  k2.up = IN_KEY_SCANCODE_DISABLE;
  k2.down = IN_KEY_SCANCODE_DISABLE;
  k2.fire = IN_KEY_SCANCODE_SPACE;

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
  zx_border(INK_BLACK);
  // Clear Screen and init Nirvana
  game_cls();
  // Init Game
  game_start_timer();
  // Load Tiles
  NIRVANAP_tiles(_btiles);
  game_attribs();
  game_over = 1;
  zx_print_ink(INK_WHITE);
  /*
  if (!game_debug) {
    zx_print_str(12, 6, "FOR FELIPE AND EDDIE");
    game_colour_message(12, 6, 6 + 20, 30, 0);
  }
  */
  // Init Screen
  frame_time = zx_clock();

  menu_curr_sel = 1;
  map_paper_last = PAPER_BLUE;
  game_control_mode = 2; // 0)2button - default 1) 1button: 1 Button UP+FIRE
                         // Shoots 2) 1 button: fire shoots, up jump , up+fire
                         // for ladders


  /*MAIN LOOP*/
  while (1) {


    /*Player Init*/

    player_lives = GAME_START_LIVES;
    player_vita = player_max_vita;

    player_score = 0;
    player_lin_scr = GAME_LIN_FLOOR - 24;
    player_col_scr = 2;
    game_round_up = 0;
    game_boss_alive = 1;
    game_over = 0;
    dirs = 0x00;
    map_paper = PAPER_BLACK;
    game_attribs();
    spr_btile_paint_back();
    // MENU
    if (!game_debug)
      menu_main();
    // GAME
    game_loop();
    // GAME OVER
    spr_flatten();
    game_update_stats();
    zx_print_str(12, 12, "GAME OVER");
    game_over = 0; // Hack game_colour_message to render background
    game_colour_message(12, 12, 12 + 9, 250, 0);

    spr_clear_scr();
    game_world = 0;
    scr_curr = 0xFF;

    game_cls();
  }
}

void test_proc() {

}

unsigned char test_func() { return 0; }
