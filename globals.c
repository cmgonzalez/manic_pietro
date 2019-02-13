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

unsigned char tbuffer[7]; // Used for ASM PRINT

//###############################################################################################
//# # # SCREEN GAME MAP
//###############################################################################################

unsigned char scr_map[512]; // 16*32
unsigned char scr_curr;

//#########################################################################################
//# # # CONTROL VARIABLES
//# #
//#
//###############################################################################################

uint16_t (*joyfunc1)(udk_t *); // pointer to joystick function Player 1
//uint16_t (*joyfunc2)(udk_t *); // pointer to joystick function for game_control_mode
udk_t k1;

unsigned char dirs;
unsigned char dirs_last;

// SPRITES GAME ARRAYS
unsigned char class[GAME_MAX_SPRITES]; // CLASS OF SPRITE
 // SPRITE STATES SEE DEFINES UPPER BIT VALUES
unsigned char state[GAME_MAX_SPRITES];
unsigned char state_a[GAME_MAX_SPRITES];
 // MISC SPRITE VALUES  MIN COL/LIN
unsigned char value_a[GAME_MAX_SPRITES];
unsigned char value_b[GAME_MAX_SPRITES];
unsigned char value_c[GAME_MAX_SPRITES];

unsigned char spr_tile[GAME_MAX_SPRITES];

unsigned char spr_speed[GAME_MAX_SPRITES];
unsigned char spr_speed_b[GAME_MAX_SPRITES];
unsigned char spr_speed_a[GAME_MAX_SPRITES];
unsigned int last_time[GAME_MAX_SPRITES];   // LAST TIME OF MOVEMENT FOR ANIMATIONS / SPEED
unsigned int last_time_a[GAME_MAX_SPRITES]; // LAST TIME A
unsigned int last_time_b[GAME_MAX_SPRITES]; // LAST TIME B

unsigned char spr_frames[GAME_MAX_SPRITES];
unsigned char spr_altset[GAME_MAX_SPRITES];
unsigned char spr_kind[GAME_MAX_SPRITES];

unsigned char tile[GAME_MAX_SPRITES]; // TILE
unsigned char lin[GAME_MAX_SPRITES];  // LINE

unsigned char col[GAME_MAX_SPRITES];      // COLUMN
unsigned char colint[GAME_MAX_SPRITES];   // INTERNAL COLUMN/TILE INCREMENT


unsigned char obj_lin[GAME_MAX_OBJECTS]; // object lin for HIGHLIGHT
unsigned char obj_col[GAME_MAX_OBJECTS]; // object col for HIGHLIGHT

unsigned char obj_count;

// PLAYER ONLY
uint64_t player_score; // SCORE
unsigned char player_lives;
unsigned char player_coins;
signed int player_vel_y;
signed int player_vel_y0;
signed char player_vel_inc;

unsigned char player_col_scr;
unsigned char player_lin_scr;

unsigned int player_kill_index;

unsigned char nirv_sprite_index;

signed int game_gravity;
unsigned char game_song_play;
unsigned char game_song_play_start;
unsigned char game_conveyor_dir;
unsigned char game_conveyor_lin;
unsigned char game_conveyor_col0;
unsigned char game_conveyor_col1;
unsigned char game_exit_col;
unsigned char game_exit_lin;
unsigned char game_code;


unsigned char game_tileset;
unsigned char game_mode;
unsigned char game_atrac;
unsigned char game_lives;

unsigned char game_round_up;
unsigned char menu_curr_sel;

unsigned int fps;

unsigned char sprite;
unsigned char game_debug;
unsigned char game_fps_show;

unsigned char s_lin0;
unsigned char s_lin1;
unsigned char s_col0;
unsigned char s_col1;
unsigned char s_row1;
unsigned int loop_count;

unsigned int index0;
unsigned int index1;

unsigned char zx_val_asm;
unsigned char attrib[4];
unsigned char attrib_hl[4];
unsigned char attrib_osd[4];

unsigned char attrib_sol0[4];
unsigned char attrib_sol1[4];
unsigned char g_ray1;



// TILE ATTRIB TODO REMOVE UNUSED
unsigned char key_attrib[4];


unsigned char *gbyte;



unsigned int curr_time;
unsigned int frame_time;
unsigned int time_air;
unsigned int time_conv;
unsigned int time_key;
unsigned char lapse_crumb;

unsigned char key_last;
unsigned char key_ink;

unsigned char spr_count;


//###############################################################################################
//# # # GAME VARIABLES
//# #
//#
//###############################################################################################

unsigned char game_inmune;
unsigned char game_inf_lives;

unsigned char game_sound;
unsigned char game_tile_cnt;

unsigned char game_over;

unsigned char game_crumb_start;
unsigned char game_crumb_end;

uint64_t game_score_top;
uint64_t player_next_extra;
unsigned char game_start_scr;

// PHASE RELATED VARIABLES
unsigned char screen_paper;
unsigned char screen_ink;

unsigned char map_border;
unsigned char map_paper;
unsigned char map_clear;

unsigned char *air_curr_byte;
const unsigned int air_start_byte = 21087;
const unsigned int air_end_byte = 21059;


//###############################################################################################
//# # # ENEMIES ANIMATION SPEEDS - INTERRUPTS VALUES 50HZ
//# #
//#
//###############################################################################################

// Enemy intialization variables, based on index on map array, used along
// GAME_TOTAL_INDEX_CLASSES.
// column to save some bytes

//Sprite Index on MAP Data
//Index on btile from bank 3
//Frames
//Horizontal Dir Set (should be equal to frames if the sprite have diferent tiles for right left)
//Sprite KIND
//Base INK for REPLACE
//Variant INK1
//Variant INK2
//Variant INK3


//28*9
const unsigned char spr_init[GAME_ENEMY_MAX_CLASSES*GAME_ENEMY_CLASS_LEN];
//Sprite color change
unsigned char spr_init_cin0;
unsigned char spr_init_cout0;
unsigned char spr_init_cin1;
unsigned char spr_init_cout1;
unsigned char spr_init_cin2;
unsigned char spr_init_cout2;
unsigned char spr_init_cin3;
unsigned char spr_init_cout3;
unsigned char spr_init_bright;

unsigned char spr_clr;

unsigned char *attribs;
unsigned char *deltas;

const unsigned char key_map[] = {
    13,  32,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  97,
    98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
    111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
};

const unsigned int scan_map[] = {
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

const unsigned char tile_class[32];

//Hack for horizontal jump increment

unsigned char player_jump_count;
unsigned char player_jump_lin;
unsigned char player_jump_top;
unsigned char player_fall_start;

//CODE
unsigned char code0[GAME_CODELEN];
unsigned char code1[GAME_CODELEN];

//Jump Geometry HORIZONTAL extra increment to emulate Willy Jump

const unsigned char player_jump_hor[] = {
  0, //0
  0, //1
  0, //2
  0, //3
  1, //4
  0, //5
  1, //6
  1, //7
  0, //8
  1, //9
  0, //10
  0, //11
  1, //12
  0, //13
  0, //14
  1, //15
  0  //16
};

const unsigned char game_encode[10] = {
  'Z', //0
  'X', //1
  'S', //2
  'P', //3
  'E', //4
  'C', //5
  'T', //6
  'R', //7
  'U', //8
  'M'  //9
};
