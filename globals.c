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
#include "globals.h"
#include <input.h>

unsigned char spec128;
//###############################################################################################
//# # # SCREEN GAME MAP
//# #
//#
//###############################################################################################
// TILE MAP'S
//  0..23   BACKGROUND
// 24..31   OBJECTS (CAN BE PICKED)
// 32..39   BACKGROUND DEADLY
// 40..47   PLATFORMS (FLOOR ONLY)
// 48..51   SPECIAL PLATFORMS (STAIRS)
// 52..55   BRICKS (FLOOR AND CEIL)
// 56..63   SPECIAL BRICKS (FLORR AND CEIL CAN BE HITTED)
//unsigned char scr_map[GAME_SCR_MAX_INDEX];
unsigned char scr_map[] = { //16 * 32
//0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  4,0,0,0,0,0,0,0,0,0,7,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4, //0
  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4, //1
  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4, //2
  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4, //3
  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,6,0,0,0,4, //4
  4,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,1,3,3,3,1,1,1,1,1,1,1,1,1,1,4, //5
  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4, //6
  4,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4, //7
  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,6,0,0,0,0,0,0,0,0,0,0,4, //8
  4,1,1,1,1,0,0,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,4, //9
  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,4, //10
  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4, //11
  4,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,4,4,4,4,3,3,3,3,3,1,1,1,4, //12
  4,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,4, //13
  4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4, //14
  4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4, //15


};

//unsigned char scr_obj0[GAME_SCR_MAX_INDEX]; // OBJECT PICK  0..15 SCREENS
//unsigned char scr_obj1[GAME_SCR_MAX_INDEX]; // OBJECT PICK 16..31 SCREENS

unsigned char scr_curr;
unsigned char map_width;
unsigned char map_heigth;

// Compressed MAP is in globals_bank06.c

//#########################################################################################
//# # # CONTROL VARIABLES
//# #
//#
//###############################################################################################

uint16_t (*joyfunc1)(udk_t *); // pointer to joystick function Player 1
uint16_t (*joyfunc2)(udk_t *); // pointer to joystick function for game_control_mode
udk_t k1;
udk_t k2; // For game_control_mode 2B

unsigned char dirs;

// SPRITES GAME ARRAYS
unsigned char class[8];   // CLASS OF SPRITE
unsigned char state[8];   // SPRITE STATES SEE DEFINES UPPER BIT VALUES
unsigned char state_a[8]; // SPRITE STATES ALT SEE DEFINES UPPER BIT VALUES
unsigned char tile[8];    // TILE
unsigned char lin[8];     // LINE
unsigned char tbuffer[7];

// unsigned char lin_max[7];      // MAX LINE FOR VERTICAL ENEMIES
// unsigned char lin_min[7];      // MIN LINE FOR VERTICAL ENEMIES

unsigned char col[8];      // COLUMN
unsigned char colint[8];   // INTERNAL COLUMN/TILE INCREMENT
unsigned int spr_timer[8]; // SPRITE GENERAL TIMER MILISECONDS
unsigned int last_time[8]; // LAST TIME OF MOVEMENT FOR ANIMATIONS / SPEED
unsigned char jump_lin[8]; // START JUMP LINE

// PLAYER ONLY
unsigned int player_score;    // SCORE
unsigned char player_lives;
unsigned char player_coins;
signed char player_vel_x;
signed char player_vel_y;
signed char player_vel_y0;
signed char player_vel_y1;
signed char player_vel_inc;
unsigned char player_keys[4];
unsigned char player_hit_count;
unsigned int player_brick_time;
// PLAYER ATTRIBUTES
unsigned char player_vita;
// Maximun Mana n Vitality
unsigned char player_max_mana;
unsigned char player_max_vita;

unsigned char player_col_scr;
unsigned char player_lin_scr;


unsigned char game_checkpoint_col;
unsigned char game_checkpoint_lin;
unsigned char game_checkpoint_scr;


unsigned int  player_hit_time;
unsigned char player_hit;


signed int    game_gravity;
unsigned char game_world;
unsigned char game_boss;
unsigned char game_boss_hit;
unsigned char game_boss_alive;
unsigned char game_boss_fix;
unsigned char game_song_play;
unsigned char game_song_play_start;


unsigned char game_round_up;
unsigned char game_control_mode;
unsigned char menu_curr_sel;

unsigned int fps;

unsigned char sprite;
unsigned char game_debug;

unsigned char s_tile0;
unsigned char s_tile1;
unsigned char s_lin0;
unsigned char s_lin1;
unsigned char s_col0;
unsigned char s_col1;
unsigned char s_row1;
unsigned char s_class;
unsigned int loop_count;
unsigned int index0;
unsigned int index1;
unsigned char tmp;
unsigned char tmp0;
unsigned char tmp1;
unsigned char tmp_uc;
signed char tmp_sc;
unsigned int tmp_ui;
unsigned char enemies;
unsigned char zx_val_asm;
unsigned char attrib[4];
unsigned char attrib_hl[4];
unsigned char attrib_osd[4];

//TILE ATTRIB
unsigned char attrib0[4];
unsigned char attrib1[4];
unsigned char attrib2[4];
unsigned char attrib3[4];
unsigned char attrib4[4];
unsigned char attrib5[4];
unsigned char attrib6[4];
unsigned char attrib7[4];

unsigned char s_state;


unsigned int curr_time;
unsigned int frame_time;
unsigned int anim_time;

unsigned char anim_count;


unsigned char spr_count;
unsigned char sprite_curr_index;
// ANIMATIONS
unsigned char anim_lin[8];
unsigned char anim_col[8];
unsigned char anim_tile[8];
unsigned char anim_int[8];
unsigned char anim_end[8];
unsigned char anim_loop[8];
unsigned char anim_respanwn[8];

unsigned char boss_lin;
unsigned char boss_col;
unsigned char boss_inc;
unsigned char boss_tile;
unsigned int  boss_time;
unsigned int  boss_time_fire;
unsigned char boss_stat;



//###############################################################################################
//# # # GAME VARIABLES
//# #
//#
//###############################################################################################

unsigned char game_inmune;
unsigned char game_inf_lives;

unsigned char game_sound;
unsigned char game_over;

unsigned int game_score_top;
unsigned char game_start_scr;

// PHASE RELATED VARIABLES
unsigned char screen_paper;
unsigned char screen_ink;

unsigned char map_paper;
unsigned char map_paper_clr;
unsigned char map_paper_last;
unsigned char map_paper_last_a;

unsigned char stp_tile;
unsigned char stp_col;
unsigned char stp_row;

//###############################################################################################
//# # # ENEMIES ANIMATION SPEEDS - INTERRUPTS VALUES 50HZ
//# #
//#
//###############################################################################################

//Enemy intialization variables, based on index on map array, used along GAME_TOTAL_INDEX_CLASSES.
unsigned char spr_init[] = {
  //TILE INDEX ON MAP FILE, CLASS OF ENEMY TO CREATE, SPRITE DIRECTION IF APLLY
  96, BIRD1, DIR_LEFT,
  97, BIRD1, DIR_RIGHT,
  98, ORC, DIR_LEFT,
  99, ORC, DIR_RIGHT,
  100, WARG, DIR_LEFT,
  101, WARG, DIR_RIGHT,
  102, DWARF, DIR_LEFT,
  103, DWARF, DIR_RIGHT,
  104, ELF, DIR_LEFT,
  105, ELF, DIR_RIGHT,
  106, DRAGON, DIR_RIGHT,
  107, DRAGON, DIR_LEFT,
  108, BAT, DIR_NONE,
  109, GHOST, DIR_RIGHT,
  110, SPIDER, DIR_NONE,
  111, PLANT, DIR_NONE,
  112, SNAKE, DIR_NONE,
  113, BAT_H, DIR_NONE,
  114, FIRE, DIR_NONE,
  115, PIRANHA, DIR_NONE,
  116, GHOST, DIR_LEFT,
  117, DEVIL, DIR_NONE,
  118, GOTA, DIR_NONE,
  119, FIRE_S, DIR_NONE,
};

unsigned char sprite_kind[] = {
  0,// PLAYER
  E_GOTA,// GOTA
  E_VERTICAL,// DRAGON
  E_VERTICAL,// BAT
  E_GHOST,// GHOST
  E_VERTICAL,// SPIDER
  E_WALK,// MUSHROOM_VITA
  E_WALK,// MUSHROOM_MANA
  E_WALK,// MUSHROOM_EXTRA
  E_VERTICAL,// FIRE
  E_VERTICAL,// PIRANHA
  E_WALK,// BIRD1
  E_WALK,// ORC
  E_WALK,// WARG
  E_WALK,// DWARF
  E_WALK,// ELF
  E_STATIC,// PLANT
  E_STATIC,// TRAP
  E_STATIC,// SNAKE
  E_HORIZONTAL,// BAT_H
  E_GHOST,// DEVIL
  E_STATIC,// FIRE_S
};

unsigned char sprite_speed[8];
unsigned char sprite_base_speed[] = {
    2,// PLAYER
    4,// GOTA
    6,// DRAGON
    2,// BAT
    8,// GHOST
    2,// SPIDER
    2,// MUSHROOM_VITA
    2,// MUSHROOM_MANA
    2,// MUSHROOM_EXTRA
    2,// FIRE
    1,// PIRANHA
    4,// BIRD1
    3,// ORC
    1,// WARG
    4,// DWARF
    6,// ELF
    8,// PLANT
    0,// TRAP
    8,// SNAKE
    2,// BAT_H
    8,// DEVIL
    2,// FIRE_S
};

//Sprite tile and animation frames for init, used with GAME_TOTAL_CLASSES
unsigned char spr_map_tile[] = {
  //ENEMY Class     ,TILE INDEX ,DIR INC
  GOTA              , 91        ,0,
  BIRD1             ,144        ,4,
  ORC               ,148        ,0,
  WARG              ,152        ,2,
  DWARF             ,156        ,0,
  ELF               ,160        ,4,
  DRAGON            ,168        ,2,
  BAT               ,172        ,0,
  GHOST             ,192        ,2,
  DEVIL             ,174        ,0,
  SPIDER            ,176        ,0,
  PLANT             ,178        ,0,
  SNAKE             ,180        ,0,
  FIRE              ,196        ,0,
  PIRANHA           ,198        ,0,
  BAT_H             ,182        ,0,
  MUSHROOM_VITA     ,185        ,0,
  MUSHROOM_MANA     ,187        ,0,
  MUSHROOM_EXTRA    ,189        ,0,
  FIRE_S            ,94         ,0,
};

unsigned char sprite_frames[] = { //TODO REPLACE CONSTANT WITH FIXED VALUES TO SIMPLIFY
  4,// PLAYER
  3,// GOTA
  2,// DRAGON
  2,// BAT
  2,// GHOST
  2,// SPIDER
  2,// MUSHROOM_VITA
  2,// MUSHROOM_MANA
  2,// MUSHROOM_EXTRA
  1,// FIRE
  1,// PIRANHA
  4,// BIRD1
  4,// ORC
  2,// WARG
  4,// DWARF
  4,// ELF
  2,// PLANT
  0,// TRAP
  2,// SNAKE
  3,// BAT_H
  2,// DEVIL
  2,// FIRE_S
};


unsigned char key_map[] = {
    13,  32,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  97,
    98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
    111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
};
unsigned int scan_map[] = {
    IN_KEY_SCANCODE_ENTER, IN_KEY_SCANCODE_SPACE, IN_KEY_SCANCODE_0,
    IN_KEY_SCANCODE_1,     IN_KEY_SCANCODE_2,     IN_KEY_SCANCODE_3,
    IN_KEY_SCANCODE_4,     IN_KEY_SCANCODE_5,     IN_KEY_SCANCODE_6,
    IN_KEY_SCANCODE_7,     IN_KEY_SCANCODE_8,     IN_KEY_SCANCODE_9,
    IN_KEY_SCANCODE_a,     IN_KEY_SCANCODE_b,     IN_KEY_SCANCODE_c,
    IN_KEY_SCANCODE_d,     IN_KEY_SCANCODE_e,     IN_KEY_SCANCODE_f,
    IN_KEY_SCANCODE_g,     IN_KEY_SCANCODE_h,     IN_KEY_SCANCODE_i,
    IN_KEY_SCANCODE_j,     IN_KEY_SCANCODE_k,     IN_KEY_SCANCODE_l,
    IN_KEY_SCANCODE_m,     IN_KEY_SCANCODE_n,     IN_KEY_SCANCODE_o,
    IN_KEY_SCANCODE_p,     IN_KEY_SCANCODE_q,     IN_KEY_SCANCODE_r,
    IN_KEY_SCANCODE_s,     IN_KEY_SCANCODE_t,     IN_KEY_SCANCODE_u,
    IN_KEY_SCANCODE_v,     IN_KEY_SCANCODE_w,     IN_KEY_SCANCODE_x,
    IN_KEY_SCANCODE_y,     IN_KEY_SCANCODE_z
};
