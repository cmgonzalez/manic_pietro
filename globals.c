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

//General Use VARIABLES
unsigned char i;
unsigned char v0;
unsigned char v1;

//###############################################################################################
//# # # SCREEN GAME MAP
//###############################################################################################

unsigned char scr_map[512];
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
unsigned char value_a[8]; // SPRITE VALUE A MIN COL/LIN
unsigned char value_b[8]; // SPRITE VALUE B MAX COL/LIN


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

unsigned char obj_lin[8]; // object lin for HIGHLIGHT
unsigned char obj_col[8]; // object col for HIGHLIGHT
unsigned char obj_count;


// PLAYER ONLY
unsigned int player_score;    // SCORE
unsigned char player_lives;
unsigned char player_coins;
signed int player_vel_y;
signed int player_vel_y0;
signed char player_vel_inc;

unsigned char player_keys[4];
unsigned char player_hit_count;
unsigned char player_jump_top;
// PLAYER ATTRIBUTES
unsigned char player_vita;
// Maximun Mana n Vitality
unsigned char player_max_mana;
unsigned char player_max_vita;

unsigned char player_col_scr;
unsigned char player_lin_scr;




unsigned int  player_hit_time;
unsigned char player_hit;
unsigned char player_jumpcount;


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
unsigned char game_fps_show;

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
unsigned char attrib_red[4];
unsigned char attrib_osd[4];

//TILE ATTRIB TODO REMOVE
unsigned char attrib0[4];
unsigned char attrib1[4];
unsigned char attrib2[4];
unsigned char attrib3[4];
unsigned char attrib4[4];
unsigned char attrib5[4];
unsigned char attrib6[4];
unsigned char attrib7[4];
unsigned char attrib8[4];

unsigned char *p_state;
unsigned char *p_state_a;


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



//###############################################################################################
//# # # ENEMIES ANIMATION SPEEDS - INTERRUPTS VALUES 50HZ
//# #
//#
//###############################################################################################

//Enemy intialization variables, based on index on map array, used along GAME_TOTAL_INDEX_CLASSES.
unsigned char spr_init[] = {
  //TILE INDEX ON MAP FILE, CLASS OF ENEMY TO CREATE, SPRITE DIRECTION IF APLLY
  16, GUARDIAN_HOR1, DIR_LEFT,
  17, GUARDIAN_HOR1, DIR_RIGHT,
  18, GUARDIAN_HOR2, DIR_LEFT,
  19, GUARDIAN_HOR2, DIR_RIGHT,
  20, GUARDIAN_VER_UP, DIR_NONE,
  21, GUARDIAN_VER_DOWN, DIR_NONE,
  22, SKYLAB, DIR_NONE,
  23, EUGENE, DIR_NONE,
};

unsigned char sprite_kind[] = {
  0,// PLAYER
  E_HORIZONTAL,
  E_HORIZONTAL,
  E_VERTICAL,
  E_VERTICAL,
  E_GOTA,
  E_EUGENE,
};

unsigned char sprite_speed[8];
unsigned char sprite_base_speed[] = {
    4,// PLAYER
    4,// E_HORIZONTAL
    4,// E_HORIZONTAL
    2,// E_VERTICAL
    2,// E_VERTICAL
    2,// E_GOTA
    2,// E_EUGENE
};

//Sprite tile and animation frames for init, used with GAME_TOTAL_CLASSES
unsigned char spr_map_tile[] = {
  //ENEMY Class     ,TILE INDEX ,DIR INC
  GUARDIAN_HOR1     ,144        ,4,
  GUARDIAN_HOR2     ,144        ,4,
  GUARDIAN_VER_UP   ,148        ,4,
  GUARDIAN_VER_DOWN ,152        ,4,
  EUGENE            ,156        ,4,

};

unsigned char sprite_frames[] = { //TODO REPLACE CONSTANT WITH FIXED VALUES TO SIMPLIFY
  4,// PLAYER
  4,// GUARDIAN_HOR1
  4,// GUARDIAN_HOR2
  4,// GUARDIAN_VER_UP
  4,// GUARDIAN_VER_DOWN
  4,// SKYLAB
  4,// EUGENE
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
