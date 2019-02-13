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
#ifndef GLOBALS_H
#define GLOBALS_H
#include <input.h>

#define GAME_SCR_MAX_INDEX           512 // 32 * 16

#define GAME_MAX_OBJECTS             5 //OBJECTS OR COINS
#define GAME_MAX_SPRITES             9 //MAX OF OBJECTS (ENEMIES + PLAYERS)
#define GAME_MAX_ENEMIES             8 //GAME_MAX_SPRITES - PLAYERS
/* Player indexes */
#define INDEX_P1                     8 //Index of P1 == GAME_MAX_PRITES - 1

//Nirvana Related
#define NIRV_SPRITE_P1               5 //Sprite to be used by P1 (Have transparency)
#define NIRV_SPRITE_DOOR             6 //Sprite to be used by P1 (Have transparency)
#define NIRV_TOTAL_SPRITES           7 //Maximun of sprites to handle (Nirvana == 8) (CUSTOM Nirvana == 7)

#define GAME_GRAVITY                 1
#define GAME_VELOCITY               -6
#define GAME_START_LIVES             3
#define GAME_COLLISION_TIME         30 //TIME BTWN COLISION CHECKS
#define GAME_CODELEN                 8 //LEN OF START CODES

#define PLAYER_SPEED                 4
#define PLAYER_SPEED_VJUMP           10
#define PLAYER_FALL_DEADLY           40
#define ENEMY_START_INDEX            64 //INDEX OF START OF ENEMY ON MAP ARRAY




//TILES CLASSES
#define TILE_EMPTY                   0  //BACKGROUND
#define TILE_FLOOR                   1  //PLATFORMS
#define TILE_CRUMB                   2  //PLATFORMS CRUMBLING
#define TILE_WALL                    3  //SOLID WALL
#define TILE_CONVEYOR                4  //CONVEYORS
#define TILE_DEADLY                  5  //DEADLY
#define TILE_EXTRA                   6  //EXTRA SWITCH
#define TILE_OBJECT                  8  //KEY

#define TILE_EXTRA_OFF               8 //EXTRA SWITCH OFF


#define TILE_CRUMB_INIT              2 //TODO MOVE ON BTILE AND PLACE ADJACENT TO CRUMBLING TILES



#define SPRITE_EXIT                  32
#define SPRITE_EMPTY                 94
#define SPRITE_TMP                   4608 //96 * 48 //4320 //90*48
#define SPRITE_DOOR                  7


#define TILE_BACKGROUND_S             0  //START OF BACKGROUND
#define TILE_BACKGROUND_E             35 //END OF BACKGROUND


#define TILE_ITEM_S                   36 //START OF ITEMS
#define TILE_ITEM_E                   47 //END OF ITEMS


#define TILE_CEIL                     72 //START CEILS TILES

#define TILE_END                      96 //END OF TILES



#define TILE_HIT                      69 //UP FROM THIS CAN BE HIT BY PLAYER JUMP
#define TILE_STOPPER                  35 //TURN ENEMY DIRECTION






#define GAME_TOTAL_CLASSES            6 //Total class of enemies
#define GAME_TOTAL_INDEX_CLASSES      30 //Total class of enemies variations, the same enemy can be left/right etc...
#define GAME_SPR_INIT_SIZE            9

#define E_HORIZONTAL                  1
#define E_VERTICAL                    2
#define E_WALK                        3
#define E_STATIC                      4
#define E_GHOST                       5
#define E_SKYLAB                      6
#define E_FALL                        7
#define E_ZIGZAG                      8
#define E_EUGENE                      9



#define TILE_TITLE                    204
#define TILE_ANIM                     128

#define GAME_ENEMY_MAX_CLASSES        30
#define GAME_ENEMY_CLASS_LEN          9




//TILE MAP'S
//  0..23   BACKGROUND
// 24..31   OBJECTS (CAN BE PICKED)
// 32..39   BACKGROUND DEADLY
// 40..47   PLATFORMS (FLOOR ONLY)
// 48..51   SPECIAL PLATFORMS (STAIRS)
// 52..55   BRICKS (FLOOR AND CEIL)
// 56..63   SPECIAL BRICKS (FLORR AND CEIL CAN BE HITTED)

/* Player 1 tiles */
#define TILE_P1_RIGHT                 8
#define TILE_P1_LEN                   4 //LEN OF PLAYER SPRITES FOR EACH DIRECTION
#define FRAMES_PLAYER                 4

/* Screen variables */
#define SPR_COLINT                    4   /* Internal colum increment */
#define SCR_COLI                      1   /* Start column */
#define SCR_COLS                      32  /* Columns max */
#define SCR_COLS_M                    31  /* Columns max */
#define SCR_ROWI                      0   /* Start row */
#define SCR_ROWS                      23  /* Rows max */
#define SCR_LINS                      184 /* Lins max */
#define GAME_LIN_FLOOR                128
#define GAME_LIN_FLOOR_NIRV           144 /* GAME_LIN_FLOOR + 16 */
#define GAME_LIN_TOP_PLATFORM         48
#define GAME_OFFSET_Y                 16

#define ST_STAND_R                    0
#define ST_STAND_L                    1
#define ST_TURN_R                     10
#define ST_TURN_L                     11

//Starting Directions
#define DIR_NONE                      0
#define DIR_LEFT                      1
#define DIR_RIGHT                     2
#define DIR_UP                        3
#define DIR_DOWN                      4


/*Stats (bit position number)*/
#define STAT_JUMP                     0
#define STAT_FALL                     1
#define STAT_DIRR                     2
#define STAT_DIRL                     3
#define STAT_CONVEYOR                 4
#define STAT_KILLED                   5
#define STAT_DIRU                     6
#define STAT_DIRD                     7

/*Stats alt (bit position number)*/
#define STAT_LOCK                     0
#define STAT_CONVJMP                  1
#define STAT_VJUMP                    2
#define STAT_LDIRL                    3
#define STAT_LDIRR                    4
#define STAT_CONVLOCK                 6
#define STAT_NEWJUMP                  7
/* General */
#define SPRITE_LIN_INC                2 //USED ONLY ONCE CHECK

/* Player */


/* Enemies */
#define ENEMIES_MAX                   5   //MAX QUANTITY OF ENEMIES ON SCREEN (0->6)
#define ENEMIES_MAXJUMP               12 //MAX JUMP WHEN HIT
#define ENEMY_JUMP_SPEED              1
#define ENEMY_FALL_SPEED              1
#define ENEMY_KILLED_SPEED            8


// Game timers
#define TIME_EVENT                    100
#define TIME_ANIM                     8


// Sound Mode Variables
#define GAME_SOUND_48_FX_ON          0x01
#define GAME_SOUND_48_FX_OFF         0xfe
#define GAME_SOUND_48_MUS_ON         0x02
#define GAME_SOUND_48_MUS_OFF        0xfd
#define GAME_SOUND_AY_FX_ON          0x04
#define GAME_SOUND_AY_FX_OFF         0xfb
#define GAME_SOUND_AY_MUS_ON         0x08
#define GAME_SOUND_AY_MUS_OFF        0xf7


//typedef uint16_t (*JOYFUNC)(udk_t *);
#endif


extern unsigned char spec128;
//General Use VARIABLES
extern unsigned char i;
extern unsigned char v0;
extern unsigned char v1;
extern unsigned char v2;
extern unsigned char v3;

extern unsigned char btiles[];
extern uint16_t (*joyfunc1)(udk_t *); //TODO REMOVE THIS AS IS PART NOW OF input.h
//extern uint16_t (*joyfunc2)(udk_t *); //TODO REMOVE THIS AS IS PART NOW OF input.h
extern udk_t k1;


extern unsigned char dirs;
extern unsigned char dirs_last;

extern unsigned char class[GAME_MAX_SPRITES];
extern unsigned char state[GAME_MAX_SPRITES];
extern unsigned char state_a[GAME_MAX_SPRITES];
extern unsigned char value_a[GAME_MAX_SPRITES];
extern unsigned char value_b[GAME_MAX_SPRITES];
extern unsigned char value_c[GAME_MAX_SPRITES];

extern unsigned char tile[GAME_MAX_SPRITES];
extern unsigned char lin[GAME_MAX_SPRITES];
extern unsigned char col[GAME_MAX_SPRITES];
extern unsigned char colint[GAME_MAX_SPRITES];

extern unsigned int last_time[GAME_MAX_SPRITES];
extern unsigned int last_time_a[GAME_MAX_SPRITES];
extern unsigned int last_time_b[GAME_MAX_SPRITES];

extern unsigned char obj_lin[GAME_MAX_OBJECTS]; // object lin for HIGHLIGHT
extern unsigned char obj_col[GAME_MAX_OBJECTS]; // object col for HIGHLIGHT
extern unsigned char obj_count;

extern unsigned char tbuffer[7];

extern uint64_t player_score;
extern signed int  player_vel_y;
extern signed int  player_vel_y0;
extern signed char  player_vel_inc;
extern unsigned char player_jump_hack;
extern unsigned char player_jump_count;
extern unsigned char player_jump_lin;
extern const unsigned char player_jump_hor[];
extern const unsigned char game_encode[];


extern unsigned char player_fall_start;

//PLAYER ATTRIBUTES




extern unsigned char player_col_scr;
extern unsigned char player_lin_scr;



extern signed   int  game_gravity;
extern unsigned char game_song_play;
extern unsigned char game_song_play_start;
extern unsigned char game_conveyor_dir;
extern unsigned char game_conveyor_lin;
extern unsigned char game_conveyor_col0;
extern unsigned char game_conveyor_col1;
extern unsigned char game_exit_col;
extern unsigned char game_exit_lin;
extern unsigned char game_code;
extern unsigned char game_lives;

extern unsigned char game_round_up;
extern unsigned char menu_curr_sel;
extern unsigned char game_tileset;
extern unsigned char game_mode;
extern unsigned char game_atrac;


extern unsigned int fps;



extern unsigned char sprite;
extern unsigned char game_debug;
extern unsigned char game_fps_show;

extern unsigned char player_jump_top;

extern unsigned int  player_kill_index;
extern unsigned char nirv_sprite_index;


extern unsigned char s_lin0;
extern unsigned char s_lin1;
extern unsigned char s_col0;
extern unsigned char s_col1;
extern unsigned char s_row1;
extern unsigned int loop_count;
extern unsigned int index0;
extern unsigned int index1;
extern unsigned char zx_val_asm;
extern unsigned char attrib[4];
extern unsigned char attrib_hl[4];
extern unsigned char attrib_osd[4];
extern unsigned char attrib_sol0[4];
extern unsigned char attrib_sol1[4];
extern unsigned char g_ray1;



//TILE ATTRIB
extern unsigned char key_attrib[4];


//TODO REVIEW 128 bytes! needs loop on final target without udg's

extern unsigned int curr_time;
extern unsigned int frame_time;

extern unsigned int time_air;
extern unsigned int time_conv;
extern unsigned int time_key;
extern unsigned char lapse_crumb;

extern unsigned char key_last;
extern unsigned char key_ink;

extern unsigned char spr_count;


extern unsigned char game_inmune;
extern unsigned char game_inf_lives;
extern unsigned char game_sound;
extern unsigned char game_tile_cnt;

extern unsigned char game_over;
extern unsigned char game_crumb_start;
extern unsigned char game_crumb_end;

extern unsigned char player_lives;
extern unsigned char player_coins;
extern uint64_t game_score_top;
extern uint64_t player_next_extra;
extern unsigned char game_start_scr;


extern unsigned char screen_paper, screen_ink;
extern const unsigned char spr_init[];


extern unsigned char spr_tile[];
extern unsigned char spr_speed[];
extern unsigned char spr_speed_b[];
extern unsigned char spr_speed_a[];
extern unsigned char spr_frames[];
extern unsigned char spr_altset[];
extern unsigned char spr_kind[];
extern unsigned char scr_map[];


//extern unsigned char scr_obj0[];
//extern unsigned char scr_obj1[];



//Bank 6 Level Data
extern const unsigned char world0[];
extern const unsigned int lenght0[];
extern const unsigned char paper0[];
extern const unsigned char tile_class1[];
extern const unsigned char tile_class2[];
extern const unsigned char spr_init1[];
extern const unsigned char spr_init2[];

extern const unsigned char game_borders0[];
extern const unsigned char map_names0[];

extern unsigned char scr_curr;
extern unsigned char *gbyte;



extern unsigned char map_border;
extern unsigned char map_paper;
extern unsigned char map_clear;

extern const unsigned char key_map[];
extern const unsigned int scan_map[];

extern const unsigned char tile_class[];
//extern unsigned char *map_names[];
//extern const unsigned char map_lens[];

extern unsigned char spr_init_cin0;
extern unsigned char spr_init_cout0;

extern unsigned char spr_init_cin1;
extern unsigned char spr_init_cout1;


extern unsigned char spr_init_cin2;
extern unsigned char spr_init_cout2;
extern unsigned char spr_init_cin3;
extern unsigned char spr_init_cout3;
extern unsigned char spr_init_bright;

extern unsigned char spr_clr;

extern unsigned char *air_curr_byte;
extern const unsigned int air_start_byte;
extern const unsigned int air_end_byte;

extern unsigned char *attribs;
extern unsigned char *deltas;

extern unsigned char code0[GAME_CODELEN];
extern unsigned char code1[GAME_CODELEN];
