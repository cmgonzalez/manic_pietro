#include "globals.h"
#include <arch/zx.h>
/* This Data is placed on Bank #6
/ zsdcc can only place code and rodata into different banks at the moment
/ anything else use assembly language and sections
/ Compressed MAP Data for game paging
*/
const unsigned char paper0[] = {
  PAPER_BLACK,
  PAPER_BLUE,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_RED,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLUE,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_GREEN,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLUE,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_WHITE,
  PAPER_BLACK,
  PAPER_CYAN,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLUE,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_BLACK,
  PAPER_GREEN,
  PAPER_BLACK,
};
const unsigned char game_borders0[] = {
  INK_RED,     // 0
  INK_RED,     // 1
  INK_RED,     // 2
  INK_RED,     // 3
  INK_BLUE,    // 4
  INK_RED,     // 5
  INK_GREEN,   // 6
  INK_RED,     // 7
  INK_BLUE,    // 8
  INK_RED,     // 9
  INK_RED,     // 10
  INK_RED,     // 11
  INK_BLUE,    // 12
  INK_YELLOW,  // 13
  INK_RED,     // 14
  INK_RED,     // 15
  INK_RED,     // 16
  INK_BLUE,    // 17
  INK_MAGENTA, // 18
  INK_RED,     // 19
  INK_BLACK,   // 0
  INK_BLUE,    // 1
  INK_BLACK,   // 2
  INK_BLACK,   // 3
  INK_GREEN,   // 4 Kong Jr Watch the game
  INK_MAGENTA, // 5 Dr. Errazking
  INK_CYAN,    // 6
  INK_BLACK,   // 7
  INK_BLACK,   // 8
  INK_BLACK,   // 9
  INK_BLACK,   // 10
  INK_BLACK,   // 11
  INK_BLACK,   // 12
  INK_BLACK,   // 13
  INK_BLACK,   // 14
  INK_BLACK,   // 15
  INK_BLACK,   // 16
  INK_BLACK,   // 17
  INK_BLACK,   // 18
  INK_BLACK,   // 19
  };

const unsigned char map_names0[1312]  = {
      "         Central Cavern         "
      "          The Cold Room         "
      "          The Menagerie         "
      "   Abandoned Uranium Workings   "
      "         Eugene's Lair          "
      "       Processing Plant         "
      "            The Vat             "
      "Miner Willy meets the Kong Beast"
      "        Wacky Amoebatrons       "
      "       The Endorian Forest      "
      "Attack of the Mutant Telephones "
      " Return of the Alien Kong Beast "
      "          Ore Refinery          "
      "       Skylab Landing Bay       "
      "             The Bank           "
      "      The Sixteenth Cavern      "
      "         The Warehouse          "
      "      Amoebatrons Revenge       "
      "     Solar Power Generator      "
      "        The Final Barrier       "
      "       Frozen Central Pipe      "
      "        Super Luizo Bro         "
      "    Pietro meets Monkey Kong    "
      "         Chuntey Crew           "
      "     Kong Jr Watch the Game     "
      "          Dr. Errazking         "
      "         No tengo nombre        "
      "            Stage 27            "
      "            Stage 28            "
      "            Stage 29            "
      "            Stage 30            "
      "            Stage 31            "
      "            Stage 32            "
      "            Stage 33            "
      "            Stage 34            "
      "            Stage 35            "
      "            Stage 36            "
      "            Stage 37            "
      "            Stage 38            "
      "           Noentiendo           "
  };

//Auto Generated
const unsigned int lenght0[] = {
133,172,141,154,162,168,169,212,173,200,203,213,198,159,192,160,241
,193
,194
,122
,147
,182
,144
,312
,258
,205
,165
,100
,100
,100
,100
,100
,100
,88
,100
,100
,100
,100
,100
,100

};
const unsigned char world0[] = {
 3,136,0,13,0,6,132,0,6,140,0,13,0,130,3,143,0,13,142,0,130,3,158,0,130,3,158,0,130,3,150,0,5,13,130,0,5,131,0,130,3,141,1,131,2,1,131,2,138,1,130,3,157,0,13,130,3,131,1,131,0,65,4,150,0,130,3,134,0,7,14,135,0,131,3,0,5,138,0,130,3,132,1,130,0,149,4,131,0,130,3,156,0,130,1,130,3,158,0,130,3,139,0,5,134,0,131,3,133,2,132,1,130,3,0,127,130,0,142,1,138,0,32,0,130,3,158,0,130,3,158,1,3
,3,146,0,142,3,134,0,13,144,0,13,133,0,6,130,3,158,0,130,3,143,0,67,4,131,0,131,2,1,134,0,130,3,144,0,18,141,0,130,3,147,1,136,0,3,130,0,130,3,148,0,132,1,3,130,2,3,130,0,130,3,1,133,2,146,0,3,13,0,3,130,0,130,3,152,0,3,130,2,3,130,0,130,3,130,0,13,133,0,135,1,131,0,132,2,130,0,3,130,2,3,130,0,130,3,152,0,3,130,2,3,130,0,130,3,130,0,4,0,130,4,146,0,3,130,2,3,130,0,130,3,141,0,132,1,0,13,133,0,3,130,2,3,130,0,130,3,0,127,133,0,132,2,143,0,67,4,32,0,130,3,154,0,12,29,130,0,130,3,158,1,3
,3,133,0,13,131,0,6,132,0,13,130,0,7,132,0,13,131,0,6,131,0,130,3,145,0,6,140,0,130,3,158,0,130,3,143,0,68,4,130,0,69,4,137,0,130,3,143,0,1,16,130,0,18,29,137,0,130,3,132,1,154,2,130,3,148,0,13,136,0,13,130,3,134,1,148,0,132,1,130,3,7,157,0,130,3,7,132,0,134,4,147,0,130,3,7,151,0,134,1,130,3,6,140,0,133,1,138,0,32,0,130,3,132,0,134,1,148,0,130,3,0,127,143,0,68,4,0,138,1,130,3,146,0,19,139,0,130,3,158,1,3
,3,13,133,0,6,134,0,147,3,139,0,13,140,0,13,131,0,32,0,130,3,158,0,130,3,146,0,134,1,134,0,130,3,154,0,132,1,130,3,1,133,0,1,137,0,1,141,0,130,3,139,0,130,1,130,0,13,132,0,131,1,134,0,13,130,3,131,2,155,0,130,3,134,0,130,1,145,0,131,1,130,0,130,3,145,0,131,1,138,0,130,3,4,0,4,154,0,1,130,3,139,0,131,1,135,0,131,1,134,0,130,3,133,0,130,1,143,0,6,132,0,131,1,130,3,71,4,133,0,71,4,136,0,130,1,137,0,127,0,130,3,1,10,133,0,6,16,149,0,130,3,158,1,3
,3,142,0,74,12,142,0,130,3,143,0,88,141,0,13,130,3,158,0,130,3,127,136,0,72,5,147,0,130,3,137,0,1,12,140,0,5,134,0,130,3,141,1,132,0,132,2,134,1,131,0,130,3,137,0,13,146,0,130,1,130,3,131,0,73,5,143,0,5,135,0,13,0,130,3,131,0,4,12,140,0,138,4,131,0,130,3,131,0,138,1,145,0,130,3,158,0,130,3,130,2,139,1,132,0,135,1,133,0,1,130,3,134,0,13,3,13,149,0,130,3,130,1,133,0,3,133,0,3,32,0,3,141,0,130,3,132,0,5,130,0,3,133,0,3,130,0,135,3,130,5,133,0,130,3,135,1,144,3,135,1,3
,3,156,0,32,0,130,3,158,0,130,3,142,0,127,143,0,130,3,148,0,6,137,0,130,3,135,0,131,1,132,0,130,1,132,0,133,1,133,0,130,3,130,0,130,1,138,0,13,3,13,138,0,131,1,130,3,143,0,3,141,0,13,130,3,134,0,77,5,134,0,77,5,134,0,133,1,131,0,130,3,130,1,132,0,6,13,134,0,14,21,142,0,130,3,13,133,0,137,1,3,137,1,133,0,130,3,140,0,13,130,0,3,6,141,0,130,3,143,0,3,142,0,130,3,130,0,5,152,0,131,1,130,3,130,0,132,4,131,0,76,5,138,0,130,1,130,0,76,5,131,0,130,3,137,0,7,20,142,0,24,29,131,0,130,3,158,1,3
,3,141,0,147,3,144,0,79,4,140,0,130,3,144,0,15,29,140,0,130,3,142,0,130,1,3,140,2,13,130,3,144,0,3,141,2,130,3,134,0,133,4,130,0,131,1,3,138,2,6,130,2,130,3,131,1,141,0,3,130,2,13,138,2,130,3,144,0,3,137,2,13,131,2,130,3,1,135,0,78,4,134,0,3,133,2,6,135,2,130,3,136,0,2,10,131,0,131,1,3,141,2,130,3,139,1,133,0,3,2,13,136,2,6,130,2,130,3,144,0,3,140,2,13,130,3,141,0,132,3,133,2,6,135,2,130,3,127,136,0,130,1,130,0,3,32,131,0,79,4,138,0,130,3,141,0,3,132,0,17,29,138,0,130,3,141,1,146,3
,3,0,6,130,0,7,132,0,6,132,0,82,13,3,7,0,3,138,0,130,3,144,0,3,130,0,3,138,0,130,3,140,0,13,0,130,1,3,141,0,130,3,144,0,3,141,0,130,3,144,0,3,141,0,130,3,131,1,133,0,134,1,130,0,3,130,1,139,0,130,3,141,0,13,130,0,3,131,0,132,1,133,0,1,130,3,0,131,1,140,0,3,0,80,5,134,0,1,131,0,130,3,0,13,133,0,131,1,134,0,3,0,18,21,138,0,130,3,144,0,3,133,1,136,0,130,3,1,138,0,131,1,130,0,3,137,0,132,1,130,3,136,0,130,1,81,7,132,0,3,141,0,130,3,131,0,130,1,133,0,12,15,132,0,3,132,0,133,1,132,0,130,3,0,127,132,0,80,5,130,0,131,4,3,32,0,3,130,1,137,0,13,0,130,3,134,0,1,9,133,0,3,130,0,3,141,0,130,3,158,1,3
,3,32,0,3,155,0,130,3,143,0,13,142,0,130,3,158,0,130,3,132,0,84,9,136,0,86,5,131,0,84,9,137,0,130,3,133,0,96,131,0,84,5,131,0,12,18,132,0,96,131,0,84,5,132,0,130,3,132,1,130,0,131,1,0,96,136,1,130,0,131,1,0,96,130,1,130,0,130,3,158,0,130,3,156,0,130,1,130,3,130,0,130,1,130,0,131,1,130,0,136,4,139,0,130,3,149,0,131,1,130,0,130,1,130,0,130,3,130,1,142,0,87,7,140,0,130,3,144,0,12,18,140,0,130,3,130,0,130,1,130,0,131,1,130,0,136,1,130,0,131,1,130,0,130,1,130,0,130,3,127,155,0,130,1,130,3,158,0,130,3,158,1,3
,3,138,0,6,0,131,1,3,0,6,0,6,138,1,130,3,141,0,13,0,3,132,0,6,136,0,13,130,3,134,1,137,0,3,132,0,13,133,0,132,1,130,3,130,0,6,140,0,3,142,0,130,3,127,142,0,3,132,1,138,0,130,3,135,0,1,135,2,3,131,0,89,5,0,136,1,130,3,132,1,135,0,13,131,0,3,131,0,17,21,137,0,130,3,138,0,89,5,131,0,3,135,1,131,2,132,0,130,3,133,1,133,0,9,14,131,0,3,0,13,140,0,130,3,6,135,0,135,1,3,140,0,130,1,130,3,132,1,130,2,134,0,89,8,0,3,135,1,134,0,6,130,3,140,0,8,14,0,3,134,0,6,131,2,132,0,130,3,135,0,138,7,141,0,130,3,131,1,135,0,89,5,32,142,0,131,1,130,3,138,0,4,16,146,0,130,3,158,7,3
,135,3,140,0,6,132,0,13,134,0,130,3,32,0,127,143,0,5,138,0,13,130,3,139,0,90,6,145,0,130,3,132,1,135,0,8,56,130,0,93,4,141,0,130,3,13,142,0,15,24,136,0,90,4,131,0,130,3,130,0,90,8,134,1,132,0,130,1,135,7,130,1,0,96,131,0,130,3,130,0,32,96,142,0,13,132,0,6,132,0,130,1,130,3,141,0,93,10,133,0,90,6,0,6,134,0,130,3,130,1,130,0,130,4,135,0,14,18,133,0,56,96,0,6,132,0,130,1,130,3,138,0,137,1,132,0,5,134,0,130,3,138,0,6,135,0,6,136,0,1,130,0,130,3,132,0,132,2,130,0,6,135,0,5,139,0,130,3,138,0,5,144,0,131,1,130,3,130,1,138,0,93,4,136,0,131,1,132,0,13,130,3,140,0,5,19,144,0,130,3,158,1,3
,3,0,6,131,0,7,132,0,6,131,0,82,13,3,7,130,0,3,137,0,130,3,158,0,130,3,142,0,130,2,142,0,130,3,142,0,13,143,0,130,3,158,0,130,3,131,1,133,0,133,2,3,130,0,3,134,2,130,1,13,132,0,130,3,0,13,139,0,3,130,0,3,136,0,81,5,130,0,1,130,3,133,0,130,1,134,0,3,0,13,3,136,0,24,28,131,0,130,3,130,0,1,138,0,3,130,0,3,135,0,134,1,130,3,137,0,132,1,3,130,0,3,141,0,130,3,133,0,1,138,0,3,131,2,138,0,130,3,140,0,81,7,130,0,3,135,0,130,1,132,0,130,3,134,1,134,0,10,15,130,0,3,133,0,5,132,0,5,130,0,130,3,0,127,134,0,81,5,131,1,3,32,0,3,4,0,137,4,13,0,130,3,136,0,1,9,131,0,3,130,0,3,141,0,130,3,140,1,132,3,142,1,3
,161,3,130,0,7,131,0,94,4,150,0,130,3,130,0,7,0,96,5,7,29,150,0,130,3,130,0,7,0,8,96,146,1,130,0,132,1,130,3,130,0,7,139,0,94,10,142,0,130,3,130,0,7,139,0,7,29,142,0,130,3,130,0,7,131,0,1,0,13,133,1,130,0,133,1,130,0,132,1,130,0,1,130,3,130,0,7,146,0,94,4,135,0,130,3,130,0,7,146,0,7,29,135,0,130,3,130,0,7,131,0,133,1,130,0,131,1,130,0,13,133,1,0,13,132,1,130,3,130,0,7,141,0,94,10,140,0,130,3,130,0,7,141,0,7,29,140,0,130,3,130,0,7,131,0,131,1,130,0,131,1,130,0,132,1,130,0,132,1,130,0,130,1,130,3,32,0,7,153,0,127,0,130,3,130,0,7,155,0,130,3,130,1,154,4,130,1,3
,3,98,4,136,0,98,4,130,0,32,133,0,98,4,136,0,130,3,0,96,137,0,96,137,0,96,136,0,130,3,150,0,13,135,0,130,3,158,0,130,3,158,0,130,3,142,0,1,7,142,0,130,3,130,0,1,7,134,0,1,7,134,0,1,7,134,0,1,7,130,0,130,3,134,0,1,7,135,0,13,134,0,1,7,130,0,13,131,0,130,3,130,0,13,155,0,130,3,132,0,1,7,134,0,1,7,134,0,1,7,134,0,1,7,130,3,158,0,130,3,1,7,134,0,1,7,132,0,4,0,132,4,132,0,1,7,132,0,130,3,158,0,130,3,134,0,1,7,146,0,13,131,0,130,3,158,0,161,3
,3,133,0,155,3,156,0,130,7,130,3,152,0,13,131,0,130,7,130,3,32,134,0,144,4,133,1,130,7,130,3,135,0,6,147,0,6,130,7,130,3,133,1,130,0,5,100,5,145,0,6,130,7,130,3,136,0,32,96,0,13,139,0,130,1,130,0,6,130,7,130,3,134,0,2,132,0,130,1,142,0,6,130,7,130,3,130,0,130,1,138,0,100,8,0,130,1,136,0,6,130,7,130,3,142,0,32,96,136,0,130,1,0,6,130,7,130,3,130,1,137,0,130,1,135,0,100,4,133,0,6,130,7,130,3,145,0,130,1,0,32,96,133,0,5,130,7,130,3,132,0,131,1,143,0,130,1,132,0,130,7,130,3,0,127,137,0,130,1,132,0,102,5,137,0,130,7,130,3,145,0,17,19,134,0,13,130,0,130,7,161,3
,3,13,157,0,130,3,158,0,130,3,157,0,13,130,3,158,0,130,3,158,0,130,3,1,132,0,1,132,0,3,32,0,3,135,0,131,1,0,105,8,131,0,130,3,138,0,3,130,0,130,3,138,0,25,29,131,0,130,3,131,0,1,134,0,3,0,13,131,3,0,105,4,133,0,134,1,130,3,138,0,3,130,0,132,3,18,23,139,0,130,3,130,2,152,4,132,0,130,3,105,4,141,0,13,142,0,130,3,1,7,135,0,130,3,130,1,140,0,1,132,0,130,3,137,1,149,0,130,3,0,127,134,0,105,4,138,0,1,132,0,1,132,0,130,3,136,0,1,19,140,0,131,5,133,0,130,3,158,1,3
,3,156,0,132,3,156,0,32,0,130,3,154,0,106,3,130,0,130,3,127,153,0,8,88,130,0,130,3,133,0,5,130,0,5,131,0,5,130,0,5,133,0,5,0,5,134,0,130,3,130,1,135,2,130,0,135,2,130,0,131,2,13,130,2,130,0,130,1,130,3,130,2,6,134,2,107,6,135,2,130,0,134,2,130,0,130,2,130,3,137,2,8,88,131,2,13,131,2,106,10,133,2,6,130,0,130,2,130,3,130,2,130,0,133,2,130,0,130,2,4,0,131,4,8,64,134,2,130,0,130,2,130,3,13,2,106,13,133,2,130,0,131,2,1,131,2,130,0,134,2,130,0,130,2,130,3,130,2,64,88,133,2,130,0,135,2,13,130,2,6,132,2,130,0,130,2,130,3,130,2,130,0,132,2,6,130,0,142,2,13,130,0,130,2,130,3,130,2,130,0,133,2,130,0,143,2,130,0,130,2,130,3,133,0,109,10,133,0,109,5,144,0,130,3,133,0,5,13,133,0,12,25,140,0,132,1,130,3,158,1,3
,3,155,0,3,32,0,130,3,143,0,13,142,0,130,3,137,0,110,7,141,0,110,11,132,0,130,3,132,0,110,5,132,0,96,0,87,11,133,0,110,4,132,0,96,132,0,130,3,133,0,96,134,0,12,18,134,0,96,137,0,130,3,130,0,130,1,130,0,131,1,130,0,136,1,130,0,131,1,130,0,132,1,130,3,144,0,87,5,140,0,130,3,130,1,142,0,12,18,140,0,130,3,139,0,136,1,130,0,131,1,130,0,130,1,130,0,130,3,130,0,130,1,130,0,131,1,149,0,130,3,144,0,87,10,138,0,130,1,130,3,144,0,12,18,140,0,130,3,130,0,130,1,130,0,131,1,130,0,136,1,130,0,131,1,130,0,130,1,130,0,130,3,130,1,142,0,86,5,138,0,127,0,130,3,144,0,12,18,140,0,3,160,1
,131,3,148,0,16,135,0,130,3,32,149,0,16,134,0,13,130,3,150,0,16,135,0,130,3,150,0,16,0,115,5,132,0,130,3,150,0,16,0,24,29,132,0,130,3,13,0,130,1,132,0,134,1,136,0,16,135,1,130,3,150,0,16,132,0,115,5,0,130,3,138,0,113,5,134,0,131,1,0,16,132,0,24,29,0,130,3,130,1,130,0,112,4,132,0,48,80,131,1,135,0,16,135,1,130,3,132,0,8,88,144,0,16,133,0,114,5,130,3,141,0,127,0,113,6,0,130,1,130,0,16,133,0,24,29,130,3,132,1,139,0,8,80,133,0,16,135,1,130,3,134,0,132,4,131,0,133,1,132,0,16,134,0,13,130,3,143,0,115,5,133,0,16,135,0,132,3,141,0,14,29,133,0,16,135,0,132,3,148,1,3,135,1,3
,158,3,10,159,3,10,159,3,10,159,3,10,159,3,10,148,3,32,0,3,0,13,135,0,148,3,130,0,3,130,0,96,6,132,0,13,148,3,130,0,3,130,0,48,96,133,0,130,3,156,0,130,1,130,3,158,0,130,3,4,0,148,4,131,0,2,132,0,130,3,136,0,5,13,0,5,0,13,130,0,5,0,13,0,5,134,0,1,130,0,130,3,130,1,156,0,130,3,132,0,130,1,121,4,145,0,127,132,0,130,3,134,0,7,22,150,0,130,3,158,1,3
,161,3,137,16,29,138,16,29,137,16,130,3,132,0,13,132,0,6,138,0,6,132,0,13,132,0,130,3,0,22,7,137,0,13,132,0,13,137,0,8,23,0,130,3,20,19,154,0,18,20,130,3,131,0,5,150,0,5,131,0,130,3,136,2,142,0,136,2,130,3,0,6,135,0,140,4,135,0,6,0,130,3,138,0,6,136,17,6,138,0,130,3,134,1,146,0,134,1,130,3,158,0,130,3,138,1,132,0,32,133,0,138,1,130,3,137,16,140,0,137,16,130,3,130,20,7,0,127,137,0,65,4,139,0,8,130,20,130,3,142,0,4,26,142,0,161,3
,130,0,23,147,0,16,17,138,0,23,132,0,13,135,0,16,17,133,0,18,19,134,0,16,17,130,0,23,140,0,18,19,137,0,16,17,130,0,18,19,130,0,23,136,0,67,4,141,0,18,19,134,0,23,136,0,8,15,149,0,23,0,131,1,4,0,136,4,131,1,132,0,5,131,0,13,133,0,23,149,0,3,137,0,23,140,0,13,136,0,22,131,0,6,133,0,23,131,0,67,4,143,0,136,1,131,0,23,131,0,5,12,153,0,1,24,139,1,131,0,132,1,168,0,32,151,0,3,131,0,3,136,0,127,135,0,24,134,2,24,131,0,22,0,5,0,22,155,0,22,0,3,0,22,134,0,20,21,20,21,20,21,20,21,20,21,136,29,20,21,20,21,20,21,20,21,20,21,20,21,20,21
,137,0,13,153,0,82,5,131,0,1,7,1,0,32,134,0,13,134,0,13,142,0,7,139,0,81,3,139,0,142,1,3,132,0,17,27,141,0,134,16,7,134,16,139,1,7,1,134,0,13,151,0,7,138,0,23,130,0,7,132,0,3,142,1,132,0,1,7,139,1,133,16,7,135,16,135,0,7,132,0,81,4,146,0,13,134,0,7,132,0,4,15,141,0,81,5,137,0,142,1,3,131,0,7,130,0,21,28,139,0,141,16,139,1,7,1,158,0,7,134,0,127,143,0,23,136,0,7,137,0,5,136,0,3,142,1,132,0,141,1,141,16,132,0
,22,23,22,23,22,23,22,1,22,23,22,23,22,23,22,23,22,23,22,23,22,1,22,23,22,23,22,23,22,23,22,23,3,17,130,0,13,130,0,6,130,0,13,130,0,6,135,0,6,133,0,6,130,0,18,24,3,17,144,0,13,139,0,18,24,3,17,132,0,71,4,150,0,18,24,3,17,132,0,5,13,141,0,71,4,0,32,133,0,18,24,22,23,130,0,22,23,22,23,22,23,1,22,23,22,23,134,0,19,26,135,0,22,23,3,17,0,13,134,0,20,135,0,1,22,23,22,23,22,23,22,23,22,130,0,18,24,3,17,136,0,20,16,0,7,0,130,16,0,7,139,0,18,24,3,17,136,0,20,16,0,7,0,130,16,0,7,133,0,13,133,0,18,24,3,17,0,130,2,131,0,22,23,1,23,22,1,22,23,22,23,1,139,0,18,24,22,23,139,0,7,132,0,7,133,0,5,133,0,22,23,3,17,132,0,13,134,0,7,132,0,7,130,0,135,4,130,0,18,24,3,17,132,0,73,6,133,0,7,132,0,7,139,0,18,24,3,17,132,0,2,11,133,0,7,0,127,130,0,7,137,0,130,2,18,24,3,17,138,16,0,7,132,0,7,0,138,16,18,24,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23,22,23
,3,136,0,17,130,16,17,130,16,18,143,0,130,3,118,10,134,0,17,132,16,130,18,143,0,130,3,142,0,1,20,21,22,140,0,130,3,130,1,137,0,17,130,16,18,130,0,19,130,1,20,21,22,134,0,17,130,3,130,7,144,0,7,5,130,0,19,25,1,20,21,22,130,16,130,3,134,0,13,139,0,7,132,0,7,5,130,1,130,25,16,130,3,143,0,78,3,134,0,7,13,130,0,7,8,17,130,3,32,142,0,3,22,134,0,7,131,0,7,8,24,130,3,138,0,77,3,139,0,12,131,0,7,8,0,130,3,130,1,18,135,0,11,26,143,0,7,8,0,130,3,16,18,135,2,133,1,25,131,1,25,136,1,7,8,0,130,3,18,130,0,7,138,0,7,131,0,7,136,0,7,8,17,130,3,131,0,7,135,0,13,130,0,7,131,0,7,134,0,13,0,7,8,17,130,3,0,127,0,12,138,0,12,76,3,0,12,136,0,130,12,17,130,3,16,142,0,2,26,141,0,130,3,131,1,130,23,1,23,1,23,1,23,130,1,23,131,1,23,131,1,23,130,1,23,133,1,3
,3,137,16,27,138,0,27,137,16,3,23,137,16,27,127,135,0,32,0,27,137,16,23,7,137,16,27,138,0,27,137,16,7,8,137,16,18,26,25,134,0,24,26,19,137,16,8,3,139,16,27,131,0,21,22,0,27,139,16,3,23,139,16,27,134,0,27,139,16,23,7,134,16,24,132,26,19,134,0,18,132,26,25,134,16,7,8,134,16,27,130,0,13,141,0,27,134,16,8,3,134,16,27,134,0,1,20,136,0,27,134,16,3,23,134,16,27,131,0,5,136,0,6,131,0,27,134,16,23,7,134,16,27,131,0,7,135,0,21,22,131,0,27,134,16,7,8,134,16,27,131,0,8,140,0,27,134,16,8,3,134,16,27,144,0,27,134,16,3,23,134,16,27,133,0,13,133,0,13,132,0,27,134,16,23,7,134,16,27,144,0,27,134,16,7,8,134,16,18,144,26,19,134,16,8
,158,0,32,234,0,13,132,0,127,154,0,3,137,0,132,1,146,0,27,0,24,136,25,130,17,18,145,0,132,1,134,0,130,17,18,17,18,0,13,143,0,132,17,134,5,130,17,18,17,18,0,3,143,0,132,17,134,6,130,17,18,17,18,0,27,142,0,130,1,7,130,17,134,6,130,17,18,17,18,0,27,142,0,130,17,18,130,17,134,6,130,17,18,17,18,0,27,139,0,13,130,0,130,17,18,130,17,134,6,130,17,18,17,18,0,27,139,0,3,130,0,130,17,18,130,17,134,6,130,17,18,3,18,0,27,136,0,3,130,0,27,130,0,130,17,18,130,17,134,6,130,17,18,27,18,0,27,135,0,140,20,134,6,142,20
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
,3,0,1,2,3,4,5,6,7,150,0,130,3,8,9,10,11,12,13,14,15,150,0,130,3,16,17,18,19,20,21,22,23,150,0,130,3,24,25,26,27,28,29,30,13,150,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,158,0,130,3,127,141,0,32,143,0,130,3,158,0,130,3,158,1,3
};
