==============================================================================

README - Manic Pietro v1.2


This games was build using the Pietro Engine, with the help of z88dk compiler and using NIRVANA+ for the multicolour graphic engine.


==============================================================================
HISTORY

Long after the destruction of the Ring, Manic Pietro comebacks for a revenge with the four last demons of Middle earth. Help him to free the world of the remaining evil of Melkor.

An arcade / rpg mix with four rounds of 16 screens each. Each Round have a an unique boss that have to be defeated to move to the next Round. Control can be by either 1 button or 2 button control. For the ZX Spectrum models with 128 Kb only.

Manic Pietro can gain experience, this will increase your fire range, levels are upgraded based your points: Also you can pick items that will increase your maximum health and mana. You can have a maximum of 200 health and mana.

Damage Table:
Deadly damage 50 points
Enemy damage 20 points
Bullet damage 10 points
Boss Bullet damage 10 points
Boss damage instant dead.

Coded on C using the powerfull z88dk compiler , the fabulous Nirvana Plus, and the genial Vortex II Player for AY music. The code is GPL’ed https://bitbucket.org/CmGonzalez/manic_pietro/

Code: Cristián González, Alvin Albrecht
Music: s9
FX: BeykerSoft
Testing:Abu Simbel


Good luck wizard!



BUILD INSTRUCTIONS:

* Install or update to the current Z88DK

https://github.com/z88dk/z88dk#installation

Compile z88dk
cd z88dk
chmod 777 build.sh
chmod 777 config.sh
./build.sh

Git clone if you will regularly update z88dk.
Nightly build if you just want to unzip the current build.

Manic Pietro uses zsdcc so linux users should make sure they also build zsdcc.
zsdcc is included among the pre-built Windows and OSX binaries already.

* Configure the nirvana+ library

Edit file "z88dk/libsrc/_DEVELOPMENT/target/zx/config_nirvanap.m4"
Change "define(`__NIRVANAP_TOTAL_ROWS', 23)" to "define(`__NIRVANAP_TOTAL_ROWS', 18)”

You can also disable wide tiles from Nirvana by changing, to save 300 bytes aprox.

define(`__NIRVANAP_OPTIONS_WIDE_DRAW',    0x00)
define(`__NIRVANAP_OPTIONS_WIDE_SPRITES', 0x00)

To build the Pentagon version instead of the regular Spectrum version:

Edit file "z88dk/libsrc/_DEVELOPMENT/target/zx/config_target.m4"
Change "define(`__SPECTRUM', 1)" to "define(`__SPECTRUM', 32)"

* Rebuild the zx library so that nirvana+ changes take effect

Open a shell and go to directory "z88dk/libsrc/_DEVELOPMENT"
Run "Winmake zx" (windows) or "make TARGET=zx" (anything else)

* Build

Open a shell in the game home directory
Run "make" to build taps.
Run "make zx7" (twice) to build zx7 compressed taps.
Run "make sna" to build 128k snapshot.

Windows users can build by running "zcompile" instead.

==============================================================================

RUNTIME MEMORY MAP:

23296 - 23583     Stack (288 bytes)
23584 - 56113ish  Game
56323 - 65378     Nirvana+ Engine (nirvana hole contains program variables)

128K ONLY:

BANK 3
49152 - ?

BANK 4
49152 - 65488     AY Music, Sound Effects and Players

BANK 6
49152 - 61895     AY Music, Sound Effects and Players, Map Data

==============================================================================



What's NEW

Version 1.03
  -Fix on Mushroom's not showing after brick hit.

Version 1.02
  -Player Jump Fix, now it won't stop at platforms

Version 1.01
  -Player Shoot Animation.
  -Performance improved
  -2 buttons alternative mode 1BB (up jumps, fire shoots, up+fire jumps on stairs)
  -Brick Hit picks items, a la SMB1
  -Level table updated (10 Level Max)
  -Player Shoot Sound fix.
  -Remove Red Dot on loading screen
  -Repaint menu choice after a game
  -Mushroom remove block by stopper tiles
  -Mushroom fix popup on screen
  -Game key repaint on screen after Round UP
  -Map fixes
  -Boss fire left range fixed
  -Speed up animations
  -Background paint fixes, strict function.
  -SPR_P1 / enemies different paint functions for performance
  -game_audio.c - audio abstract
  -game_menu.c - menu abstract
  -Tiles fixes
  -Corrected btiles paper restore after game end.
  -Game clear screen better routine
  -Code clean
