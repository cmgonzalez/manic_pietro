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
#include "game_player.h"
#include "game_sprite.h"
#include "game_zx.h"
#include "macros.h"
#include <arch/zx.h>
#include <arch/zx/nirvana+.h>
#include <input.h>
#include <stdlib.h>

void player_init(unsigned char f_lin, unsigned char f_col,
                 unsigned char f_tile) {
  // COMMON SPRITE VARIABLES
  class[GAME_INDEX_P1] = 0xFF;
  spr_speed[GAME_INDEX_P1] = PLAYER_SPEED;
  spr_frames[GAME_INDEX_P1] = 4;

  lin[GAME_INDEX_P1] = f_lin; //*SPRITELIN(GAME_INDEX_P1);
  col[GAME_INDEX_P1] = f_col; //*SPRITECOL(GAME_INDEX_P1);

  state[GAME_INDEX_P1] = 0;
  state_a[GAME_INDEX_P1] = 0;
  last_time[GAME_INDEX_P1] = zx_clock();
  player_vel_y = 0;
  colint[GAME_INDEX_P1] = 0;
  spr_frames[GAME_INDEX_P1] = 4;
  p_state = &state[GAME_INDEX_P1];
  p_state_a = &state_a[GAME_INDEX_P1];

  // PLAYER ONLY VARIABLES
  if (f_col < 16) {
    BIT_SET(*p_state, STAT_DIRR);
    BIT_SET(*p_state_a, STAT_LDIRR);
    tile[GAME_INDEX_P1] = f_tile;
  } else {
    colint[GAME_INDEX_P1] = 3;
    BIT_SET(*p_state, STAT_DIRL);
    BIT_SET(*p_state_a, STAT_LDIRL);
    tile[GAME_INDEX_P1] = f_tile + spr_frames[GAME_INDEX_P1];
  }
  NIRVANAP_spriteT(NIRV_SPRITE_P1, tile[GAME_INDEX_P1], lin[GAME_INDEX_P1] + 16,
                   col[GAME_INDEX_P1]);
}

void player_turn(void) {
  unsigned char c;
  sprite = GAME_INDEX_P1;
  if (spr_chktime(&sprite)) {

    c = in_inkey();

    if (c == 49) { // 1
      --scr_curr;
      game_round_up = 1;
      if (scr_curr == 255) {
        scr_curr = 19;
      }
    }
    if (c == 50) { // 2
      ++scr_curr;
      game_round_up = 1;
      if (scr_curr == 20) {
        scr_curr = 0;
      }
    }
    // zx_border(INK_RED); // TODO REMOVE ME ONLY FOR COLISION DETECTION
    dirs = (joyfunc1)(&k1);
    /* Player initial Values */
    p_state = &state[GAME_INDEX_P1];
    p_state_a = &state_a[GAME_INDEX_P1];

    // TODO PERF POINTERS lin y col ARRAYS
    s_lin0 = lin[GAME_INDEX_P1];
    s_col0 = col[GAME_INDEX_P1];
    s_tile0 = tile[GAME_INDEX_P1];
    s_class = 0;
    player_move();
    player_collision();
    if (player_killed && game_debug) {
      player_killed = 0;
      zx_border(INK_BLACK);
    }
    if (player_killed) {
      player_killed = 0;
      player_lost_life();
    }
    // Level UP
    if (obj_count == 0xFF && col[GAME_INDEX_P1] == game_exit_col &&
        lin[GAME_INDEX_P1] == game_exit_lin - 16) {
      ++scr_curr;
      game_round_up = 1;
    }
  }
}

unsigned char player_move(void) {

  if (BIT_CHK(*p_state, STAT_JUMP) || BIT_CHK(*p_state, STAT_FALL)) {
    // Jump Handling
    player_move_jump();

  } else {
    // Walk Handling
    player_move_walk();
  }
  /* Paint Player Sprite */
  spr_paint_player();
  return 0;
}

unsigned char player_move_jump(void) {

  signed int val_yc;
  ++player_jump_count;
  player_vel_y = player_vel_y + game_gravity;

  // JUMP HACK! TO BE LIKE REAL MMINER WE JUST INSERT HORIZONTAL EXTRA MOVEMENT
  // jump_count 0=>16
  if ((player_jump_count == 2 || player_jump_count == 4 ||
       player_jump_count == 10 || player_jump_count == 12 ||
       player_jump_count == 14) &&
      player_jump_hack) {
    player_jump_hack = 0;
    // Force anim on next loop

    last_time[GAME_INDEX_P1] = 0;
    player_vel_y = player_vel_y - game_gravity;
    spr_move_horizontal();
    return 0;
  }
  player_jump_hack = 1;

  // CONVER TO PIXEL'S
  val_yc = player_vel_y / 100;

  s_lin1 = (unsigned char)val_yc;

  // Nirvana don't support odds lines
  if ((s_lin1 & 1) != 0) {
    s_lin1--;
  }

  s_lin1 = lin[GAME_INDEX_P1] + s_lin1;

  if (val_yc <= 0) {

    // Asending
    spr_set_up();
    if (s_lin1 > GAME_LIN_FLOOR) {
      s_lin1 = 0;
    }
    if (!player_check_ceil(s_lin1, col[GAME_INDEX_P1])) {
      player_vel_y = 0;
      BIT_CLR(*p_state, STAT_DIRR);
      BIT_CLR(*p_state, STAT_DIRL);
    } else {
      lin[GAME_INDEX_P1] = s_lin1;
    }

    player_jump_top = s_lin1;

  } else {

    // Falling
    v0 = player_get_floor();

    if ((s_lin1 >= 120) || // Out of screen
        s_lin1 >= v0 &&    // Curr floor
            v0 >= s_lin0   // Is below
    ) {

      if ((s_lin1 - player_jump_top) > 48) {
        // JUMP DEAD
        player_killed = 1;
      }
      // Jump end
      lin[GAME_INDEX_P1] = v0;
      player_vel_y = 0;
      BIT_CLR(*p_state, STAT_FALL);
      BIT_CLR(*p_state, STAT_JUMP);
      return 0;
    }
    spr_set_down();
    lin[GAME_INDEX_P1] = s_lin1;
  }
  spr_move_horizontal();
  return 1;
}

unsigned char player_move_walk(void) {
  // TODO CLEAN THIS!
  /* User have pressed valid input */

  if (player_check_input()) {

    if (dirs & IN_STICK_FIRE) {
      // Pre CHECK
      if (player_check_ceil(lin[GAME_INDEX_P1] - 2, col[GAME_INDEX_P1])) {
        // NEW JUMP
        player_vel_inc = 1;
        player_jump_count = 0xFF;
        audio_salto();
        spr_set_up();
        player_vel_y = player_vel_y0;

        BIT_CLR(*p_state, STAT_DIRL);
        BIT_CLR(*p_state, STAT_DIRR);

        if (BIT_CHK(*p_state, STAT_CONVEYOR) && !BIT_CHK(*p_state, STAT_DIRL) &&
            !BIT_CHK(*p_state, STAT_DIRR)) {

          if (game_conveyor_dir == DIR_LEFT) {
            spr_set_left();
          }
          if (game_conveyor_dir == DIR_RIGHT) {
            spr_set_right();
          }

        } else {

          if (dirs & IN_STICK_LEFT) {
            spr_set_left();
          }

          if (dirs & IN_STICK_RIGHT) {
            spr_set_right();
          }
        }

        player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
      }

      return 1;
    } // END IN_STICK_FIRE

    if (BIT_CHK(*p_state, STAT_CONVEYOR)) {

      if (!BIT_CHK(*p_state, STAT_DIRL) && !BIT_CHK(*p_state, STAT_DIRR)) {

      } else {
        /* Move Right */
        if (game_conveyor_dir == DIR_RIGHT) {
          spr_set_right();
          spr_move_horizontal();
        }

        /* Move Left */
        if (game_conveyor_dir == DIR_LEFT) {
          spr_set_left();
          spr_move_horizontal();
        }
      }

    } else {

      // CONVEYOR DETECTION
      BIT_CLR(*p_state, STAT_CONVEYOR);
      index1 = spr_calc_index(lin[GAME_INDEX_P1] + 16, col[GAME_INDEX_P1]);
      if (scr_map[index1] == TILE_CONVEYOR ||
          scr_map[index1 + 1] == TILE_CONVEYOR) {
        zx_border(INK_CYAN);
        BIT_SET(*p_state, STAT_CONVEYOR);
      }

      /* Move Right */
      if (dirs & IN_STICK_RIGHT) {
        spr_set_right();
        spr_move_horizontal();
      }

      /* Move Left */
      if (dirs & IN_STICK_LEFT) {
        spr_set_left();
        spr_move_horizontal();
      }
    }

    /* Set Tile according to current direction */
    player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
    /* Check if the player have floor, and set fall if not */
    if (player_check_floor(0) && player_check_floor(1)) {
      // spr_move_horizontal();
      BIT_SET(*p_state, STAT_FALL);
      BIT_CLR(*p_state, STAT_DIRL);
      BIT_CLR(*p_state, STAT_DIRR);
    }
    return 1;
  } else {
    // TODO CONVEYOR DETECTION
    BIT_CLR(*p_state, STAT_CONVEYOR);
    index1 = spr_calc_index(lin[GAME_INDEX_P1] + 16, col[GAME_INDEX_P1]);
    if (scr_map[index1] == TILE_CONVEYOR ||
        scr_map[index1 + 1] == TILE_CONVEYOR) {
      BIT_SET(*p_state, STAT_CONVEYOR);
      BIT_CLR(*p_state, STAT_DIRL);
      BIT_CLR(*p_state, STAT_DIRR);
    }

    /* Check if the player have floor, and set fall if not */
    if (player_check_floor(0) && player_check_floor(1)) {
      BIT_SET(*p_state, STAT_FALL);
      BIT_CLR(*p_state, STAT_DIRL);
      BIT_CLR(*p_state, STAT_DIRR);
    }

    if (BIT_CHK(*p_state, STAT_CONVEYOR)) {

      if (game_conveyor_dir == DIR_LEFT) {
        BIT_SET(*p_state, STAT_DIRL);
        player_tile(TILE_P1_RIGHT, TILE_P1_LEN);

      } else {
        BIT_SET(*p_state, STAT_DIRR);
        player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
      }

      spr_move_horizontal();
    }
  }

  return 0;
}
void player_collision() {
  // Left

  BIT_CLR(*p_state, STAT_ONEXIT);

  index1 = spr_calc_index(lin[GAME_INDEX_P1], col[GAME_INDEX_P1]);
  v0 = scr_map[index1];
  v0 = player_pick_deadly(v0);
  v0 = player_pick_exit(v0);

  v0 = player_pick_item(v0, index1);

  index1 = spr_calc_index(lin[GAME_INDEX_P1] + 14, col[GAME_INDEX_P1]);
  v0 = scr_map[index1];
  v0 = player_pick_deadly(v0);
  v0 = player_pick_exit(v0);
  v0 = player_pick_item(v0, index1);

  // Right

  index1 = spr_calc_index(lin[GAME_INDEX_P1], col[GAME_INDEX_P1] + 1);
  v0 = scr_map[index1];
  v0 = player_pick_deadly(v0);
  v0 = player_pick_exit(v0);
  v0 = player_pick_item(v0, index1);

  index1 = spr_calc_index(lin[GAME_INDEX_P1] + 14, col[GAME_INDEX_P1] + 1);
  v0 = scr_map[index1];
  v0 = player_pick_deadly(v0);
  v0 = player_pick_exit(v0);
  v0 = player_pick_item(v0, index1);

  // Sprite Collision
  for (i = 0; i < GAME_INDEX_P1; ++i) {
    if (class[i] > 0) {
      v0 = abs(col[i] - col[GAME_INDEX_P1]);
      if (v0 < 2) {
        v0 = abs(lin[i] - lin[GAME_INDEX_P1]);
        if (v0 < 16) {
          v0 = abs(colint[i] - colint[GAME_INDEX_P1]);
          if (v0 < 2) {
            player_kill_index = 0xFFFF;
            player_killed = 1;
          }
        }
      }
    }
  }
}

unsigned char player_get_floor() {
  index1 = spr_calc_index(lin[GAME_INDEX_P1] + 16, col[GAME_INDEX_P1]);

  i = 0;
  while (i < 16) {

    v0 = scr_map[index1];
    v1 = scr_map[index1 + 1];

    if ((v0 == TILE_EMPTY || v0 == TILE_OBJECT || v0 == TILE_DEADLY1 ||
         v0 == TILE_DEADLY2 || v0 >= TILE_EXIT0) &&
        (v1 == TILE_EMPTY || v1 == TILE_OBJECT || v1 == TILE_DEADLY1 ||
         v1 == TILE_DEADLY2 || v1 >= TILE_EXIT0)) {
      index1 = index1 + 32;
      ++i;
    } else {
      return ((index1 >> 5) << 3) - 16; // /32 *8 -16 Optimizar
    }
  }
  return GAME_LIN_FLOOR - 24;
}

unsigned char player_check_input(void) {

  if ((dirs & IN_STICK_LEFT) && (dirs & IN_STICK_RIGHT)) {
    // Error on reading both horizontal's
    dirs = 0;
  }

  if (dirs & IN_STICK_LEFT || dirs & IN_STICK_RIGHT || dirs & IN_STICK_FIRE) {
    return 1;
  }

  return 0;
}

void player_1up() {
  if (player_lives < 100) {
    ++player_lives;
    spr_flatten();
    zx_print_str(12, 12, "1 UP!");
    game_colour_message(12, 12, 12 + 5, 25, 0);
    game_update_stats();
    spr_unflattenP1();
  }
}

void player_tile(unsigned char f_tile, unsigned char f_inc) {
  tile[GAME_INDEX_P1] = spr_get_tile_dir(&f_tile, &f_inc);
}

void player_score_add(unsigned int f_score) __z88dk_fastcall {
  player_score = player_score + f_score;

  // CHECK FOR TOP SCORE
  if (player_score > game_score_top) {
    game_score_top = player_score;
  }
  game_print_score();
}

unsigned char player_pick_item(unsigned char l_val, int l_index) {

  if (l_val == TILE_OBJECT) {
    zx_border(INK_YELLOW);
    scr_map[l_index] = TILE_EMPTY;
    ++player_coins;
    audio_coin();
    zx_border(INK_WHITE);
    return TILE_EMPTY;
  }
  return l_val;
}

unsigned char player_pick_deadly(unsigned char l_val) {

  if (l_val == TILE_DEADLY1 || l_val == TILE_DEADLY2) {
    player_killed = 1;
    player_kill_index = index1;
    v0 = TILE_EMPTY;
    v1 = TILE_EMPTY;
    return 0;
  }
  return l_val;
}

unsigned char player_pick_exit(unsigned char l_val) {

  if (l_val >= TILE_EXIT0) {
    BIT_SET(*p_state, STAT_ONEXIT);
    return 0;
  }
  return l_val;
}

unsigned char player_check_ceil(unsigned char f_lin, unsigned char f_col) {
  // TODO OPTIMIZA LIKE CHECK FLOOR JUMP/WALK
  if (colint[GAME_INDEX_P1] < 3) {
    index1 = spr_calc_index(f_lin, f_col);
    v0 = scr_map[index1];
  } else {
    v0 = TILE_EMPTY;
  }

  if (colint[GAME_INDEX_P1] > 0) {
    index1 = spr_calc_index(f_lin, f_col + 1);
    v1 = scr_map[index1];
  } else {
    v1 = TILE_EMPTY;
  }

  if (v0 != TILE_WALL && v1 != TILE_WALL) {
    return 1;
  }

  return 0;
}

unsigned char player_check_floor(unsigned char f_inc) {

  index1 = spr_calc_index(lin[GAME_INDEX_P1] + 16, col[GAME_INDEX_P1] + f_inc);


  //HACK CRUMB
    if (f_inc == 0 && colint[GAME_INDEX_P1] == 3) {
      v0 = TILE_EMPTY;
    } else {
      v0 = scr_map[index1];
    }
    if (f_inc == 1 && colint[GAME_INDEX_P1] == 0) {
      v0 = TILE_EMPTY;
    } else {
      v0 = scr_map[index1];
    }
  //END HACK
  v0 = scr_map[index1];
  if (v0 == TILE_EMPTY || v0 == TILE_OBJECT || v0 == TILE_DEADLY1 ||
      v0 == TILE_DEADLY2) {
    return 1;
  }


  if (v0 == TILE_CRUMB0 ||
      (v0 >= TILE_CRUMB1 && v0 <= TILE_CRUMB3)) { // TODO CAUTION!
    s_col1 = col[GAME_INDEX_P1] + f_inc;

    if (v0 == TILE_CRUMB0) {
      scr_map[index1] = TILE_CRUMB1;
    } else {
      ++scr_map[index1];
      //++scr_map[index1];
      if (scr_map[index1] > TILE_CRUMB3) {
        scr_map[index1] = TILE_EMPTY;
      }
    }

    // GASTA BRICK TODO OPTIMIZE CALC
    s_row1 = (((lin[GAME_INDEX_P1] + 16) >> 3) + 1) << 3;
    // game_cell_paint();
    spr_draw8(scr_map[index1], s_row1, s_col1);
  }

  return 0;
}

void player_lost_life() {
  unsigned char i;

  ay_reset();
  audio_explosion();
  game_update_stats();

  // BLINK
  while (i < 32) {

    if (player_kill_index != 0xFFFF) {
      index1 = player_kill_index;
      NIRVANAP_spriteT(NIRV_SPRITE_P1,
                       tile[GAME_INDEX_P1] + colint[GAME_INDEX_P1],
                       lin[GAME_INDEX_P1] + 16, col[GAME_INDEX_P1]);
      NIRVANAP_halt();
      NIRVANAP_spriteT(NIRV_SPRITE_P1, 0, 0, 0);
      NIRVANAP_halt();
      game_cell_paint_index(); // TODO REMOVE
      NIRVANAP_halt();
    } else {
      NIRVANAP_spriteT(NIRV_SPRITE_P1,
                       tile[GAME_INDEX_P1] + colint[GAME_INDEX_P1],
                       lin[GAME_INDEX_P1] + 16, col[GAME_INDEX_P1]);
      NIRVANAP_halt();
      NIRVANAP_spriteT(NIRV_SPRITE_P1, 0, 0, 0);
      NIRVANAP_halt();
    }
    ++i;
  }
  game_playing = 0;
  // z80_delay_ms(600);
  s_lin0 = lin[GAME_INDEX_P1];
  s_col0 = col[GAME_INDEX_P1];
  spr_init_effects();

  // Player lost life
  if (!game_inf_lives) {
    --player_lives;
  }
  // game_update_stats();

  if (player_lives > 0) {
    // z80_delay_ms(500);
    game_round_init();
    game_playing = 1;
  } else {
    // Game End
    player_lives = 0;
    game_over = 1;
  }
}
