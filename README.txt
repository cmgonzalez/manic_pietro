==============================================================================

README - Manic Pietro v1.0


This games was build using the Pietro Engine, with the help of z88dk compiler and using NIRVANA+ for the multicolour graphic engine.


==============================================================================
HISTORY

Help Pietro on his journey, thought 20 different brand new levels, based on some of our favorites 8 bit games.

The brave souls that can finish this adventure, will get a secret code to unlock the original 20 Miner levels.

If you die on your quest you will get a code to continue playing from your last stage, with the lives you have when start it, but remember there's no swordfish for cheaters!. There's another more codes so keep playing!

A remake of the master piece "Manic Miner" from the genius Matthew Smith, for contest ZX DEV MIA remakes.

Only for 128kb spectrums, coded on C using z88dk with multicolor effects thanks to the Nirvana Plus Engine.

Good luck player!


Coding
Cristián González
Alvin Albrecht
Einar Saukas

Graphics and script
Cristián González
Igor Errazking

Sound and music
Sergio Vaquer "Beykersoft"
Davos

Loading screen
Mick Farrow

Testing
Dario Ruellan "Daríopedía"
Juan José Arenas "Canal de Juanjo"
Carlos Perezgrín
Federico Jerez "Abu Simbel"
Ed Verde




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

Version 1.0
-Initial Release
