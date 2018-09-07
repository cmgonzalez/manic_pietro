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
  scr_curr = 1;// 0xFF;

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

  // Init Screen
  frame_time = zx_clock();

  menu_curr_sel = 1;
  map_paper_last = PAPER_BLACK; // DEFAULT PAPER ON BTILE FILE
  game_control_mode = 2; // 0)2button - default 1) 1button: 1 Button UP+FIRE
                         // Shoots 2) 1 button: fire shoots, up jump , up+fire
                         // for ladders

  /*MAIN LOOP*/
  game_attribs();

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

    for (i = 0; i <= SPR_P1; ++i) {
      NIRVANAP_spriteT(i, 0, 0, 0);
    }
    // game_cls();
  }
}

void test_proc() {}

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

void game_sprite_draw8(unsigned char f_spr8, unsigned char f_lin,
                       unsigned char f_col) {

  /*
    A quick note about the "btile" format:

    Each "btile" represents a 16x16 image, using 8x2 attributes. It's very
    similar to "ctiles" used in BIFROST* and ZXodus, except each tile is 48
    bytes (instead of 64 bytes) and their attributes are stored vertically:
    Byte 1: bitmap value for 1st pixel line, 1st column
    Byte 2: bitmap value for 1st pixel line, 2nd column
    Byte 3: bitmap value for 2nd pixel line, 1st column
    Byte 4: bitmap value for 2nd pixel line, 2nd column
    Byte 5: bitmap value for 3rd pixel line, 1st column
    Byte 6: bitmap value for 3rd pixel line, 2nd column
    ...
    Byte 31: bitmap value at 16th pixel line, 1st column
    Byte 32: bitmap value at 16th pixel line, 2nd column
    Byte 33: attribute value for 1st and 2nd pixel line, 1st column
    Byte 34: attribute value for 3rd and 4th pixel line, 1st column
    Byte 35: attribute value for 5th and 6th pixel line, 1st column
    ...
    Byte 40: attribute value for 15th and 16th pixel line, 1st column
    Byte 41: attribute value for 1st and 2nd pixel line, 2nd column
    Byte 42: attribute value for 3rd and 4th pixel line, 2nd column
    ...
    Byte 48: attribute value for 15th and 16th pixel line, 2nd column

    The ordering above looks somewhat unconventional, but there's a reason: it's
    more convenient for the NIRVANA ENGINE this way, so it can be more compact
    and efficient.
  */
  // Geometria ancho del btile (teorico) por ejemplo 8 btiles (16x16)
  unsigned char *f_byte;
  unsigned char *f_byte_src;
  unsigned char *f_byte_src0;
  unsigned char f_spr16;
  unsigned char f_lin1;
  unsigned char *f_attrib_start;

  if (f_spr8 < 16) {
    f_spr16 =
        f_spr8 >>
        1; // si es mayor que 16 puedo sacar los graficos de la segunda linea
    f_spr8 = f_spr8 % 2;
  } else {
    f_spr8 = f_spr8 - 16;
    f_spr16 =
        f_spr8 >>
        1; // si es mayor que 16 puedo sacar los graficos de la segunda linea
    f_spr8 = 2 + (f_spr8 % 2);
  }

  f_byte_src0 = &btiles[0] + (48 * f_spr16);

  switch (f_spr8) {
  case 0:
    f_byte_src = f_byte_src0;
    f_attrib_start = f_byte_src0 + 32;
    break;
  case 1:
    f_byte_src = f_byte_src0 + 1;
    f_attrib_start = f_byte_src0 + 32 + 8;
    break;
  case 2:
    f_byte_src = f_byte_src0 + 16;
    f_attrib_start = f_byte_src0 + 32 + 4;
    break;
  case 3:
    f_byte_src = f_byte_src0 + 17;
    f_attrib_start = f_byte_src0 + 32 + 12;
    break;
  }

  attrib[0] = *f_attrib_start;
  ++f_attrib_start;
  attrib[1] = *f_attrib_start;
  ++f_attrib_start;
  attrib[2] = *f_attrib_start;
  ++f_attrib_start;
  attrib[3] = *f_attrib_start;

  NIRVANAP_paintC(&attrib, f_lin + 8, f_col); // TODO direct Nirvana Buffer poke

  f_lin1 = f_lin + 8;
  // TODO can be optimized on div 8 rows, no need for each zx_py2addr
  while (f_lin < f_lin1) {
    f_byte = zx_py2saddr( f_lin ) + f_col;
    *f_byte = *f_byte_src;
    f_byte_src = f_byte_src + 2;
    ++f_lin;
  }
}
