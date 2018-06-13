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
  class[SPR_P1] = PLAYER;
  sprite_speed[SPR_P1] = sprite_base_speed[0];

  tile[SPR_P1] = f_tile;
  lin[SPR_P1] = f_lin; //*SPRITELIN(SPR_P1);
  col[SPR_P1] = f_col; //*SPRITECOL(SPR_P1);
  colint[SPR_P1] = 0;
  state[SPR_P1] = 0;
  state_a[SPR_P1] = 0;
  jump_lin[SPR_P1] = f_lin;
  last_time[SPR_P1] = zx_clock();
  //BIT_SET(state_a[SPR_P1], STAT_LOCK);
  // PLAYER ONLY VARIABLES
  BIT_SET(state_a[SPR_P1], STAT_LDIRR);
  NIRVANAP_spriteT(SPR_P1, f_tile, f_lin + 16, f_col);
}

void player_turn(void) {
  if (spr_chktime(&sprite)) {
    zx_border(INK_BLACK); // TODO REMOVE ME ONLY FOR COLISION DETECTION
    dirs = (joyfunc1)(&k1);
    /* Player initial Values */
    // TODO POINTERS
    s_lin0 = lin[SPR_P1];
    s_col0 = col[SPR_P1];
    s_state = state[SPR_P1];
    s_class = 0;
    player_move();
    // player_collision();
    /* Store State variable */
    state[SPR_P1] = s_state;
  }
}

unsigned char player_move(void) {

  if (BIT_CHK(s_state, STAT_JUMP) || BIT_CHK(s_state, STAT_FALL)) {
    /* Jump Handling */

    player_move_jump();
    /*Handle Fire*/
  } else {
    /* Read player input */
    player_move_walk();

    /* Check if the player have floor, and set fall if not */
    if (player_check_floor1(lin[SPR_P1] + 16, col[SPR_P1])) {
      BIT_SET(s_state, STAT_FALL);
      BIT_CLR(s_state, STAT_DIRL);
      BIT_CLR(s_state, STAT_DIRR);
    }

  }
  spr_paint_player();

  return 0;
}

unsigned char player_check_input(void) {

  if (dirs & IN_STICK_LEFT && dirs & IN_STICK_RIGHT) {
    // Error on reading both horizontal's
    dirs = 0;
  }

  if (dirs & IN_STICK_FIRE) {
    return 1;
  }

  if (dirs & IN_STICK_LEFT || dirs & IN_STICK_RIGHT) {
    return 1;
  }

  return 0;
}

unsigned char player_move_walk(void) {
  // TODO CLEAN THIS!
  /* User have pressed valid input */

  if (player_check_input()) {

    if (dirs & IN_STICK_FIRE) {
      // NEW JUMP
      player_vel_inc = 1;
      audio_salto();
      spr_set_up(&s_state);
      jump_lin[SPR_P1] = lin[SPR_P1];
      player_vel_y = player_vel_y0;

      BIT_CLR(s_state, STAT_DIRL);
      BIT_CLR(s_state, STAT_DIRR);

      if (dirs & IN_STICK_LEFT) {
        spr_set_left(&s_state);
      }

      if (dirs & IN_STICK_RIGHT) {
        spr_set_right(&s_state);
      }

      state[SPR_P1] = s_state;
      player_tile(TILE_P1_RIGHT, TILE_P1_LEN);
      return 1;
    }

    /* Move Right */
    if (dirs & IN_STICK_RIGHT) {
      spr_set_right(&s_state);
      // BIT_SET(state_a[SPR_P1], STAT_LDIRR);
      // BIT_CLR(state_a[SPR_P1], STAT_LDIRL);
      spr_move_horizontal();
    }

    /* Move Left */
    if (dirs & IN_STICK_LEFT) {
      spr_set_left(&s_state);
      // BIT_SET(state_a[SPR_P1], STAT_LDIRL);
      // BIT_CLR(state_a[SPR_P1], STAT_LDIRR);
      spr_move_horizontal();
    }

    /* Set Tile according to current direction */
    state[SPR_P1] = s_state; // TODO FIX SPR_TILE
    player_tile(TILE_P1_RIGHT, TILE_P1_LEN);

    return 1;
  } else {
    BIT_CLR(s_state, STAT_DIRL);
    BIT_CLR(s_state, STAT_DIRR);
    if (BIT_CHK(s_state, STAT_CHAIN)) {

      BIT_SET(s_state, STAT_DIRL); //TODO
      state[SPR_P1] = s_state; //TODO!!! POINTERS
      player_tile(TILE_P1_RIGHT, TILE_P1_LEN);

      spr_move_horizontal();
    }
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
  tile[SPR_P1] = spr_tile_dir(&f_tile, &sprite, &f_inc);
}

void player_score_add(unsigned int f_score) __z88dk_fastcall {
  player_score = player_score + f_score;

  // CHECK FOR TOP SCORE
  if (player_score > game_score_top) {
    game_score_top = player_score;
  }
  game_print_score();
}

unsigned char player_check_floor(unsigned char f_lin, unsigned char f_col) {

  unsigned char v0;
  unsigned char v1;


  index1 = spr_calc_index(f_lin, f_col);

  v0 = scr_map[index1];
  v1 = scr_map[index1 + 1];

  if (colint[SPR_P1] == 0) {
    v1 = 0;
  }
  if (colint[SPR_P1] == sprite_frames[SPR_P1] - 1) {
    v0 = 0;
  }

  if (v0 == 0 && v1 == 0) {
    return 1;
  }



  if (v0 == TILE_DEADLY1 || v1 == TILE_DEADLY1 || v0 == TILE_DEADLY2 ||
      v1 == TILE_DEADLY2) {
    zx_border(INK_RED);
  }

  if (v0 == TILE_OBJECT) {

    scr_map[index1] = TILE_EMPTY;
    ++player_coins;
  }

  if (v1 == TILE_OBJECT) {

    scr_map[index1 + 1] = TILE_EMPTY;
    ++player_coins;
  }

  return 0;
}

unsigned char player_check_floor1(unsigned char f_lin, unsigned char f_col) {
  unsigned char v0;
  unsigned char v1;

  index1 = spr_calc_index(f_lin, f_col);

  v0 = scr_map[index1];
  v1 = scr_map[index1 + 1];

  if (colint[SPR_P1] == 0) {
    v1 = 0;
  }
  if (colint[SPR_P1] == sprite_frames[SPR_P1] - 1) {
    v0 = 0;
  }

  if (v0 == 0 && v1 == 0) {
    return 1;
  }
  // GASTA BRICK TODO OPTIMIZE CODE
  if (v0 >= TILE_FLOOR0 && v0 <= TILE_FLOOR7) {

    if (v0 < TILE_FLOOR6) {
      scr_map[index1] = v0 + 1;
    } else {
      scr_map[index1] = TILE_EMPTY;
    }
    s_col1 = f_col;
    s_lin1 = f_lin >> 3;
    s_lin1 = s_lin1 << 3;
    s_lin1 = s_lin1;
    s_row1 = (s_lin1 >> 3) + 1;
    game_cell_paint();
  }

  if (v1 >= TILE_FLOOR0 && v1 <= TILE_FLOOR7) {
    ++index1;
    if (v1 < TILE_FLOOR6) {
      scr_map[index1] = v1 + 1;
    } else {
      scr_map[index1] = TILE_EMPTY;
    }

    s_col1 = f_col + 1;
    s_lin1 = f_lin >> 3;
    s_lin1 = s_lin1 << 3;
    s_lin1 = s_lin1;
    s_row1 = (f_lin >> 3) + 1;
    game_cell_paint();
  }

  if (v0 == TILE_CHAIN || v1 == TILE_CHAIN) {
    BIT_SET(s_state, STAT_CHAIN);
  } else {
    BIT_CLR(s_state, STAT_CHAIN);
  }

  if (v0 == TILE_DEADLY1 || v1 == TILE_DEADLY1 || v0 == TILE_DEADLY2 ||
      v1 == TILE_DEADLY2) {
    zx_border(INK_RED);
  }

  if (v0 == TILE_OBJECT) {
    scr_map[index1] = TILE_EMPTY;
    ++player_coins;
  }

  if (v1 == TILE_OBJECT) {
    scr_map[index1 + 1] = TILE_EMPTY;
    ++player_coins;
  }

  return 0;
}

unsigned char player_check_ceil(unsigned char f_lin, unsigned char f_col) {
  unsigned char v0;
  unsigned char v1;

  index1 = spr_calc_index(f_lin, f_col);

  v0 = scr_map[index1];
  v1 = scr_map[index1 + 1];

  if (colint[SPR_P1] == 0) {
    v1 = 0;
  }

  if (colint[SPR_P1] == sprite_frames[SPR_P1] - 1) {
    v0 = 0;
  }

  if (v0 == TILE_DEADLY1 || v1 == TILE_DEADLY1 || v0 == TILE_DEADLY2 ||
      v1 == TILE_DEADLY2) {
    zx_border(INK_RED);
  }

  if (v0 == TILE_OBJECT) {
    scr_map[index1] = TILE_EMPTY;
    ++player_coins;
  }

  if (v1 == TILE_OBJECT) {
    scr_map[index1 + 1] = TILE_EMPTY;
    ++player_coins;
  }

  if (v0 != TILE_WALL && v1 != TILE_WALL) {
    return 1;
  }

  return 0;
}

void player_gasta_brick() {
  tmp_uc = s_lin0;
  s_lin1 = s_lin0;
  s_col1 = s_col0;
  s_lin0 = lin[SPR_P1] + 16;
  s_col0 = col[SPR_P1];
  spr_back_repaint(); // restore background
  s_lin0 = tmp_uc;
  s_col0 = s_col1;
}

unsigned char player_move_jump(void) {

  signed int val_yc;

  player_vel_y = player_vel_y + game_gravity;

  // MAX SPEEDS
  if (player_vel_y > 120) {
    player_vel_y = 120;
  }

  if (player_vel_y < -120) {
    player_vel_y = -120;
  }
  // CONVER TO PIXEL'S
  val_yc = player_vel_y;

  s_lin1 = (unsigned char)val_yc;

  // Nirvana don't support odds lines
  if ((s_lin1 & 1) != 0) {
    s_lin1--;
  }

  s_lin1 = lin[SPR_P1] + s_lin1;

  if (val_yc < 0) {
    // Asending
    spr_set_up(&s_state);
    if (s_lin1 > GAME_LIN_FLOOR) {
      s_lin1 = 0;
    }
    if (!player_check_ceil(s_lin1, col[SPR_P1])) {
      player_vel_y = 0;
    } else {
      lin[SPR_P1] = s_lin1;
    }

    player_jump_top = s_lin1;
  } else {
    // Falling
    spr_set_down(&s_state);

    if (s_lin1 > GAME_LIN_FLOOR) {
      s_lin1 = GAME_LIN_FLOOR;
    }

    // 7z80_delay_ms(50);
    // if (game_check_map(s_lin1 + 16, col[SPR_P1]) ||
    //  game_check_map(s_lin1 + 16, col[SPR_P1] + 1)) {

    if (s_lin1 - player_jump_top > 2) {

      if (!player_check_floor(s_lin1 + 16, col[SPR_P1])) {
        // Jump end
        tmp_uc = (s_lin1 >> 3) << 3;

        if (tmp_uc - s_lin1 <= 2) {
          lin[SPR_P1] = tmp_uc;
          player_vel_y = 0;
          BIT_CLR(s_state, STAT_FALL);
          BIT_CLR(s_state, STAT_JUMP);
          return 1;
        }
      }
    }

    lin[SPR_P1] = s_lin1;
  }
  /*
    if (!BIT_CHK(s_state, STAT_DIRL) && !BIT_CHK(s_state, STAT_DIRR)) {
      if (dirs & IN_STICK_LEFT) {
        spr_set_left(&s_state);
      }
      if (dirs & IN_STICK_RIGHT) {
        spr_set_right(&s_state);
      }
    }
  */
  spr_move_horizontal();
  spr_move_horizontal();
  return 0;
}

void player_lost_life() {
  unsigned char i;
  // ay_fx_play(ay_effect_18);

  player_vita = 0;
  game_update_stats();
  s_lin0 = lin[SPR_P1];
  s_col0 = col[SPR_P1];
  spr_init_effects();
  for (i = 0; i <= SPR_P1; i++) {
    NIRVANAP_spriteT(i, TILE_EMPTY, 0, 0);
  }
  ay_reset();
  audio_explosion();
  audio_lotr_lose_a_life();

  // Player lost life
  if (!game_inf_lives) {
    --player_lives;
  }
  player_vita = GAME_START_MAX_VITA;
  // game_update_stats();

  if (player_lives > 0) {
    z80_delay_ms(500);
    game_round_init();
  } else {
    // Game End
    player_lives = 0;
    game_over = 1;
  }
}

void player_damage(unsigned char f_val) __z88dk_fastcall {
  if (player_vita > f_val) {
    if (!game_inmune)
      player_vita = player_vita - f_val;
    player_hit_time = curr_time;
    player_hit = 1;
    audio_golpe();
    game_update_stats();
  } else {
    player_lost_life();
  }
}
