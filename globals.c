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
unsigned char class[GAME_MAX_SPRITES]; // CLASS OF SPRITE
unsigned char
    state[GAME_MAX_SPRITES]; // SPRITE STATES SEE DEFINES UPPER BIT VALUES
unsigned char
    state_a[GAME_MAX_SPRITES]; // SPRITE STATES ALT SEE DEFINES UPPER BIT VALUES
unsigned char value_a[GAME_MAX_SPRITES]; // SPRITE VALUE A MIN COL/LIN
unsigned char value_b[GAME_MAX_SPRITES]; // SPRITE VALUE B MAX COL/LIN

unsigned char spr_tile[GAME_MAX_SPRITES];
unsigned char spr_speed[GAME_MAX_SPRITES];
unsigned char spr_frames[GAME_MAX_SPRITES];
unsigned char spr_kind[GAME_MAX_SPRITES];

unsigned char tile[GAME_MAX_SPRITES]; // TILE
unsigned char lin[GAME_MAX_SPRITES];  // LINE

unsigned char col[GAME_MAX_SPRITES];      // COLUMN
unsigned char colint[GAME_MAX_SPRITES];   // INTERNAL COLUMN/TILE INCREMENT
unsigned int spr_timer[GAME_MAX_SPRITES]; // SPRITE GENERAL TIMER MILISECONDS
unsigned int
    last_time[GAME_MAX_SPRITES]; // LAST TIME OF MOVEMENT FOR ANIMATIONS / SPEED

unsigned char obj_lin[GAME_MAX_OBJECTS]; // object lin for HIGHLIGHT
unsigned char obj_col[GAME_MAX_OBJECTS]; // object col for HIGHLIGHT

unsigned char obj_count;

// PLAYER ONLY
unsigned int player_score; // SCORE
unsigned char player_lives;
unsigned char player_coins;
signed int player_vel_y;
signed int player_vel_y0;
signed char player_vel_inc;
unsigned char player_jump_hack;
unsigned char player_jump_count;
unsigned char player_jump_top;
unsigned char player_col_scr;
unsigned char player_lin_scr;
unsigned char player_killed;
unsigned int player_kill_index;
unsigned int player_hit_time;
unsigned char player_hit;
unsigned char player_jumpcount;
unsigned char player_fall_start;

unsigned char nirv_sprite_index;

signed int game_gravity;
unsigned char game_world;
unsigned char game_song_play;
unsigned char game_song_play_start;
unsigned char game_conveyor_dir;
unsigned char game_conveyor_lin;
unsigned char game_conveyor_col0;
unsigned char game_conveyor_col1;
unsigned char game_exit_col;
unsigned char game_exit_lin;

unsigned char game_attrib_osd;
unsigned char game_tileset;
unsigned char game_mode;

unsigned char game_playing;
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

unsigned int tmp_ui;
unsigned char enemies;
unsigned char zx_val_asm;
unsigned char attrib[4];
unsigned char attrib_hl[4];
unsigned char attrib_osd[4];
// TILE ATTRIB TODO REMOVE UNUSED
unsigned char key_attrib[4];

unsigned char *p_state;
unsigned char *p_state_a;



unsigned int curr_time;
unsigned int frame_time;
unsigned int air_time;
unsigned int anim_time;
unsigned char key_last;
unsigned char key_ink;

unsigned char anim_count;

unsigned char spr_count;
unsigned char sprite_curr_index;
// ANIMATIONS
unsigned char anim_lin[GAME_MAX_SPRITES];
unsigned char anim_col[GAME_MAX_SPRITES];
unsigned char anim_tile[GAME_MAX_SPRITES];
unsigned char anim_int[GAME_MAX_SPRITES];
unsigned char anim_end[GAME_MAX_SPRITES];
unsigned char anim_loop[GAME_MAX_SPRITES];

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

unsigned int game_score_top;
unsigned char game_start_scr;

// PHASE RELATED VARIABLES
unsigned char screen_paper;
unsigned char screen_ink;

unsigned char map_border;
unsigned char map_paper;
unsigned char map_paper_clr;
unsigned char map_paper_last;

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
//Sprite KIND
//Base INK for REPLACE
//Variant INK1
//Variant INK2
//Variant INK3



const unsigned char spr_init_len = 8; //Columns on SPR_INIT

unsigned char const spr_init[] = {
    64,   4, 4, E_HORIZONTAL, 0xFF, 0xFF, 0xFF, 0xFF, // ROBOT
    66,   8, 4, E_HORIZONTAL, 0x05, 0x07, 0xFF, 0xFF, // PENGUIN
    68,  12, 4, E_HORIZONTAL, 0x03, 0x02, 0x04, 0xFF, // CHICKEN
    70,  16, 4, E_HORIZONTAL, 0x05, 0x03, 0xFF, 0xFF, // FOCA
    72,  20, 4, E_HORIZONTAL, 0x07, 0x00, 0xFF, 0xFF, // WATER
    74, 108, 1, E_VERTICAL  , 0xFF, 0xFF, 0xFF, 0xFF, // EUGENE
    76,  24, 4, E_HORIZONTAL, 0x46, 0x43, 0x45, 0x06, // PAC
    78,  28, 4, E_HORIZONTAL, 0x05, 0x03, 0x06, 0x02, // KANGOROO
    80,  32, 4, E_HORIZONTAL, 0x45, 0x43, 0x44, 0x42, // BARRELL
    82, 104, 4, E_STATIC    , 0xFF, 0xFF, 0xFF, 0xFF, // KONG
    84,  52, 4, E_VERTICAL  , 0x03, 0x05, 0x04, 0x02, // AMEBATRON
    86,  36, 4, E_HORIZONTAL, 0x04, 0x05, 0x02, 0x06, // CARRITO
    88,  40, 4, E_HORIZONTAL, 0x05, 0x06, 0x02, 0x07, // EWOK
    90,  44, 4, E_VERTICAL  , 0x03, 0x02, 0x04, 0x06, // TELEF
    92,  48, 4, E_HORIZONTAL, 0x06, 0x04, 0x02, 0x04, // WEA
    94,  56, 4, E_HORIZONTAL, 0x03, 0x04, 0x06, 0x02, // WEA 2
    96,  60, 4, E_VERTICAL  , 0xFF, 0xFF, 0xFF, 0xFF, // EYE
    98,  64, 8, E_SKYLAB    , 0x07, 0x04, 0x06, 0xFF, // SKYLAB
   100,  72, 4, E_VERTICAL  , 0x46, 0x47, 0x44, 0x43, // SAFE
   102,  76, 4, E_HORIZONTAL, 0xFF, 0xFF, 0xFF, 0xFF, // BILL
   104,  80, 4, E_HORIZONTAL, 0x45, 0x43, 0x46, 0x44, // MOFETA
   106,  84, 4, E_VERTICAL  , 0x03, 0x06, 0x05, 0x01, // WEA 3
   108,  88, 4, E_HORIZONTAL, 0x02, 0x05, 0xFF, 0xFF, // WEA CON PATAS
   110,  92, 4, E_VERTICAL  , 0x06, 0x04, 0x03, 0x05, // AMEBATRON 2
   112,  96, 4, E_VERTICAL  , 0x22, 0x21, 0x26, 0x24, // PELOTA(PAPER GREEN)
   114, 100, 4, E_HORIZONTAL, 0x26, 0x21, 0x22, 0x27, // WEA SOLAR (PAPER GREEN)
   116, 106, 6, E_FALL      , 0xFF, 0xFF, 0xFF, 0xFF, // KONG FALLING

};


unsigned char spr_init_cin;
unsigned char spr_init_cout;


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

const unsigned char tile_class[] = {
    TILE_EMPTY,     //0
    TILE_FLOOR,     //1
    TILE_CRUMB,     //2
    TILE_WALL,      //3
    TILE_CONVEYOR,  //4
    TILE_DEADLY,    //5
    TILE_DEADLY,    //6
    TILE_EXTRA,     //7 Switch
    TILE_OBJECT,    //8
    TILE_CRUMB,     //9
    TILE_CRUMB,     //10
    TILE_CRUMB,     //11
    TILE_EMPTY,     //12
    TILE_EMPTY,     //13
    TILE_EMPTY,     //14
    TILE_EMPTY,     //15
};
