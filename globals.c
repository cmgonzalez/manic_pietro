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

// General Use VARIABLES
unsigned char i;
unsigned char v0;
unsigned char v1;
unsigned char v2;
unsigned char v3;

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
uint16_t (*joyfunc2)(
    udk_t *); // pointer to joystick function for game_control_mode
udk_t k1;

unsigned char dirs;

// SPRITES GAME ARRAYS
unsigned char class[8];   // CLASS OF SPRITE
unsigned char state[8];   // SPRITE STATES SEE DEFINES UPPER BIT VALUES
unsigned char state_a[8]; // SPRITE STATES ALT SEE DEFINES UPPER BIT VALUES
unsigned char value_a[8]; // SPRITE VALUE A MIN COL/LIN
unsigned char value_b[8]; // SPRITE VALUE B MAX COL/LIN

unsigned char tile[8]; // TILE
unsigned char lin[8];  // LINE
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
unsigned int player_score; // SCORE
unsigned char player_lives;
unsigned char player_coins;
signed int player_vel_y;
signed int player_vel_y0;
signed char player_vel_inc;

unsigned char player_jump_top;
// PLAYER ATTRIBUTES
// Maximun Mana n Vitality

unsigned char player_col_scr;
unsigned char player_lin_scr;
unsigned char player_killed;
unsigned int player_kill_index;

unsigned int player_hit_time;
unsigned char player_hit;
unsigned char player_jumpcount;

signed int game_gravity;
unsigned char game_world;
unsigned char game_song_play;
unsigned char game_song_play_start;
unsigned char game_conveyor_dir;
unsigned char game_conveyor_lin;
unsigned char game_conveyor_col0;
unsigned char game_conveyor_col1;

unsigned char game_round_up;
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

unsigned int index_tmp;
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
// TILE ATTRIB TODO REMOVE UNUSED
unsigned char attrib_key[4];

unsigned char *p_state;
unsigned char *p_state_a;

// TODO REVIEW
unsigned char game_conveyor_tile;
unsigned char game_conveyor_flag;
unsigned char game_attrib_osd;
unsigned char game_tileset;

unsigned int curr_time;
unsigned int frame_time;
unsigned int anim_time;
unsigned char last_rotated;

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
/* Enemies Class indexes */
#define PLAYER 0xFF
#define GUARD1_RIGHT 1      // Different right / left sprite
#define GUARDIAN_HOR2 2     // Simetric Horizonal
#define GUARDIAN_VER_UP 3   //
#define GUARDIAN_VER_DOWN 4 //
#define SKYLAB 5            //
#define EUGENE 6            //

// Enemy intialization variables, based on index on map array, used along
// GAME_TOTAL_INDEX_CLASSES.
const unsigned char spr_init[] = {
    // SPR INDEX, TILE INDEX, SPRITE DIRECTION, FRAMES, SPEED , KIND
    64, 16, DIR_LEFT , 4, 4, E_HORIZONTAL, // ROBOT
    65, 16, DIR_RIGHT, 4, 4, E_HORIZONTAL,
    66, 24, DIR_LEFT , 4, 4, E_HORIZONTAL, // PENGUIN
    67, 24, DIR_RIGHT, 4, 4, E_HORIZONTAL,
    68, 32, DIR_LEFT , 4, 4, E_HORIZONTAL, // CHICKEN
    69, 32, DIR_RIGHT, 4, 4, E_HORIZONTAL,
    70, 40, DIR_LEFT , 4, 4, E_HORIZONTAL, // FOCA
    71, 40, DIR_RIGHT, 4, 4, E_HORIZONTAL,
    72, 48, DIR_LEFT , 4, 4, E_HORIZONTAL, // WATER
    73, 48, DIR_RIGHT, 4, 4, E_HORIZONTAL,
    0,0,0,0,0,0,
};

unsigned char spr_tile[8];
unsigned char spr_speed[8];
unsigned char spr_frames[8];
unsigned char spr_kind[8];

const unsigned char key_map[] = {
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
    IN_KEY_SCANCODE_y,     IN_KEY_SCANCODE_z};
