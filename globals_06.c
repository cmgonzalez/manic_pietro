#include "globals.h"
#include <arch/zx.h>
#include "globals_06_map.h"
/* This Data is placed on Bank #6
/ zsdcc can only place code and rodata into different banks at the moment
/ anything else use assembly language and sections
/ Compressed MAP Data for game paging
*/
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
  INK_BLACK,   // 20
  INK_BLUE,    // 21
  INK_BLACK,   // 22
  INK_BLUE,    // 23
  INK_GREEN,   // 24 Kong Jr Watch the game
  INK_MAGENTA, // 25 Dr. Errazking
  INK_CYAN,    // 26
  INK_MAGENTA, // 27
  INK_RED,     // 28
  INK_BLACK,   // 29
  INK_BLUE,    // 30
  INK_BLACK,   // 31
  INK_BLACK,   // 32
  INK_BLACK,   // 33
  INK_WHITE,   // 34
  INK_BLACK,   // 35
  INK_BLACK,   // 36
  INK_WHITE,   // 37
  INK_BLUE,   // 38
  INK_BLACK,   // 39
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
      "          Chutney Crew          "
      "     Kong Jr Watch the Alba     "
      "          Dr. Errazking         "
      "         Super Oki Doki         "
      "         Darius the Kid         "
      " Senior Mutant Alcoholic Turtles"
      "          Felipe Man            "
      "        Beyker The Sonico       "
      "           Fede Simbel          "
      "            Work Over           "
      "          Juanjo Egg's          "
      "    Chuck Norris Karate Champ   "
      "            Ale Pack            "
      "        Nodes of Carlod         "
      "     Chilean Corner Spider's    "
      "      Einar is Good Enough      "
      "       Castle Noentiendo!       "  };

const unsigned char tile_class1[] = {
    TILE_EMPTY,     //00
    TILE_FLOOR,     //01
    TILE_CRUMB,     //02
    TILE_WALL,      //03
    TILE_CONVEYOR,  //04
    TILE_DEADLY,    //05
    TILE_DEADLY,    //06
    TILE_EXTRA,     //07 Switch

    TILE_FLOOR,     //08 Switch Off

    TILE_CRUMB,     //09 Crumb1
    TILE_CRUMB,     //10 Crumb2
    TILE_CRUMB,     //11 Crumb3

    TILE_FLOOR,     //12 Misc Platform
    TILE_OBJECT,    //13 Object
    TILE_EMPTY,     //14 Reverved Exit
    TILE_EMPTY,     //15 Reverved Exit

    TILE_EMPTY,     //16 Backgound
    TILE_EMPTY,     //17 Backgound
    TILE_EMPTY,     //18 Backgound
    TILE_EMPTY,     //19 Backgound

    TILE_CRUMB,     //20 Platform
    TILE_CRUMB,     //21 Platform
    TILE_CRUMB,     //22 Platform
    TILE_CRUMB,     //23 Platform

    TILE_CRUMB,      //24 Wall
    TILE_CRUMB,      //25 Wall
    TILE_CRUMB,      //26 Wall
    TILE_WALL,      //27 Wall

    TILE_DEADLY,    //28
    TILE_DEADLY,    //29
    TILE_EMPTY,     //30 Reverved Exit
    TILE_EMPTY,     //31 Reverved Exit
};

const unsigned char tile_class2[] = {
    TILE_EMPTY,     //00
    TILE_FLOOR,     //01
    TILE_CRUMB,     //02
    TILE_WALL,      //03
    TILE_CONVEYOR,  //04
    TILE_DEADLY,    //05
    TILE_DEADLY,    //06
    TILE_EXTRA,     //07 Switch

    TILE_FLOOR,     //08 Switch Off

    TILE_CRUMB,     //09 Crumb1
    TILE_CRUMB,     //10 Crumb2
    TILE_CRUMB,     //11 Crumb3

    TILE_FLOOR,     //12 Misc Platform
    TILE_OBJECT,    //13 Object
    TILE_EMPTY,     //14 Reverved Exit
    TILE_EMPTY,     //15 Reverved Exit

    TILE_EMPTY,     //16 Backgound
    TILE_EMPTY,     //17 Backgound
    TILE_EMPTY,     //18 Backgound
    TILE_EMPTY,     //19 Backgound

    TILE_FLOOR,     //20 Platform
    TILE_FLOOR,     //21 Platform
    TILE_FLOOR,     //22 Platform
    TILE_FLOOR,     //23 Platform

    TILE_WALL,      //24 Wall
    TILE_WALL,      //25 Wall
    TILE_WALL,      //26 Wall
    TILE_WALL,      //27 Wall

    TILE_DEADLY,    //28
    TILE_DEADLY,    //29
    TILE_EMPTY,     //30 Reverved Exit
    TILE_EMPTY,     //31 Reverved Exit
};
const unsigned char spr_init1[] = { //WILLY MODE
    64,   4, 4,   4, E_HORIZONTAL, 0xFF, 0xFF, 0xFF, 0xFF, // ROBOT
    66,   8, 4,   4, E_HORIZONTAL, 0x05, 0x07, 0xFF, 0xFF, // PENGUIN
    68,  12, 4,   4, E_HORIZONTAL, 0x03, 0x02, 0x04, 0xFF, // CHICKEN
    70,  16, 4,   4, E_HORIZONTAL, 0x03, 0x05, 0xFF, 0xFF, // FOCA
    72,  20, 4,   4, E_HORIZONTAL, 0x17, 0x10, 0xFF, 0xFF, // WATER
    74, 112, 4, 255, E_EUGENE    , 0xFF, 0xFF, 0xFF, 0xFF, // EUGENE
    76,  24, 4,   4, E_HORIZONTAL, 0x46, 0x43, 0x45, 0x06, // PAC
    78,  28, 4,   4, E_HORIZONTAL, 0x05, 0x03, 0x06, 0x02, // KANGOROO
    80,  32, 4,   4, E_HORIZONTAL, 0x45, 0x43, 0x44, 0x42, // BARRELL
    82, 104, 4,   0, E_STATIC    , 0xFF, 0xFF, 0xFF, 0xFF, // KONG
    84,  52, 4,   4, E_VERTICAL  , 0x03, 0x04, 0x05, 0x02, // AMEBATRON
    86,  36, 4,   4, E_HORIZONTAL, 0x04, 0x05, 0x02, 0x06, // CARRITO
    88,  40, 4,   4, E_HORIZONTAL, 0x05, 0x06, 0x02, 0x07, // EWOK
    90,  44, 4,   4, E_VERTICAL  , 0x03, 0x02, 0x04, 0x06, // TELEF
    92,  48, 4,   4, E_HORIZONTAL, 0x06, 0x04, 0x02, 0x04, // WEA
    94,  56, 4,   4, E_HORIZONTAL, 0x03, 0x04, 0x06, 0x02, // WEA 2
    96,  60, 4,   4, E_VERTICAL  , 0xFF, 0xFF, 0xFF, 0xFF, // EYE
    98,  64, 8,   3, E_SKYLAB    , 0x0F, 0x0D, 0x0E, 0xFF, // SKYLAB
   100,  72, 4,   4, E_VERTICAL  , 0x46, 0x47, 0x44, 0x43, // SAFE
   102,  76, 4,   0, E_HORIZONTAL, 0xFF, 0xFF, 0xFF, 0xFF, // BILL
   104,  80, 4,   4, E_HORIZONTAL, 0x45, 0x43, 0x46, 0x44, // MOFETA
   106,  84, 4,   4, E_VERTICAL  , 0x03, 0x06, 0x47, 0x01, // WEA 3
   108,  88, 4,   4, E_HORIZONTAL, 0x02, 0x05, 0xFF, 0xFF, // WEA CON PATAS
   110,  92, 4,   4, E_VERTICAL  , 0x03, 0x04, 0x05, 0x06, // AMEBATRON 2
   112,  96, 4,   4, E_VERTICAL  , 0x22, 0x26, 0x21, 0x24, // PELOTA(PAPER GREEN)
   114, 100, 4,   0, E_HORIZONTAL, 0x26, 0x21, 0x22, 0x27, // WEA SOLAR (PAPER GREEN)
   116, 106, 6,   0, E_FALL      , 0xFF, 0xFF, 0xFF, 0xFF, // KONG FALLING
   118, 112, 2,   0, E_STATIC    , 0xFF, 0xFF, 0xFF, 0xFF, // KONG JR
   120, 108, 4,   4, E_HORIZONTAL, 0xFF, 0xFF, 0xFF, 0xFF, // CARRITO FINAL
   122,  36, 4,   4, E_HORIZONTAL, 0x04, 0x03, 0x05, 0x06, // CARRITO COPIA PARA OTROS COLORES
};

const unsigned char texts[] = { //GAME TEXT
"Lorem ipsum dolor sit amet,     "
"consectetur adipiscing elit, sed"
"do eiusmod tempor incididunt ut "
"labore et dolore magna aliqua.  "
};



const unsigned char spr_init2[] = { //PIETRO MODE
    64,   4, 4,   4, E_HORIZONTAL, 0xFF, 0xFF, 0xFF, 0xFF, // TORTUGA
    66,   8, 4,   4, E_HORIZONTAL, 0x05, 0x07, 0xFF, 0xFF, // GOMPA
    68, 114, 2,   0, E_STATIC    , 0x03, 0x02, 0x04, 0xFF, // BOWSER
    70,  16, 4,   4, E_HORIZONTAL, 0x05, 0x03, 0xFF, 0xFF, // FOCA
    72,  20, 4,   4, E_HORIZONTAL, 0x17, 0x10, 0xFF, 0xFF, // WATER
    74,  54, 2,   2, E_VERTICAL  , 0xFF, 0xFF, 0xFF, 0xFF, // GOONIE BIRD
    76,  24, 4,   4, E_HORIZONTAL, 0x46, 0x43, 0x45, 0x06, // TRAMPA DKJR
    78,  28, 4,   4, E_HORIZONTAL, 0x05, 0x03, 0x06, 0x02, // AVE DKJR
    80,  32, 4,   4, E_HORIZONTAL, 0x45, 0x43, 0x44, 0x42, // KONG BARRELL
    82, 104, 4,   0, E_STATIC    , 0xFF, 0xFF, 0xFF, 0xFF, // KONG
    84,  36, 4,   4, E_VERTICAL  , 0x03, 0x05, 0x04, 0x02, // NODES
    86,  44, 4,   4, E_HORIZONTAL, 0x04, 0x05, 0x02, 0x06, // CORNER SPIDER
    88,  40, 4,   4, E_HORIZONTAL, 0x03, 0x02, 0x01, 0x07, // GERMEN HORIZONTAL
    90,  56, 4,   4, E_VERTICAL  , 0x03, 0x02, 0x04, 0x06, // GERMEN VERTICAL
    92,  48, 4,   4, E_HORIZONTAL, 0x45, 0x44, 0x42, 0x43, // CULEBRA
    94,  52, 2,   2, E_VERTICAL  , 0x4D, 0x4F, 0x4E, 0x02, // ABEJA SONIC
    96,  60, 4,   4, E_ZIGZAG    , 0xFF, 0xFF, 0xFF, 0xFF, // GAME OVER ALIEN
    98,  64, 4,   3, E_SKYLAB    , 0x0F, 0x0D, 0x0E, 0xFF, // GOTA ABU
   100,  68, 4,   4, E_HORIZONTAL, 0x46, 0x47, 0x44, 0x43, // PAJARO ABU
   102,  76, 4,   0, E_HORIZONTAL, 0x45, 0x43, 0x46, 0x42, // GHOST PACMAN
   104,  80, 4,   4, E_HORIZONTAL, 0x45, 0x43, 0x46, 0x44, // SONIC
   106,  84, 4,   4, E_ZIGZAG    , 0x02, 0x06, 0x47, 0x01, // ASTEROID
   108,  88, 4,   4, E_HORIZONTAL, 0x2A, 0x29, 0xFF, 0xFF, // SHY GUY
   110,  72, 4,   4, E_HORIZONTAL, 0x03, 0x04, 0x05, 0x06, // GAME OVER TROLL
   112,  96, 4,   4, E_HORIZONTAL, 0x03, 0x02, 0x01, 0x06, // TORTUGA NINJA+A1
   114, 100, 4,   4, E_HORIZONTAL, 0x05, 0x04, 0x02, 0x01, // NINJA
   116, 106, 6,   0, E_FALL      , 0xFF, 0xFF, 0xFF, 0xFF, // KONG FALLING
   118, 112, 2,   0, E_STATIC    , 0xFF, 0xFF, 0xFF, 0xFF, // KONG JR
   120,  92, 4,   4, E_HORIZONTAL, 0x04, 0x02, 0x03, 0x01, // POLLO
};
