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
#include <input.h>
#include <stdlib.h>

void player_init(unsigned char f_lin, unsigned char f_col,
                 unsigned char f_tile) {
  // COMMON SPRITE VARIABLES
  class[INDEX_P1] = 0xFF;
  spr_speed[INDEX_P1] = PLAYER_SPEED;
  spr_frames[INDEX_P1] = 4;

  lin[INDEX_P1] = f_lin; //*SPRITELIN(INDEX_P1);
  col[INDEX_P1] = f_col; //*SPRITECOL(INDEX_P1);
  player_jump_top = f_lin;
  state[INDEX_P1] = 0;
  state_a[INDEX_P1] = 0;
  last_time[INDEX_P1] = zx_clock();
  player_vel_y = 0;

  spr_frames[INDEX_P1] = 4;

  // PLAYER ONLY VARIABLES

  if (value_a[INDEX_P1]) {
    colint[INDEX_P1] = 3;
    BIT_SET(state[INDEX_P1], STAT_DIRL);
    BIT_SET(state_a[INDEX_P1], STAT_LDIRL);
    tile[INDEX_P1] = f_tile + 4;
    colint[INDEX_P1] = value_b[INDEX_P1];
  } else {
    BIT_SET(state[INDEX_P1], STAT_DIRR);
    BIT_SET(state_a[INDEX_P1], STAT_LDIRR);
    tile[INDEX_P1] = f_tile;
    colint[INDEX_P1] = value_b[INDEX_P1];
  }
  // TODO PERF remove +16 and ajust all game
  // NIRVANAP_spriteT(NIRV_SPRITE_P1, tile[INDEX_P1], lin[INDEX_P1] +
  // 16,
  //                 col[INDEX_P1]);
}

void player_turn(void) {

  sprite = INDEX_P1;
  if (spr_chktime()) {
    s_lin0 = lin[INDEX_P1];
    s_col0 = col[INDEX_P1];

    dirs = (joyfunc1)(&k1);

    player_handle_lock();
    player_move();
    player_collision();

    if (BIT_CHK(state[INDEX_P1], STAT_KILLED) && !game_inmune) {
      player_lost_life();
      BIT_CLR(state[INDEX_P1], STAT_KILLED);
    }
    player_check_exit();
    player_debug_keys();
  }
}

void player_debug_keys() {
  unsigned char c;

  c = in_inkey();
  if (c == 32) { // SPACE
    if (game_song_play_start) {
      game_song_play_start = 0;
      ay_song_stop();
    } else {
      audio_ingame();
    }
    in_wait_nokey();
  }
  if (c == 49) { // 1
    --scr_curr;
    game_round_up = 1;
    if (scr_curr == 255) {
      scr_curr = 39;
    }
    in_wait_nokey();
  }
  if (c == 50) { // 2
    ++scr_curr;
    game_round_up = 1;
    if (scr_curr == 40) {
      scr_curr = 0;
    }
    in_wait_nokey();
  }

  if (c == 51) {
    game_inmune = !game_inmune;
    if (game_inmune) {
      zx_print_str(21, 0, "MILLENIAL MODE");
    } else {
      zx_print_str(21, 0, "GENX MODE");
    }

    z80_delay_ms(25);
    in_wait_nokey();
    game_fill_row(21, 32);
  }

  if (c == 52) {
    game_round_up = 1;
    z80_delay_ms(250);
    in_wait_nokey();
  }

  if (c == 53) {
    if (scr_curr >= 20) {
      scr_curr = 0;
    } else {
      scr_curr = 20;
    }
    game_round_up = 1;
    z80_delay_ms(250);
    in_wait_nokey();
  }
}

void player_check_exit() {
  // Level UP
  if (obj_count == 0xFF && (col[INDEX_P1] == game_exit_col)) {
    v0 = abs((lin[INDEX_P1] + 16) - game_exit_lin);

    if (v0 < 8) {
      ++scr_curr;
      game_round_up = 1;
      // Amimate Bonus Air
      ay_song_stop();
      while ((unsigned int)air_curr_byte > (unsigned int)air_end_byte) {
        audio_tick();
        game_anim_air();
        player_score_add(25);
        z80_delay_ms(5);
      }
    }
  }
}

unsigned char player_move(void) {
  if (BIT_CHK(state[INDEX_P1], STAT_JUMP) ||
      BIT_CHK(state[INDEX_P1], STAT_FALL)) {
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

  ++player_jump_count; // 0xFF -> 16 normal horizontal jump

  //CAE COMO ROCA
  if (player_jump_count > 17) {
    BIT_CLR(state[INDEX_P1], STAT_DIRR);
    BIT_CLR(state[INDEX_P1], STAT_DIRL);
  }
  player_vel_y = player_vel_y + game_gravity;

  if (BIT_CHK(state[INDEX_P1], STAT_CONVEYOR)) {
    // FIX Conveyor FALL
    BIT_CLR(state[INDEX_P1], STAT_DIRR);
    BIT_CLR(state[INDEX_P1], STAT_DIRL);
    BIT_CLR(state[INDEX_P1], STAT_CONVEYOR);
  }

  // END JUMP HACK
  // CONVER TO PIXEL'S
  val_yc = player_vel_y / 100;

  s_lin1 = (unsigned char)val_yc;

  // Nirvana don't support odds lines
  if ((s_lin1 & 1) != 0) {
    s_lin1--;
  }

  s_lin1 = lin[INDEX_P1] + s_lin1;

  if (val_yc <= 0) {
    // Asending
    spr_set_up();
    if (s_lin1 > GAME_LIN_FLOOR) {
      s_lin1 = 0;
    }
    if (!player_check_ceil(s_lin1, col[INDEX_P1])) {
      player_vel_y = 0;

      BIT_CLR(state[INDEX_P1], STAT_DIRR);
      BIT_CLR(state[INDEX_P1], STAT_DIRL);
    } else {
      lin[INDEX_P1] = s_lin1;
    }

    player_jump_top = s_lin1;
  } else {
    // Falling

    // audio_fall();

    v0 = player_get_floor();

    if ((s_lin1 >= 120) || // Out of screen
        s_lin1 >= v0 &&    // Curr floor
            v0 >= s_lin0   // Is below
    ) {

      if ((s_lin1 - player_jump_top) >= 40) {
        // JUMP DEAD
        BIT_SET(state[INDEX_P1], STAT_KILLED);
      }
      // Jump end
      lin[INDEX_P1] = v0;
      player_vel_y = 0;
      // if (!player_check_floor(0) || !player_check_floor(1)) {
      BIT_CLR(state[INDEX_P1], STAT_FALL);
      BIT_CLR(state[INDEX_P1], STAT_JUMP);
      player_check_conveyor();
      // BIT_CLR(state[INDEX_P1], STAT_CONVEYOR);
      ay_fx_stop();
      //}
      // FOR CRUMBLE FLOOR
      if ( (dirs & IN_STICK_FIRE) ) {
        player_check_floor(0);
        player_check_floor(1);
      }

      return 0;
    }
    spr_set_down();
    lin[INDEX_P1] = s_lin1;
  }

  // JUMP HACK! TO BE LIKE REAL MMINER WE JUST INSERT HORIZONTAL EXTRA
  // MOVEMENT jump_count 0=>16
  if (BIT_CHK(state[INDEX_P1], STAT_DIRL) ||
      BIT_CHK(state[INDEX_P1], STAT_DIRR))
     {
      // TODO Ugly should fix
      if (player_jump_hor[player_jump_count]) {
        // Force anim on next loop
        player_vel_y = player_vel_y - game_gravity;
        spr_move_horizontal();
        // last_time[INDEX_P1] = 0;
        // z80_delay_ms(2);
        return 0;
      }
      spr_move_horizontal();
    }
  return 1;
}

unsigned char player_move_walk(void) {
  // Handle of player walk
  if (player_check_input()) {

    if (dirs & IN_STICK_FIRE) {
      player_new_jump();
      spr_move_horizontal();
      return 1;
    }

    if (!BIT_CHK(state[INDEX_P1], STAT_CONVEYOR)) {

      player_handle_walk();
    }

    if (!BIT_CHK(state_a[INDEX_P1], STAT_LOCK)) {
      player_handle_conveyor();
    }

    player_handle_fall();
    spr_move_horizontal();
    return 1;
  } else {

    // NO INPUT FROM Player
    if (player_handle_conveyor()) {
      spr_move_horizontal();
    }
    player_handle_fall();
  }
  return 0;
}

void player_handle_lock() {
  // Key Lock
  if (!BIT_CHK(state[INDEX_P1], STAT_CONVEYOR) &&
      !BIT_CHK(state[INDEX_P1], STAT_JUMP) &&
      !BIT_CHK(state[INDEX_P1], STAT_FALL)) {

    if (dirs & IN_STICK_RIGHT) {
      if (dirs_last != DIR_RIGHT)
         {
          dirs_last = DIR_RIGHT;
          BIT_CLR(state_a[INDEX_P1], STAT_LOCK);
        }
      else {
        BIT_SET(state_a[INDEX_P1], STAT_LOCK);
      }
    }

    if (dirs & IN_STICK_LEFT) {
      if (dirs_last != DIR_LEFT) {
        dirs_last = DIR_LEFT;
        BIT_CLR(state_a[INDEX_P1], STAT_LOCK);
      } else {
        BIT_SET(state_a[INDEX_P1], STAT_LOCK);
      }
    }
  }
  if (!(dirs & IN_STICK_RIGHT) && !(dirs & IN_STICK_LEFT)) {
    BIT_CLR(state_a[INDEX_P1], STAT_LOCK);
  }
}

void player_handle_walk() {
  // if (BIT_CHK(state_a[INDEX_P1], STAT_LOCK) && !BIT_CHK(state_a[INDEX_P1],
  // STAT_CONVJMP)) {

  if (dirs & IN_STICK_RIGHT) {
    if (BIT_CHK(state_a[INDEX_P1], STAT_LDIRL)) {
      colint[INDEX_P1]--; // DarioPedia
    }
    spr_set_right();
  }

  /* Move Left */
  if (dirs & IN_STICK_LEFT) {
    if (BIT_CHK(state_a[INDEX_P1], STAT_LDIRR)) {
      colint[INDEX_P1]++; // DarioPedia
    }
    spr_set_left();
  }
  // Set Tile according to current direction
  player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
}

void player_handle_fall() {
  /* Check if the player have floor, and set fall if not */
  if (player_check_floor(0) && player_check_floor(1)) {
    player_jump_top = lin[INDEX_P1];
    BIT_SET(state[INDEX_P1], STAT_FALL);
    BIT_CLR(state[INDEX_P1], STAT_DIRL);
    BIT_CLR(state[INDEX_P1], STAT_DIRR);
    BIT_CLR(state_a[INDEX_P1], STAT_LOCK);
    player_jump_count = 0xFF;
    player_fall_start = 1;
    audio_fall();
  }
}

unsigned char player_handle_conveyor() {

  player_check_conveyor();
  if (BIT_CHK(state[INDEX_P1], STAT_CONVEYOR)) {

    /* Move Right */
    if (game_conveyor_dir == DIR_RIGHT) {
      spr_set_right();
      player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
      return 1;
    }

    /* Move Left */
    if (game_conveyor_dir == DIR_LEFT) {
      spr_set_left();
      player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
      return 1;
    }
  }
  return 0;
}
void player_new_jump() {

  // Pre CHECK
  if (player_check_ceil(lin[INDEX_P1] - 2, col[INDEX_P1])) {
    // NEW JUMP
    if (game_mode == 0) {
      // Willy
      game_gravity = 100;                  // GAME_GRAVITY;
      player_vel_y0 = -(game_gravity * 6); // GAME_VELOCITY;
    } else {
      // Pietro
      game_gravity = 100; // GAME_GRAVITY;
      // player_vel_y0 = -(game_gravity * 7); // GAME_VELOCITY;
      player_vel_y0 = -(game_gravity * 6); // GAME_VELOCITY;
    }

    player_vel_inc = 1;
    player_jump_count = 0xFF;

    audio_jump();
    spr_set_up();
    player_vel_y = player_vel_y0;

    // Set the Lock

    if (player_handle_conveyor()) {

      if (BIT_CHK(state_a[INDEX_P1], STAT_LOCK) &&
          !BIT_CHK(state_a[INDEX_P1], STAT_CONVJMP)) {
        if (dirs & IN_STICK_LEFT) {
          spr_set_left();
        }
        if (dirs & IN_STICK_RIGHT) {
          spr_set_right();
        }
        player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
      }

      BIT_SET(state_a[INDEX_P1], STAT_CONVJMP);
    } else {

      BIT_CLR(state[INDEX_P1], STAT_DIRL);
      BIT_CLR(state[INDEX_P1], STAT_DIRR);

      // Normal JUMP
      if (dirs & IN_STICK_LEFT) {
        spr_set_left();
      }
      if (dirs & IN_STICK_RIGHT) {
        spr_set_right();
      }
      // Not facing jump if disabled
      player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
    }

    BIT_CLR(state[INDEX_P1], STAT_CONVEYOR);
    BIT_CLR(state_a[INDEX_P1], STAT_CONVJMP);
    // BIT_CLR(state_a[INDEX_P1], STAT_LOCK);
  }
}

void player_collision() {
  // Left
  index1 = spr_calc_index(lin[INDEX_P1], col[INDEX_P1]);
  player_pick();

  index1 = spr_calc_index(lin[INDEX_P1] + 14, col[INDEX_P1]);
  player_pick();

  // Right
  index1 = spr_calc_index(lin[INDEX_P1], col[INDEX_P1] + 1);
  player_pick();

  index1 = spr_calc_index(lin[INDEX_P1] + 14, col[INDEX_P1] + 1);
  player_pick();

  // Sprite Collision
  for (i = 0; i < INDEX_P1; ++i) {
    if (class[i] > 0) {
      v0 = abs(col[i] - col[INDEX_P1]);
      if (v0 < 2) {

        v0 = abs(lin[i] - lin[INDEX_P1]);
        if (lin[i] < lin[INDEX_P1]) {
          if (v0 < 16) {
            v0 = 1;
          } else {
            v0 = 0;
          }
        } else {
          // TODO SPRITE HEIGHT'S FOR MORE PREC
          if (v0 < 14) {
            v0 = 1;
          } else {
            v0 = 0;
          }
        }
        // v0 = abs(lin[i] - lin[INDEX_P1]);
        if (v0) {
          v0 = abs(colint[i] - colint[INDEX_P1]);
          if (v0 < 2) {
            player_kill_index = 0xFFFF;
            BIT_SET(state[INDEX_P1], STAT_KILLED);
          }
        }
      }
    }
  }
}

void player_pick(void) {
  v0 = scr_map[index1];
  v0 = player_pick_deadly(v0);
  v0 = player_pick_item(v0, index1);
  v0 = player_pick_extra(v0);
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

void player_tile(unsigned char f_tile, unsigned char f_inc) {
  tile[INDEX_P1] = spr_get_tile_dir(&f_tile, &f_inc);
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
    player_score_add(100);
    scr_map[l_index] = TILE_EMPTY;
    ++player_coins;
    audio_coin();
    if (obj_count == player_coins) {
      audio_door_open();
      game_flash_exit(FLASH);
      obj_count = 0xFF;
    }
    return TILE_EMPTY;
  }
  return l_val;
}

unsigned char player_pick_deadly(unsigned char l_val) {

  l_val = tile_class[l_val];

  if (l_val == TILE_DEADLY) {
    BIT_SET(state[INDEX_P1], STAT_KILLED);
    player_kill_index = index1;
    v0 = TILE_EMPTY;
    v1 = TILE_EMPTY;
    return 0;
  }
  return l_val;
}


unsigned char player_pick_extra(unsigned char l_val) {

  if (l_val == TILE_EXTRA) {

    // HACK
    if (scr_curr == 7 || scr_curr == 11) {
      // 7 Miner Willy meets the kong beast
      scr_map[index1] = TILE_EXTRA_OFF;
      game_cell_paint_index();

      if (index1 == 5 || index1 == 6) {
        // Switch 1
        index1 = 352 + 17;
        scr_map[index1] = TILE_EMPTY;
        game_cell_paint_index();
        index1 = 352 + 32 + 17;
        scr_map[index1] = TILE_EMPTY;
        game_cell_paint_index();
        value_b[2] = 18;
      }
      if (index1 == 18) {
        // Switch 2
        tile[0] = tile[0] + 2;
        spr_kind[0] = E_FALL;
        spr_speed[0] = 4;
        index1 = 64 + 15;
        scr_map[index1] = TILE_EMPTY;
        game_cell_paint_index();
        index1 = 64 + 16;
        scr_map[index1] = TILE_EMPTY;
        game_cell_paint_index();
      }
    }

    return 0;
  }
  return l_val;
}

unsigned char player_check_ceil(unsigned char f_lin, unsigned char f_col) {
  // TODO OPTIMIZA LIKE CHECK FLOOR JUMP/WALK
  // if (colint[INDEX_P1] < 3) {
  index1 = spr_calc_index(f_lin, f_col);
  v0 = scr_map[index1];
  //} else {
  //  v0 = TILE_EMPTY;
  //}

  // if (colint[INDEX_P1] > 0) {
  index1 = spr_calc_index(f_lin, f_col + 1);
  v1 = scr_map[index1];
  //} else {
  //  v1 = TILE_EMPTY;
  //}

  if (v0 > 31)
    v0 = TILE_EMPTY;
  if (v1 > 31)
    v1 = TILE_EMPTY;

  v0 = tile_class[v0];
  v1 = tile_class[v1];

  if (v0 != TILE_WALL && v1 != TILE_WALL) {
    return 1;
  }

  return 0;
}

void player_check_conveyor() {

  // if (!BIT_CHK(state_a[INDEX_P1], STAT_LOCK) || BIT_SET(state_a[INDEX_P1],
  // STAT_CONVJMP)) {

  index1 = spr_calc_index(lin[INDEX_P1] + 16, col[INDEX_P1]);

  v0 = tile_class[scr_map[index1]];
  v1 = tile_class[scr_map[index1 + 1]];
  if (v0 == TILE_CONVEYOR || v1 == TILE_CONVEYOR) {
    BIT_SET(state[INDEX_P1], STAT_CONVEYOR);
  } else {
    BIT_CLR(state[INDEX_P1], STAT_CONVEYOR);
  }
  //}
}

unsigned char player_check_floor(unsigned char f_inc) {

  index1 = spr_calc_index(lin[INDEX_P1] + 16, col[INDEX_P1] + f_inc);

  if (v0 > 31) {
    v0 = TILE_EMPTY;
  }
  v0 = tile_class[scr_map[index1]];

  if (v0 == TILE_EMPTY || v0 == TILE_OBJECT || v0 == TILE_DEADLY) {
    return 1;
  }

  if (v0 == TILE_CRUMB) { // TODO CAUTION!


/*
    //Hack para romper solo el piso en los bordes, ojo con el joystick es la unica forma
    if ( !(dirs & IN_STICK_LEFT) && !(dirs & IN_STICK_RIGHT) ) {
      if (f_inc == 0 && colint[INDEX_P1] == 3) {
        return 1;
      }
      if (f_inc == 1 && colint[INDEX_P1] == 0) {
        return 1;
      }
    }
*/
    s_col1 = col[INDEX_P1] + f_inc;

    if (scr_map[index1] == TILE_CRUMB_INIT) {
      audio_crumble();
      scr_map[index1] = TILE_CRUMB_START;
    } else {
      ++scr_map[index1];
      //++scr_map[index1];
      if (scr_map[index1] > TILE_CRUMB_END) {
        scr_map[index1] = TILE_EMPTY;
      }
    }

    // GASTA BRICK TODO OPTIMIZE CALC
    s_row1 = (((lin[INDEX_P1] + 16) >> 3) + 1) << 3;
    spr_draw8(scr_map[index1], s_row1, s_col1);
  }

  return 0;
}

unsigned char player_get_floor() {
  index1 = spr_calc_index(lin[INDEX_P1] + 16, col[INDEX_P1]);

  i = 0;
  while (i < 16) {

    v0 = scr_map[index1];
    v1 = scr_map[index1 + 1];

    // Remove sprites 16x16, exits,
    if (v0 > 31)
      v0 = TILE_EMPTY;
    if (v1 > 31)
      v1 = TILE_EMPTY;

    v0 = tile_class[v0];
    v1 = tile_class[v1];

    if ((v0 == TILE_EMPTY || v0 == TILE_OBJECT || v0 == TILE_DEADLY) &&
        (v1 == TILE_EMPTY || v1 == TILE_OBJECT || v1 == TILE_DEADLY)) {
      index1 = index1 + 32;
      ++i;
    } else {
      return ((index1 >> 5) << 3) - 16; // /32 *8 -16 Optimizar
    }
  }
  return GAME_LIN_FLOOR - 24;
}

void player_lost_life() {
  unsigned char i;

  ay_reset();
  audio_dead();

  // BLINK
  while (i < 32) {

    if (player_kill_index != 0xFFFF) {
      index1 = player_kill_index;
      NIRVANAP_spriteT(NIRV_SPRITE_P1, tile[INDEX_P1] + colint[INDEX_P1],
                       lin[INDEX_P1] + 16, col[INDEX_P1]);
      NIRVANAP_halt();
      NIRVANAP_spriteT(NIRV_SPRITE_P1, 0, 0, 0);
      NIRVANAP_halt();
      game_cell_paint_index(); // TODO REMOVE
      NIRVANAP_halt();
    } else {
      NIRVANAP_spriteT(NIRV_SPRITE_P1, tile[INDEX_P1] + colint[INDEX_P1],
                       lin[INDEX_P1] + 16, col[INDEX_P1]);
      NIRVANAP_halt();
      NIRVANAP_spriteT(NIRV_SPRITE_P1, 0, 0, 0);
      NIRVANAP_halt();
    }
    ++i;
  }

  // z80_delay_ms(600);
  s_lin0 = lin[INDEX_P1];
  s_col0 = col[INDEX_P1];

  // Player lost life
  if (!game_inf_lives) {
    --player_lives;
  }

  if (player_lives > 0) {
    // z80_delay_ms(500);
    game_round_init();
  } else {
    // Game End
    player_lives = 0;
    game_over = 1;
  }
}
